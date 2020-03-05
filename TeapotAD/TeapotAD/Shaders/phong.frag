#version 430

in Data 
{
	vec3 N;
	vec3 lightPos;
	vec3 vertPos;
} data;

struct LightData
{
	vec3 La;            // Ambient Light intensity
	vec3 Ld;            // Diffuse Light intensity
	vec3 Ls;            // Specular Light intensity
};

struct MaterialData
{
	vec3 Ka;            // Ambient Reflectivity in Material
	vec3 Kd;            // Diffusion Reflectivity in Material
	vec3 Ks;            // Specular Reflectivity in Material
};


uniform LightData Light;
uniform MaterialData Material;

layout( location = 0 ) out vec4 FragColour;

void light(vec3 N, vec3 vertPos, vec3 lightPos, vec3 La, vec3 Ld, vec3 Ls, vec3 Ka, vec3 Kd, vec3 Ks, out vec4 ambience, out vec4 diffusion, out vec4 specularity)
{
	vec3 vectorsNorm = normalize(lightPos - vertPos);					// Gets the difference between the light's position and the material vertex's position, then normalizes it to a unit vector so it just indicates direction.

////AMBIENCE////
	ambience = clamp(vec4(vec4(La, 1.0) * vec4(Ka, 1.0)), 0.0, 1.0);	// Returns the ambient lighting intensity multiplied with the material reflectivity.					

////DIFFUSION////
	vec4 Id = vec4(Ld, 1.0) * max(dot(N, vectorsNorm), 0.0);			// Substitution for cos(theta) * Ld.
	Id = clamp(Id, 0.0, 1.0);											// Contrains the value of specular intensity between 0 and 1. Because the light intensity can't be negative, or more than 1.				
	diffusion = vec4(Kd,1.0) * Id;										// The diffused light intensity multiplied with the material's reflectivity.

////SPECULARITY////
	vec3 normalisedVertPos = normalize(-vertPos);						// Inverted model vertex position.
	vec3 reflection = reflect(-vectorsNorm, N);							// Inverted model vertex position reflected across its normal.
		
	vec4 Is = vec4(pow(max(dot(reflection, normalisedVertPos), 0.0), 1.0));	// Substitution for cos^normal(angle between light and object).
	Is = clamp(Is, 0.0, 1.0);											// Contrains the value of specular intensity between 0 and 1. Because the light intensity can't be negative, or more than 1.	
	specularity = vec4(vec4(Ls, 1.0) * vec4(Ks, 1.0) * Is);				// The specular light intensity multiplied with the material's reflectivity and the model's curviture.
}

void main() 
{
	vec4 ambience, diffusion, specularity;
	light(data.N, data.vertPos, data.lightPos, Light.La, Light.Ld, Light.Ls, Material.Ka, Material.Kd, Material.Ks, ambience, diffusion, specularity);

	// Final Fragment Colour
	FragColour = ambience + diffusion + specularity;
}
