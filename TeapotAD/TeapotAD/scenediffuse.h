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
 
	VBOTeapot *teapot;  //Teapot VBO
	VBOPlane *plane;  //Plane VBO

    mat4 model; //Model matrix

    void setMatrices(QuatCamera camera); //Set the camera matrices

    void compileAndLinkShader(); //Compile and link the shader

public:
    SceneDiffuse(); //Constructor

	vec3 initialLa = vec3(0.3f, 0.3f, 0.3f);
	vec3 initialLd = vec3(0.9f, 0.9f, 0.9f);
	vec3 initialLs = vec3(0.3f, 0.3f, 0.3f);

	vec3 curLa = initialLa;
	vec3 curLd = initialLd;
	vec3 curLs= initialLs;


	void setLightParams(); //Setup the lighting

    void initScene(QuatCamera camera); //Initialise the scene

    void update(); //Update the scene

    void render(QuatCamera camera);	//Render the scene

    void resize(QuatCamera camera, int, int); //Resize

	void animate(bool &shift, bool &a, bool &d, bool &s, bool &r);
};

}

#endif // SCENEDIFFUSE_H
