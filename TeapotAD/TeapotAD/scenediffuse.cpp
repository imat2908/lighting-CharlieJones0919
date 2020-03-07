
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
	///// Default Constructor
	/////////////////////////////////////////////////////////////////////////////////////////////
	SceneDiffuse::SceneDiffuse()
	{
	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	///// Initialise the Scene
	/////////////////////////////////////////////////////////////////////////////////////////////
	void SceneDiffuse::initScene(QuatCamera camera)
	{
		//|Compile and link the shader  
		compileAndLinkShader();

		gl::Enable(gl::DEPTH_TEST);

		// Set the Initial and Current Lighting Parameters for Each of the Lighting Elements
		for (int i = 0; i < numOfLightingParams; i++)
		{
			switch (i)
			{
			case 0:
				lightingParameter[i].initalParam = vec3(0.3f, 0.3f, 0.3f);	// Ambience Initialised
				break;
			case 1:
				lightingParameter[i].initalParam = vec3(0.9f, 0.9f, 0.9f);	// Diffusion Initialised
				break;
			case 2:
				lightingParameter[i].initalParam = vec3(0.3f, 0.3f, 0.3f);	// Specularity Initialised
				break;
			}

			lightingParameter[i].currentVal = lightingParameter[i].initalParam;	// All of the element's current lighting parameters set to their respective initial parameters.
		}

		// Set up the lighting.
		setLightParams();

		// Create the plane to represent the ground.
		plane = new VBOPlane(100.0, 100.0, 100, 100);

		// A matrix to move the teapot lid upwards.
		glm::mat4 lid = glm::mat4(1.0);
		lid *= glm::translate(vec3(0.0,1.0,0.0));

		//Create the teapot with translated lid.
		teapot = new VBOTeapot(16, lid);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	// Set up the lighting variables in the shader.
	/////////////////////////////////////////////////////////////////////////////////////////////
	void SceneDiffuse::setLightParams()
	{
		vec3 worldLight = vec3(10.0f, 10.0f, 10.0f);	// Position of the light source.

		prog.setUniform("matrixProperties.LightPosition", worldLight);	// Setting the light position to its uniform value in the vertex shader.
		prog.setUniform("Light.attenuation", 20.0f);					// Setting the light attenuation to its uniform value in the fragment shader.

		// Setting each of the lighting element's intensities in their respective new current values to their uniform values in the fragment shader.
		for (int i = 0; i < numOfLightingParams; i++)
		{
			switch (i)
			{
			case 0:
				prog.setUniform("Light.La", lightingParameter[i].currentVal);	// Setting the ambience to its current value.
				break;
			case 1:
				prog.setUniform("Light.Ld", lightingParameter[i].currentVal);	// Setting the diffusion to its current value.
				break;
			case 2:
				prog.setUniform("Light.Ls", lightingParameter[i].currentVal);	// Setting the specularity to its current value.
				break;
			}
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	///// Render the scene to the camera.
	/////////////////////////////////////////////////////////////////////////////////////////////
	void SceneDiffuse::render(QuatCamera camera)
	{
		gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);	// Clear the buffers.

		// Initialise the model matrix for the plane.
		model = mat4(1.0f);
		// Set the matrices for the plane although it is only the model matrix that changes so could be made more efficient.
		setMatrices(camera);
		// Set the plane's material properties in the shader and render.
		prog.setUniform("Material.Ka", 0.51f, 1.0f, 0.49f); // Values for ambience's RGB colour.
		prog.setUniform("Material.Kd", 0.51f, 1.0f, 0.49f); // Values for diffusion's RGB colour.
		prog.setUniform("Material.Ks", 0.1f, 0.1f, 0.1f);	// Values for specularity's RGB colour.
		plane->render();	// Binds the vertex's VAO handle to the VAO then draws/renders them as triangles.

		// Initialise the model matrix for the teapot. 
		model = mat4(1.0f);
		setMatrices(camera);
		// Set the Teapot material properties in the shader and render.
		prog.setUniform("Material.Ka", 0.46f, 0.29f, 0.0f);  // Values for ambience's RGB colour.
		prog.setUniform("Material.Kd", 0.46f, 0.29f, 0.0f);  // Values for diffusion's RGB colour.
		prog.setUniform("Material.Ks", 0.29f, 0.29f, 0.29f); // Values for specularity's RGB colour.
		teapot->render();	// Binds the vertex's VAO handle to the VAO then draws/renders them as triangles.
	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	// Send the MVP matrices to the GPU/Vertex Shader
	/////////////////////////////////////////////////////////////////////////////////////////////
	void SceneDiffuse::setMatrices(QuatCamera camera)
	{
		mat4 mv = camera.view() * model;							// The model's matrix translated into the camera view or eye co-ordinates.
		prog.setUniform("ModelViewMatrix", mv);						
		prog.setUniform("matrixProperties.NormalMatrix", mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
		prog.setUniform("MVP", camera.projection() * mv);
		mat3 normMat = glm::transpose(glm::inverse(mat3(model)));	// Inverses and transposes the co-ordinate system of the model.
		prog.setUniform("matrixProperties.M", model);
		prog.setUniform("matrixProperties.V", camera.view());
		prog.setUniform("matrixProperties.P", camera.projection());
	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	// Resize the viewport.
	/////////////////////////////////////////////////////////////////////////////////////////////
	void SceneDiffuse::resize(QuatCamera camera, int w, int h)
	{
		gl::Viewport(0, 0, w, h);
		width = w;
		height = h;
		camera.setAspectRatio((float)w / h);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	// Compile and link the shader.
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
		// If the "R" key has is being pressed, reset all the lighting element's parameters back to their initial values then set the key back to false.
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
			unsigned int paramNum;				// Which parameter needs changing. (0 = Ambience, 1 = Diffusion, 3 = Specularity).
			bool paramButton[3] = { a, d, s };	// An array so all the buttons can be looped through for checking if they are currently true.

			for (int i = 0; i < numOfLightingParams; i++)
			{
				// If the current key boolean is true, set the paramNum to this lighting element's index number. If the shift key is also down, decrease the element's intensity, otherwise increase it.
				if (paramButton[i])
				{
					paramNum = i;

					if (!shift)
					{
						lightingParameter[paramNum].currentVal += paramIncrement;
					}
					else
					{
						lightingParameter[paramNum].currentVal -= paramIncrement;
					}
				}
			}
		}

		setLightParams();	// Update the lighting values to their new current values.

		// Outputs the changable lighting values and if their buttons are down to the console.
		std::cout << "Ambient: " << lightingParameter[0].currentVal.x << " " << a << endl;
		std::cout << "Diffuse: " << lightingParameter[1].currentVal.x << " " << d << endl;
		std::cout << "Specular: " << lightingParameter[2].currentVal.x << " " << s << endl;
		std::cout << endl;
	}
}