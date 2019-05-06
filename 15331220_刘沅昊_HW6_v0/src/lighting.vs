#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 lightingColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

uniform float ambientFactor;
uniform float diffuseFactor;
uniform float specularFactor;
uniform int shininessFactor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform int shading_choose;

out vec3 FragPos;
out vec3 Normal;

void main()
{
	if (shading_choose == 1) {
		gl_Position =  projection * view * model * vec4(aPos, 1.0f);
		FragPos = vec3(model * vec4(aPos, 1.0f));
		Normal = mat3(transpose(inverse(model))) * aNormal;
	}
	if (shading_choose == 2) {
		gl_Position =  projection * view * model * vec4(aPos, 1.0f);
		vec3 Position = vec3(model * vec4(aPos, 1.0));
		Normal = mat3(transpose(inverse(model))) * aNormal;

		float ambientStrength = 0.1;
		vec3 ambient = ambientStrength * lightColor;

		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(lightPos - Position);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = diff * lightColor;

		float specularStrength = 0.5;
		vec3 viewDir = normalize(viewPos - Position);
		vec3 reflectDir = reflect(-lightDir, norm);  
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininessFactor);
		vec3 specular = specularStrength * spec * lightColor; 

		lightingColor = ambient*ambientFactor + diffuse*diffuseFactor + specular*specularFactor;
	}
}