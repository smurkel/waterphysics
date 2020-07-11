#type vertex
#version 430

layout(location = 0) in vec2 index;
layout(binding = 0) uniform sampler2D _X;
layout(binding = 1) uniform sampler2D _Y;
layout(binding = 2) uniform sampler2D _Z;

uniform int L;

void main()
{
	vec2 X = (index-0.5) * L;
	float dx = texture(_X, index).r;
	float dy = texture(_Y, index).r;
	float dz = texture(_Z, index).r;
	gl_Position = vec4(X.x - dx, dy, X.y - dz, 1.0);
}

#type geometry
#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform vec3 u_LightPosition;
uniform vec3 u_ViewPosition;
uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;

out float Reflection;
out float Righton;
out float Fog;

void main()
{

	// calculate normal vector
	vec3 V1 = gl_in[0].gl_Position.xyz;
	vec3 V2 = gl_in[1].gl_Position.xyz;
	vec3 V3 = gl_in[2].gl_Position.xyz;
	vec3 Normal = normalize(cross(V2-V1, V3-V1));

	vec3 P = ((V1 + V2 + V3) / 3).xyz;
	Reflection = dot(normalize(reflect(P - u_LightPosition, Normal)),normalize(u_ViewPosition - P));
	Righton = dot(-Normal, normalize(P - u_ViewPosition));

	
	gl_Position = u_ViewMatrix * gl_in[0].gl_Position;	
	Fog = pow(length(V1)/40.0, 2);
	gl_Position = u_ProjectionMatrix * gl_Position;
	EmitVertex();

	gl_Position = u_ViewMatrix * gl_in[1].gl_Position;	
	Fog = pow(length(V2)/40.0, 2);
	gl_Position = u_ProjectionMatrix * gl_Position;
	EmitVertex();

	gl_Position = u_ViewMatrix * gl_in[2].gl_Position;	
	Fog = pow(length(V3)/40.0, 2);
	gl_Position = u_ProjectionMatrix * gl_Position;
	EmitVertex();

}

#type fragment
#version 330

layout(location = 0) out vec4 fragColor;

in float Reflection;
in float Righton;
in float Fog;

uniform vec4 C_Emissive;
uniform vec4 C_Ambient;
uniform vec4 C_Diffuse;
uniform vec4 C_Specular;

void main (void) 
{	
	float F_Emissive = 1.0;
	float F_Ambient = 1.0;
	float F_Diffuse = 1.0;
	float F_Specular = 1.0;
	bool Facing = Reflection > 0.0;
	bool Right = Righton > 0.0;

	fragColor = C_Ambient * F_Ambient +
				C_Diffuse * F_Diffuse * max(Reflection, 0) +
				(Facing ?
					C_Specular * F_Specular * pow(Reflection,20) :
					vec4(0.0, 0.0, 0.0, 0.0)) +
				(Right ?
					C_Emissive * F_Emissive * pow(Righton, 2) :
					vec4(0.0, 0.0, 0.0, 0.0));

	fragColor.a = 1;
//	if (Fog > 0.5)
//	{
//		float fog = Fog - 0.5;
//		fragColor = (1 - fog) * fragColor + fog * vec4(1,1,1,1);
//	}
}