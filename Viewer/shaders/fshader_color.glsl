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

// Toon shading
uniform bool useToonShading;
uniform int toonShadingLevels;

// Model
uniform vec4 ambiantColor;
uniform vec4 ambiantLighting;
uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform float shininess;

// The final color of the fragment (pixel)
out vec4 frag_color;

vec4 calculateDiffusePart( vec3 lightSourceLocation, vec4 lightColor);
vec4 calculateSpecularPart(vec3 lightSourceLocation, vec4 lightColor);
vec4 getDiffuseColor(float brightness, vec4 lightColor);
float getBrightness(vec4 directionToLight, vec4 normalVector);
float getDampedFactor(float relectionCameraDotProduct);
vec4 capValuesAtOne(vec4 vector);

void main()
{
	vec4 diffusePartSum =  vec4(0.0f);
	vec4 specularPartSum = vec4(0.0f);
	
	for(int i = 0; i < numberOfLights; i++)
	{
		diffusePartSum  = diffusePartSum  + calculateDiffusePart(lightsPositions[i],lightColors[i]);
		specularPartSum = specularPartSum + calculateSpecularPart(lightsPositions[i],lightColors[i]);
	}
	diffusePartSum = capValuesAtOne(diffusePartSum);
	specularPartSum = capValuesAtOne(specularPartSum);

	frag_color = capValuesAtOne(ambiantColor * ambiantLighting + diffusePartSum + specularPartSum);
}

vec4 capValuesAtOne(vec4 vector)
{
	vector.x = min (vector.x, 1.0f);
	vector.y = min (vector.y, 1.0f);
	vector.z = min (vector.z, 1.0f);
	vector.w = min (vector.w, 1.0f);
	return vector;
}

vec4 calculateDiffusePart(vec3 lightSourceLocation, vec4 lightColor)
{
	vec4 directionToLight = vec4(normalize(lightSourceLocation - fragPosition),0.0f);
	vec4 normalVector = vec4(fragNormal,0.0f);
	float brightness = getBrightness(directionToLight, normalVector);
	vec4 result = getDiffuseColor(brightness, lightColor);
	
	return result;
}

float getBrightness(vec4 directionToLight, vec4 normalVector)
{
	float brightness = dot(directionToLight, normalVector);
	if(useToonShading)
	{
		float level = floor(brightness * toonShadingLevels);
		brightness = level/toonShadingLevels;
	}
	return brightness;
}

vec4 getDiffuseColor(float brightness, vec4 lightColor)
{
	if(useTextures) 
	{
		vec4 color = vec4(texture(textureMap, fragTexCoords) * brightness * lightColor);
		if(useToonShading)
		{
			float level = floor(color.r * toonShadingLevels);
			color.r = level/toonShadingLevels;
			level = floor(color.g * toonShadingLevels);
			color.g = level/toonShadingLevels;
			level = floor(color.b * toonShadingLevels);
			color.b = level/toonShadingLevels;
		}
		return color;
	}
	return vec4(diffuseColor * brightness * lightColor);
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
	float dampedFactor = getDampedFactor(relectionCameraDotProduct);
	vec4 result = specularColor * dampedFactor * lightColor; 
	
	return result;
}

float getDampedFactor(float relectionCameraDotProduct)
{
	float dampedFactor = pow(relectionCameraDotProduct,shininess);
	dampedFactor = max(dampedFactor,0.001f);
	//if(useToonShading)
	//{
		// do it
	//}
	return dampedFactor;
}

vec4 calculateToonSpecularPart(vec3 lightSourceLocation, vec4 lightColor)
{
	vec4 normalVector = normalize(vec4(fragNormal,0.0f));
	vec4 directionToLight = vec4(normalize(lightSourceLocation - fragPosition),0.0f);
	float normalFactor = dot(directionToLight,normalVector);
	vec4 N = normalVector * normalFactor; // light source projection on the normal
	vec4 P = vec4(N - vec4(lightSourceLocation,0.0f)); // Light source point to N
	vec4 R = 2 * (dot(directionToLight, normalVector)) * normalVector - directionToLight;

	vec4 V = vec4(normalize(cameraLocation - fragPosition),0.0f); // Direction to camera
	float relectionCameraDotProduct = max(dot(R,V),0.0f);
	float dampedFactor = pow(relectionCameraDotProduct,shininess);
	dampedFactor = max(dampedFactor,0.001f);
	float level = floor(dampedFactor * toonShadingLevels);
	dampedFactor = level/toonShadingLevels;
	vec4 result = specularColor * dampedFactor * lightColor; 
	
	return result;
}