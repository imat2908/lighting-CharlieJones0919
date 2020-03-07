
#include "scenediffuse.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
using std::cerr;
using std::endl;

#include "defines.h"

using glm::vec3;


#include <gtc/matrix_transform.hpp>
#include <gtx/transform2.hpp>


namespace imat2908
{
	/////////////////////////////////////////////////////////////////////////////////////////////
	// Default constructor
	/////////////////////////////////////////////////////////////////////////////////////////////
	SceneDiffuse::SceneDiffuse()
	{
	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	//Initialise the scene
	/////////////////////////////////////////////////////////////////////////////////////////////
	void SceneDiffuse::initScene(QuatCamera camera)
	{
		//|Compile and link the shader  
		compileAndLinkShader();

		gl::Enable(gl::DEPTH_TEST);

		for (int i = 0; i < numOfLightingParams; i++)
		{
			switch (i)
			{
			case 0:
				lightingParameter[i].paramIndex = 'a';
				lightingParameter[i].initalParam = vec3(0.3f, 0.3f, 0.3f);
				break;
			case 1:
				lightingParameter[i].paramIndex = 'd';
				lightingParameter[i].initalParam = vec3(0.9f, 0.9f, 0.9f);
				break;
			case 2:
				lightingParameter[i].paramIndex = 's';
				lightingParameter[i].initalParam = vec3(0.3f, 0.3f, 0.3f);
				break;
			}

			lightingParameter[i].currentVal = lightingParameter[i].initalParam;
		}

		//Set up the lighting
		setLightParams();

		//Create the plane to represent the ground
		plane = new VBOPlane(100.0, 100.0, 100, 100);

		//A matrix to move the teapot lid upwards
		glm::mat4 lid = glm::mat4(1.0);
		//lid *= glm::translate(vec3(0.0,1.0,0.0));

		//Create the teapot with translated lid
		teapot = new VBOTeapot(16, lid);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	//Update not used at present
	/////////////////////////////////////////////////////////////////////////////////////////////
	void SceneDiffuse::update()
	{
	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	// Set up the lighting variables in the shader
	/////////////////////////////////////////////////////////////////////////////////////////////
	void SceneDiffuse::setLightParams()
	{
		vec3 worldLight = vec3(10.0f, 10.0f, 10.0f);

		prog.setUniform("LightPosition", worldLight);
		prog.setUniform("Light.attenuation", 20.0f);

		for (int i = 0; i < numOfLightingParams; i++)
		{
			switch (i)
			{
			case 0:
				prog.setUniform("Light.La", lightingParameter[i].currentVal);
				break;
			case 1:
				prog.setUniform("Light.Ld", lightingParameter[i].currentVal);
				break;
			case 2:
				prog.setUniform("Light.Ls", lightingParameter[i].currentVal);
				break;
			}
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	// Render the scene
	/////////////////////////////////////////////////////////////////////////////////////////////
	void SceneDiffuse::render(QuatCamera camera)
	{
		gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

		//Initialise the model matrix for the plane
		model = mat4(1.0f);
		//Set the matrices for the plane although it is only the model matrix that changes so could be made more efficient
		setMatrices(camera);
		//Set the plane's material properties in the shader and render
		prog.setUniform("Material.Ka", 0.51f, 1.0f, 0.49f); // Values for diffusion's RGB colour.
		prog.setUniform("Material.Kd", 0.51f, 1.0f, 0.49f);
		prog.setUniform("Material.Ks", 0.1f, 0.1f, 0.1f);
		plane->render();	// Binds the vertex's VAO handle to the VAO then draws/renders them as triangles.

		//Now set up the teapot 
		model = mat4(1.0f);
		setMatrices(camera);
		//Set the Teapot material properties in the shader and render
		prog.setUniform("Material.Ka", 0.46f, 0.29f, 0.0f); // Values for diffusion's RGB colour.
		prog.setUniform("Material.Kd", 0.46f, 0.29f, 0.0f);
		prog.setUniform("Material.Ks", 0.29f, 0.29f, 0.29f);
		teapot->render();	// Binds the vertex's VAO handle to the VAO then draws/renders them as triangles.
	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	//Send the MVP matrices to the GPU
	/////////////////////////////////////////////////////////////////////////////////////////////
	void SceneDiffuse::setMatrices(QuatCamera camera)
	{
		mat4 mv = camera.view() * model;
		prog.setUniform("ModelViewMatrix", mv);
		prog.setUniform("NormalMatrix", mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
		prog.setUniform("MVP", camera.projection() * mv);
		mat3 normMat = glm::transpose(glm::inverse(mat3(model))); // Inverses and transposes the co-ordinate system of the model.
		prog.setUniform("M", model);
		prog.setUniform("V", camera.view());
		prog.setUniform("P", camera.projection());
	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	// resize the viewport
	/////////////////////////////////////////////////////////////////////////////////////////////
	void SceneDiffuse::resize(QuatCamera camera, int w, int h)
	{
		gl::Viewport(0, 0, w, h);
		width = w;
		height = h;
		camera.setAspectRatio((float)w / h);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	// Compile and link the shader
	/////////////////////////////////////////////////////////////////////////////////////////////
	void SceneDiffuse::compileAndLinkShader()
	{
		try {
			prog.compileShader("Shaders/phong.vert");
			prog.compileShader("Shaders/phong.frag");
			prog.link();
			prog.validate();
			prog.use();
		}
		catch (GLSLProgramException & e) {
			cerr << e.what() << endl;
			exit(EXIT_FAILURE);
		}
	}

	void SceneDiffuse::animate(bool &shift, bool &a, bool &d, bool &s, bool &r)
	{
		if (r)
		{
			for (int i = 0; i < numOfLightingParams; i++)
			{
				lightingParameter[i].currentVal = lightingParameter[i].initalParam;
			}
			r = false;
		}
		else
		{
			unsigned int paramNum;
			bool paramButton[3] = { a, d, s };
			vec3 curParamVal;

			for (int i = 0; i < numOfLightingParams; i++)
			{
				if (paramButton[i])
				{
					paramNum = i;
					curParamVal = lightingParameter[paramNum].currentVal;

					if (!shift)
					{
						lightingParameter[paramNum].currentVal += paramIncrement;
					}
					else
					{
						lightingParameter[paramNum].currentVal -= paramIncrement;
					}

					i = numOfLightingParams;
				}
			}
		}

		setLightParams();

		std::cout << "Ambient: " << lightingParameter[0].currentVal.x << " " << a << endl;
		std::cout << "Diffuse: " << lightingParameter[1].currentVal.x << " " << d << endl;
		std::cout << "Specular: " << lightingParameter[2].currentVal.x << " " << s << endl;
		std::cout << endl;
	}
}