#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in float light;

float v;

// texture samplers
uniform sampler2D texture1;
uniform vec2 texSize;

void main()
{
	//FragColor = texture(texture1, vec2(TexCoord.x / texSize.x, (1.0 - (TexCoord.y + 1)) / texSize.y) * light);
	FragColor = texture(texture1, vec2(TexCoord.x / texSize.x, (1.0 - (TexCoord.y + 1)) / texSize.y)) * light;
}