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

void main() 
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;

	// Specular Lighting Output
	vec3 vectorsNorm = normalize(lightPos - vertPos);				// Gets the difference between the light's position and the material vertex's position, then normalizes it to a unit vector so it just indicates direction.
	vec4 Id = vec4(Ld, 1.0) * max(dot(N, vectorsNorm), 0.0);		// Substitution for cos(theta). The dot product of the normalised vectors.
	Id = clamp(Id, 0.0, 1.0);										// Contrains the value of specular intensity between 0 and 1. Because the light intensity can't be negative, or more than 1.				
	diffuse = vec4(Kd,1.0) * Id;									// The diffused light intensity's dot product with the material's reflectivity.

	FragColour = diffuse;
}
