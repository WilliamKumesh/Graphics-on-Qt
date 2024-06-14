#version 330 core
in vec3 fragment_normal;
in vec3 fragment_world_pos;
in vec2 fragment_tex_coord;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Dir_light {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 view_pos;
uniform sampler2D tex;
uniform sampler2D normal_map;

vec3 calc_dir_light(Dir_light light, Material material, vec3 normal, vec3 view_dir)
{
    vec3 light_dir = normalize(-light.direction);
    float diff = max(dot(normal, light_dir), 0.0);
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;

    return (ambient + diffuse + specular);
}

void main() {
    Dir_light light;
        light.direction = vec3(0, -5, 0);
        light.ambient = vec3(0.3, 0.5, 0.3);
	light.diffuse = vec3(0.8, 0.8, 0.8);
	light.specular = vec3(0.5, 0.5, 0.5);

	Material material;
        material.ambient = vec3(0.3, 0.3, 0.3);
	material.diffuse = vec3(0.8, 0.8, 0.8);
        material.specular = vec3(0.6, 0.6, 0.6);
        material.shininess = 60.0f;

    vec3 normal = normalize(fragment_normal);
    vec3 view_dir = normalize(view_pos - fragment_world_pos);

    vec3 tex_color = texture(tex, fragment_tex_coord).rgb;
    material.diffuse *= tex_color;

    vec3 result = calc_dir_light(light, material, normal, view_dir);

    gl_FragColor = vec4(result, 1.0);
}



