#type compute
#version 430

layout(local_size_x = 9, local_size_y = 1) in;
layout(binding = 0, rgba32f) readonly uniform image2D _X;
layout(binding = 1, rgba32f) readonly uniform image2D _Y;
layout(binding = 2, rgba32f) readonly uniform image2D _Z;

layout(std430, binding = 3) buffer Nodes
{
	vec4 Floater[ ];
};


uniform mat4 u_Transform;
uniform float u_L;
uniform int u_N;

const float V = 20;
const float floater_height = 0.5;
const float g = 9.81;
const float water_density = 1;
const float floater_density = 0.8;
void main(void)
{
	
	uint idx = gl_LocalInvocationID.x;
	vec4 position = u_Transform * Floater[idx];
	vec4 origin = u_Transform * vec4(0, 0, 0, 1.0);
	ivec2 pixelCoords = ivec2(position.xz / u_L + 0.5) * u_N;

	float _xOffset = imageLoad(_X, pixelCoords).r;
	float _zOffset = imageLoad(_Z, pixelCoords).r;
	position.x += _xOffset;
	position.z += _zOffset;

	pixelCoords = ivec2((position.xz / u_L + 0.5) * u_N);
	

	// Floater position now known. Calculate force on floater.

	float floater_depth = imageLoad(_Y, pixelCoords).r - position.y;
	float force = (-g * V * floater_density) + (V * water_density * g * clamp(floater_depth, 0, floater_height) / floater_height);
	vec3	Force	=	vec3(0, force, 0);
	vec3	Arm		=	position.xyz - origin.xyz; // center of mass is at (0, 0, 0).
	vec3	Torque	=	cross(Arm, Force);
	Floater[idx].xyz = Torque;
	Floater[idx].w = Force.y;

}
