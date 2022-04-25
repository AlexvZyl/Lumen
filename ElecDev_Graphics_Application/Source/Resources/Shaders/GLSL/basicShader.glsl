#shader vertex
#version 450 core

layout(location = 0) in vec3 v_pos;
layout(location = 1) in vec4 v_color;
layout(location = 2) in float v_outline;
layout(location = 3) in uint v_entityID;

uniform mat4 viewProjMatrix;

out vec4 f_color;
flat out uint f_entityID;

void main()
{
	f_color = v_color;
    gl_Position = viewProjMatrix * vec4(v_pos, 1.0);
	f_entityID = v_entityID;
}

#shader fragment
#version 450 core

layout(location = 0) out vec4 o_color;
layout(location = 1) out uint o_entityID;

in vec4 f_color;
flat in uint f_entityID;

void main()
{
	o_color = f_color;
	o_entityID = f_entityID;
};