//#version 430 core

in vec4 position;
in vec3 normal;

uniform mat4 mv_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;

uniform vec4 light_pos;

out VS_OUT
{
    vec3 normal;
    vec3 light;
    vec3 view;
} vs_out;

void main() {

    //mat4
    // View-Space position
    vec4  P = mv_matrix * view_matrix * position;
    // View-Space normal
    vs_out.normal = normalize(mat3(mv_matrix * view_matrix) * normal);
    // Light Vector
    vs_out.light = normalize(vec3(light_pos) - P.xyz);
    // View Vector
    vs_out.view = normalize(-P.xyz);


   gl_Position = proj_matrix * view_matrix * mv_matrix * position;
//   gl_Position = mv_matrix * position;
}
