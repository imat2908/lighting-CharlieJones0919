#version 430

layout (location = 0) in vec3 VertexPosition; // Input of the models vertexs' local position.
layout (location = 1) in vec3 VertexNormal;	  // Input of the models vertexs' local normal.

///////////////////////////////////////////////////////////////////////////
/////  Data Passed out of the Vertex Shader into the Fragment Shader  /////
///////////////////////////////////////////////////////////////////////////
out Data	
{
	vec3 N;				 // Normal transformed into the eye co-ordinates.
	vec3 lightPos;		 // Light's position transformed into the eye co-ordinates. (Camera plane).
	vec3 vertPos;		 //	Models vertexs' position transformed into the eye co-ordinates.
} data;					 // Object of the Data structure to hold the output variables.

///////////////////////////////////////////////////////////
/////////////////////  Matrices Data  /////////////////////
///////////////////////////////////////////////////////////
struct MatrixData
{
	vec3 LightPosition; // Light's Position as Declared in scenediffuse.cpp's setLightParams() function.
	mat3 NormalMatrix;	// Model's Vertex Normal Multiplied with the Camera's View in scenediffuse.cpp's setMatricies() function.
	mat4 M;				// Model's Matrix.
	mat4 V;				// Camera View Matrix.
	mat4 P;				// Camera Projection Matrix.
};	
uniform MatrixData matrixProperties;	// Object of the MatrixData structure to hold the declared variables.

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////  Main Function Return the Objects in the Scene's Local Vertex Positions Transformed into the Eye Co-ordinates  /////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void main()
{
   data.N = normalize( matrixProperties.NormalMatrix * VertexNormal);														// Translation of the Local Vertex Normal
   data.lightPos = vec3(matrixProperties.V * matrixProperties.M * vec4(matrixProperties.LightPosition, 1.0));				// Translation of the Local Light Position
   data.vertPos = vec3(matrixProperties.V * matrixProperties.M * vec4(VertexPosition, 1.0));								// Translation of the Local Models Vertexs' Position

   gl_Position = gl_Position = matrixProperties.P * matrixProperties.V * matrixProperties.M * vec4(VertexPosition, 1.0);	// Unused gl_Position Variable
}
