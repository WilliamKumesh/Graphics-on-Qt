#version 330 core
layout (location = 0) in vec3 position_attr;
layout (location = 1) in vec3 normal_attr;
layout (location = 2) in vec2 tex_coord_attr;


out vec3 fragment_normal;
out vec3 fragment_world_pos;
//
out vec2 fragment_tex_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    fragment_normal = mat3(transpose(inverse(model))) * normal_attr;
    fragment_world_pos = vec3(model * vec4(position_attr, 1.0));

    fragment_tex_coord = tex_coord_attr;

    gl_Position = projection * view * model * vec4(position_attr, 1.0);
}
