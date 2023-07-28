#version 330 core 

out vec4 FragColor; //pixel color

uniform sampler2D tex0;
uniform sampler2D norm_tex;
uniform sampler2D sec_tex;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform vec3 ambientColor;
uniform float ambientStr;

uniform vec3 cameraPos;
uniform float specStr;
uniform float specPhong;

in vec2 texCoord;
in vec3 normCoord;
in vec3 fragPos;
in mat3 TBN;

void main()
{
	vec3 normal = texture(norm_tex, texCoord).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	normal = normalize(TBN * normal);

	vec3 lightDir = normalize(lightPos - fragPos);
	
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 ambientCol = ambientColor * ambientStr;

	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);

	float spec = pow(max(dot(reflectDir, viewDir), 0.1), specPhong);
	vec3 specColor = spec * specStr * lightColor;

	float distance = distance(lightPos, fragPos);
	float intensity = 100 * (1 / (distance * distance));

	vec4 pixelColor = texture(tex0, texCoord);

	if(pixelColor.a < 0.1)
	{
		/* if alpha low enough, use sec_tex data instead */
		pixelColor = texture(sec_tex, texCoord);
		/* set alpha to 0.5 aswell */
		pixelColor.a = 0.5;
	}

	/* intensity adjustment so object doesnt disappear if intensity is too low */
	/* Added specColor multiplier to make it shinier */
	vec3 finalColor = ((specColor * 5) + diffuse + ambientCol) * intensity;

	FragColor = vec4(finalColor, 1.0) * pixelColor;
}