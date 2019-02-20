#version 330 core

// MAX_LIGHTS_NUMBER is defined in Scene.h and the
// define statement is inserted by the ShaderProgram::LoadShaders function

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 fragTexCoords;

// Camera
uniform vec3 cameraLocation;

// Textures
uniform sampler2D textureMap;
uniform bool useTextures;

// Light
uniform int numberOfLights;
uniform vec3 lightsPositions[MAX_LIGHTS_NUMBER];
uniform vec4 lightColors[MAX_LIGHTS_NUMBER];

// Model
uniform vec4 ambiantColor;
uniform vec4 ambiantLighting;
uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform float shininess;

// The final color of the fragment (pixel)
out vec4 frag_color;

vec4 calculateDiffusePart( vec3 lightSourceLocation, vec4 lightColor);
vec4 calculateTextureDiffusePart(vec3 lightSourceLocation, vec4 lightColor);
vec4 calculateSpecularPart(vec3 lightSourceLocation, vec4 lightColor);

void main()
{
	vec4 diffusePartSum =  vec4(0.0f);
	vec4 specularPartSum = vec4(0.0f);
	
	if(useTextures)
	{
		for(int i = 0; i < numberOfLights; i++)
		{
			diffusePartSum  = diffusePartSum  + calculateTextureDiffusePart(lightsPositions[i],lightColors[i]);
			specularPartSum = specularPartSum + calculateSpecularPart(lightsPositions[i],lightColors[i]);
		}
	}
	else
	{
		for(int i = 0; i < numberOfLights; i++)
		{
			diffusePartSum  = diffusePartSum  + calculateDiffusePart(lightsPositions[i],lightColors[i]);
			specularPartSum = specularPartSum + calculateSpecularPart(lightsPositions[i],lightColors[i]);
		}
	}

	frag_color = ambiantColor * ambiantLighting + diffusePartSum + specularPartSum;
}

vec4 calculateDiffusePart(vec3 lightSourceLocation, vec4 lightColor)
{
	vec4 directionToLight = vec4(normalize(lightSourceLocation - fragPosition),0.0f);
	vec4 normalVector = vec4(fragNormal,0.0f);
	float scalar = dot(directionToLight, normalVector);
	vec4 result = diffuseColor * scalar * lightColor;
	
	return result;
}

vec4 calculateTextureDiffusePart(vec3 lightSourceLocation, vec4 lightColor)
{
	vec4 directionToLight = vec4(normalize(lightSourceLocation - fragPosition),0.0f);
	vec4 normalVector = vec4(fragNormal,0.0f);
	float scalar = dot(directionToLight, normalVector);
	vec4 result = texture(textureMap, fragTexCoords) * scalar * lightColor;
	
	return result;
}

vec4 calculateSpecularPart(vec3 lightSourceLocation, vec4 lightColor)
{
	// Explanation is here: http://learnwebgl.brown37.net/09_lights/lights_specular.html
	vec4 normalVector = normalize(vec4(fragNormal,0.0f));
	vec4 directionToLight = vec4(normalize(lightSourceLocation - fragPosition),0.0f);
	float normalFactor = dot(directionToLight,normalVector);
	vec4 N = normalVector * normalFactor; // light source projection on the normal
	vec4 P = vec4(N - vec4(lightSourceLocation,0.0f)); // Light source point to N
	vec4 R = 2 * (dot(directionToLight, normalVector)) * normalVector - directionToLight;

	vec4 V = vec4(normalize(cameraLocation - fragPosition),0.0f); // Direction to camera
	float relectionCameraDotProduct = max(dot(R,V),0.0f);
	float specularFactor = pow(relectionCameraDotProduct,shininess);
	specularFactor = max(specularFactor,0.001f);
	vec4 result = specularColor * specularFactor * lightColor; 
	
	return result;
}
