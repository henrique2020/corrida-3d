#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D textureToAplicate;
uniform sampler2D texture2;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	//FragColor = mix(texture(textureToAplicate, TexCoord), texture(textureToAplicate, TexCoord), 0.5);
	FragColor = texture(textureToAplicate, TexCoord);
	//FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
