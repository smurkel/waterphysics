#type vertex
#version 330

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 UV;

uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat4 u_Transform;

uniform vec3 u_LightPosition;
uniform vec3 u_ViewPosition;
out vec2 textureCoordinates;
out float illumination;
out float reflection;
//out float Brightness;
void main() 
{
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_Transform * vec4(Position, 1.0);
	textureCoordinates = UV;
	// Brightness
	illumination = dot(Normal, normalize(u_LightPosition - Position));
	// Diffuse lightning
	reflection = dot(normalize(reflect(Position - u_LightPosition, Normal)), normalize(u_ViewPosition - Position));
	//Brightness = 1 / pow(length(u_LightPosition - Position),2);
}

#type fragment
#version 330

layout(location = 0) out vec4 fragColor;

in vec2 textureCoordinates;
in float illumination;
in float reflection;
in float Brightness;
uniform sampler2D u_Texture; 

void main (void) 
{	
	float Luminance = 0.2 + 0.5 * max(illumination, 0) + 0.5 * max(reflection, 0);
	
	fragColor = texture(u_Texture, textureCoordinates) * Luminance;
	fragColor.a = 1;
}