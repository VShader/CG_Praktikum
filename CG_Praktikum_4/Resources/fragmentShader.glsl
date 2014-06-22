#version 430 core

layout(location=0) out vec4 color;

in VS_OUT
{
    vec3 normal;
    vec3 light;
    vec3 view;
} fs_in;

//uniform Material
//{
uniform vec3 ambient;
uniform vec3 diffuse_albedo;
uniform vec3 specular_albedo;
uniform float specular_power;
uniform vec3 light_color;
//} material;

void main() {

   // vec3 diffuse_albedo = vec3(0.5, 0.2, 0.7);
   // vec3 specular_albedo = vec3(0.7, 0.7, 0.7);
   // float specular_power = 128.0;
   // vec3 light_color = vec3(0.7, 0.7, 0.7);

    vec3 reflection = reflect(-fs_in.light, fs_in.normal);

    vec3 diffuse = max(dot(fs_in.normal, fs_in.light), 0.0) * diffuse_albedo;
    vec3 specular = pow(max(dot(reflection, fs_in.view), 0.0), specular_power) * specular_albedo;
   color = vec4(ambient*diffuse_albedo + diffuse + specular, 1.0);

    //gl_FragColor = vec4(1.0f, 0.0f, 0.0f, 0.5f);
}
