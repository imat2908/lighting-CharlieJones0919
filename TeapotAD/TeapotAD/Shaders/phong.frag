#version 430

in vec3 N;
in vec3 vertPos;
in vec3 lightPos;

uniform vec3 La;            // Ambient Light intensity
uniform vec3 Ld;            // Diffuse Light intensity
uniform vec3 Ls;            // Specular Light intensity

uniform vec3 Ka;            // Ambient Reflectivity in Material
uniform vec3 Kd;            // Diffusion Reflectivity in Material
uniform vec3 Ks;            // Specular Reflectivity in Material

layout( location = 0 ) out vec4 FragColour;

vec4 ambientOut(vec3 La, vec3 Ka)
{
	return clamp(vec4(vec4(La, 1.0) * vec4(Ka, 1.0)), 0.0, 1.0);	// Returns the ambient lighting intensity multiplied with the material reflectivity.					
}

vec4 diffuseOut(vec3 N, vec3 vertPos, vec3 LightPos, vec3 Ld, vec3 Kd)
{
	vec3 vectorsNorm = normalize(lightPos - vertPos);				// Gets the difference between the light's position and the material vertex's position, then normalizes it to a unit vector so it just indicates direction.
	vec4 Id = vec4(Ld, 1.0) * max(dot(N, vectorsNorm), 0.0);		// Substitution for cos(theta) * Ld.
	Id = clamp(Id, 0.0, 1.0);										// Contrains the value of specular intensity between 0 and 1. Because the light intensity can't be negative, or more than 1.				
	return vec4(Kd,1.0) * Id;										// The diffused light intensity multiplied with the material's reflectivity.
}

vec4 specularOut(vec3 N, vec3 vertPos, vec3 LightPos, vec3 Ls, vec3 Ks)
{
	vec3 vectorsNorm = normalize(lightPos - vertPos);				// Gets the difference between the light's position and the material vertex's position, then normalizes it to a unit vector so it just indicates direction. 
	vec3 normalisedVertPos = normalize(-vertPos);					// Inverted model vertex position.
	vec3 reflection = reflect(-vectorsNorm, N);						// Inverted model vertex position reflected across its normal.
		
	vec4 Is = vec4(pow(max(dot(reflection, normalisedVertPos), 0.0), 1.0));	// Substitution for cos^normal(angle between light and object).
	Is = clamp(Is, 0.0, 1.0);										// Contrains the value of specular intensity between 0 and 1. Because the light intensity can't be negative, or more than 1.	
	return vec4(vec4(Ls, 1.0) * vec4(Ks, 1.0) * Is);				// The specular light intensity multiplied with the material's reflectivity and the model's curviture.
}

void main() 
{
	// Final Fragment Colour
	FragColour = ambientOut(La, Ka) + diffuseOut(N, vertPos, lightPos, Ld, Kd) + specularOut(N, vertPos, lightPos, Ls, Ks);
}
