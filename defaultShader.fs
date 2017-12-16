#version 130

in vec4 color;
in vec3 varNormal;

uniform vec3 lightDir = vec3(1, 1, 0);

void main()
{
	float ambient = 0.3f;
	float diffuze = 0.7f;

	diffuze *= max(dot(varNormal, lightDir), 0.0f);

	vec4 fragColor = color * (diffuze + ambient);
	fragColor.a = 1.0f;

    gl_FragColor = fragColor; 
}