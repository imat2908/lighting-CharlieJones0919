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

	const unsigned int numOfLightingParams = 3;			// Number of lighting elements. (Ambience, Diffusion and Specularity).
	vec3 paramIncrement = vec3(0.01f, 0.01f, 0.01f);	// Amount the lighting parameters are increase or decreased by on key press.

	struct LightingParam	// Varaibles for each of the lighting elements to reuse.
	{
		vec3 initalParam;	// What the parameter started as before user input.
		vec3 currentVal;	// The current value of the parameter as changed by user input.
	};
	LightingParam lightingParameter[3]; // An index for the 3 lighting parameter's values. ([0] = Ambient, [1] = Diffuse, [2] = Specular).

	VBOTeapot *teapot;  // Teapot VBO.
	VBOPlane *plane;  // Plane VBO.

    mat4 model; // Model matrix.

    void setMatrices(QuatCamera camera); // Set the camera matrices.

    void compileAndLinkShader(); // Compile and link the shader.

public:
    SceneDiffuse(); // Constructor.

	void setLightParams();				// Setup the lighting's parameters.

    void initScene(QuatCamera camera);	// Initialise the scene.

    void render(QuatCamera camera);		// Render the scene.

    void resize(QuatCamera camera, int, int); // Resize.

	void animate(bool &shift, bool &a, bool &d, bool &s, bool &r); // Used to update the lighting parameters based on the user's keyboard input.
};
}

#endif // SCENEDIFFUSE_H