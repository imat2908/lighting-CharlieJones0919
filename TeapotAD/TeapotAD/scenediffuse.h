#ifndef SCENEDIFFUSE_H
#define SCENEDIFFUSE_H

#include "gl_core_4_3.hpp"

#include <glfw3.h>
#include "scene.h"
#include "glslprogram.h"

#include "vboteapot.h"
#include "vboplane.h"

#include <glm.hpp>

using glm::mat4;

namespace imat2908
{

class SceneDiffuse : public Scene
{
private:
    GLSLProgram prog;

    int width, height;

	const unsigned int numOfLightingParams = 3;
	vec3 paramIncrement = vec3(0.01f, 0.01f, 0.01f);

	struct LightingParam
	{
		char paramIndex;
		vec3 initalParam;
		vec3 currentVal;
	};

	LightingParam lightingParameter[3];

	VBOTeapot *teapot;  //Teapot VBO
	VBOPlane *plane;  //Plane VBO

    mat4 model; //Model matrix

    void setMatrices(QuatCamera camera); //Set the camera matrices

    void compileAndLinkShader(); //Compile and link the shader

public:
    SceneDiffuse(); //Constructor

	void setLightParams(); //Setup the lighting

    void initScene(QuatCamera camera); //Initialise the scene

    void update(); //Update the scene

    void render(QuatCamera camera);	//Render the scene

    void resize(QuatCamera camera, int, int); //Resize

	void animate(bool &shift, bool &a, bool &d, bool &s, bool &r);
};
}

#endif // SCENEDIFFUSE_H