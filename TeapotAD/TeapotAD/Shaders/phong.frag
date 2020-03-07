#version 430

/////////////////////////////////////////////////////////////////////////
/////  Data Passed into the Fragment Shader from the Vertex Shader  /////
/////////////////////////////////////////////////////////////////////////
in Data 
{
	vec3 N;			// Vertex Normal as Translated into Worldspace by the Vertex Shader.
	vec3 lightPos;  // Light's Position as Translated into Worldspace by the Vertex Shader.
	vec3 vertPos;   // Models Vertexs' Position as Translated into Worldspace by the Vertex Shader.
} data;				// Object of the Data structure to hold the input variables.

////////////////////////////////////////////////////////////////////
/////////////////////  The Light's Properties  /////////////////////
////////////////////////////////////////////////////////////////////
struct LightData
{
	float attenuation;  // Intensity of Attenuation.
	vec3 La;            // Ambient Light Intensity.
	vec3 Ld;            // Diffuse Light Intensity.
	vec3 Ls;            // Specular Light Intensity.
};

////////////////////////////////////////////////////////////////////////
/////////////////////  The Materials's Properties  /////////////////////
////////////////////////////////////////////////////////////////////////
struct MaterialData		
{
	vec3 Ka;            // Ambient Reflectivity in Material
	vec3 Kd;            // Diffusion Reflectivity in Material
	vec3 Ks;            // Specular Reflectivity in Material
};

uniform LightData Light;		// The Uniform Light Source for the Parameters to be Passed into.
uniform MaterialData Material;	// The Uniform Model Material for the Parameters to be Passed into.

layout( location = 0 ) out vec4 FragColour; // The Final Output Fragment Colour with Consideration of the Lighting and Materials' Properties.

/////////////////////////////////////////////////////////////////////////////////////////////
//////  Function to Calculate the Colour Output Contribution of Each Lighting Element  //////
/////////////////////////////////////////////////////////////////////////////////////////////
void light(vec3 N, vec3 vertPos, vec3 lightPos, vec3 La, vec3 Ld, vec3 Ls, vec3 Ka, vec3 Kd, vec3 Ks, out vec4 ambience, out vec4 diffusion, out vec4 specularity)
{
	vec3 vectorsNorm = normalize(lightPos - vertPos);					// Gets the difference between the light's position and the material vertex's position, then normalizes it to a unit vector so it just indicates direction.

/////////////////////////////////
///// AMBIENCE CONTRIBUTION /////
/////////////////////////////////
	ambience = clamp(vec4(vec4(La, 1.0) * vec4(Ka, 1.0)), 0.0, 1.0);	// Ambient lighting intensity multiplied with the material reflectivity.					

//////////////////////////////////
///// DIFFUSION CONTRIBUTION /////
//////////////////////////////////
	vec4 Id = vec4(Ld, 1.0) * max(dot(N, vectorsNorm), 0.0);			// Substitution for cos(theta) * Ld.
	Id = clamp(Id, 0.0, 1.0);											// Contrains the value of specular intensity between 0 and 1. Because the light intensity can't be negative, or more than 1.				
	diffusion = vec4(Kd,1.0) * Id;										// The diffused light intensity multiplied with the material's reflectivity.

////////////////////////////////////
///// SPECULARITY CONTRIBUTION /////
////////////////////////////////////
	vec3 normalisedVertPos = normalize(-vertPos);						// Inverted model vertex position.
	vec3 reflection = reflect(-vectorsNorm, N);							// Inverted model vertex position reflected across its normal.
		
	vec4 Is = vec4(pow(max(dot(reflection, normalisedVertPos), 0.0), 1.0));	// Substitution for cos^normal(angle between light and object).
	Is = clamp(Is, 0.0, 1.0);											// Contrains the value of specular intensity between 0 and 1. Because the light intensity can't be negative, or more than 1.	
	specularity = vec4(vec4(Ls, 1.0) * vec4(Ks, 1.0) * Is);				// The specular light intensity multiplied with the material's reflectivity and the model's curviture.
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//////  Function to Output the Sum of all the Lighting Elements as Subject to the Attenuation  //////
/////////////////////////////////////////////////////////////////////////////////////////////////////
void attenuate(float attenuation, vec3 lightPos, vec3 vertPos, vec4 ambi, vec4 diff, vec4 spec, out vec4 final)
{
	float dist = length(lightPos - vertPos);				// Distance of the light source to the model's vertex.
	float atten = clamp(attenuation / dist, 0.0, 1.0);		// Clamps the attenuation between 0 and 1;

	final = atten * (ambi + diff + spec);					// The sum of the lighting elements multiplied with the distance of the model's vertex to the light.
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///// Main Function to Call the Lighting Calculations and Return the Resultant Fragment Colour /////
////////////////////////////////////////////////////////////////////////////////////////////////////
void main() 
{
	// Variables to Hold the Output Lighting Values
	vec4 ambience, diffusion, specularity, final;

	// Calling the Light Function
	light(data.N, data.vertPos, data.lightPos, Light.La, Light.Ld, Light.Ls, Material.Ka, Material.Kd, Material.Ks, ambience, diffusion, specularity);

	// Attenuating and Combining the Lighting Element's Output
	attenuate(Light.attenuation, data.lightPos, data.vertPos, ambience, diffusion, specularity, final); // Results Output into "final".

	// Final Fragment Colour Output as Subject to the Lighting and Materials' Properties
	FragColour = final;
}
