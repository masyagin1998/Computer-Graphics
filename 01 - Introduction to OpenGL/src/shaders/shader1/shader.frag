#version 450 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D texture_asuka;
uniform sampler2D texture_rey;

uniform float mix_value;

void main() {
	// Linearly interpolate between both textures (second texture is only slightly combined)
	color = mix(texture(texture_asuka, TexCoord), texture(texture_rey, TexCoord), mix_value);
}
