#version 130

in vec4 color;
in vec3 normal;
in vec2 texCoord;

uniform vec3 lightDir = vec3(1, 1, 0);
uniform vec3 uColor = vec3(0, 0, 0);

uniform sampler2D texture;

void main()
{
	float ambient = 0.3f;
	float diffuze = 0.7f;

	diffuze *= max(dot(normal, lightDir), 0.0f);
	diffuze = 1;

	vec4 fragColor = (color + vec4(uColor, 1.0f)) * (diffuze + ambient);
	fragColor.a = 1.0f;

	gl_FragColor = fragColor;
}