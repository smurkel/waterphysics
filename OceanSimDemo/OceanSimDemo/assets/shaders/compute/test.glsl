#type vertex
#version 430

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_Texture;

uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

out vec2 UV;

void main()
{
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(a_Position, 1.0);
	UV = a_Texture;
}

#type fragment
#version 430

layout(location = 0) out vec4 fragColor;
layout(binding = 0) uniform sampler2D tilde_h0k;

in vec2 UV;

void main (void)
{
	fragColor = texture(tilde_h0k, UV);
}
