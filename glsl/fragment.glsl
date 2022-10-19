#version 460

// Width and height of the shader
uniform vec2 u_resolution;  
// Camera pos in xyz
uniform vec3 u_camera_origin;
// Camera rotation as a quaternion
uniform vec4 u_camera_rotation_q;
out vec4 frag_color;
// maybe needs a layout (location = 0)  

#define MAX_STEPS 100
#define MAX_DIST 1000.
#define SURFACE_TRESH .01

float sphere_dist(vec3 point)
{
    float repetition = 5;
    float sphere_radius = 0.25;
    vec3 sphere_pos = vec3(repetition / 2);
    sphere_radius *= (repetition / 2);
    return distance(mod(point, repetition), sphere_pos) - sphere_radius;
}
 
vec3 ray_march(vec3 point, vec3 direction) 
{
    float origin_distance = 0;
    int ray_step = 0;
    for(; ray_step < MAX_STEPS; ray_step++)
    {
        float surface_distance = sphere_dist(point);
        origin_distance += surface_distance;
        point += direction * surface_distance;

        if (origin_distance > MAX_DIST ||
                surface_distance < SURFACE_TRESH)
            break;
    }
    
    if (ray_step == MAX_STEPS || origin_distance > MAX_DIST)
        return vec3(0., 0., 0.);


    return vec3(1. - (ray_step / float(MAX_STEPS)));

}

vec4 quat_mul(vec4 a, vec4 b){
    mat4 m;
    vec4 v = a.wxyz;
    m[0] = b.wxyz;
    m[1] = vec4(-b.x, b.w, b.z, -b.y);
    m[2] = vec4(-b.y, -b.z, b.w, b.x);
    m[3] = vec4(-b.z, b.y, -b.x, b.w);
    return vec4(m * v).yzwx;
}
 
void main()
{
    // maps screen coordinates from pixel indexes to [-1;1] range on the width,
    // the width resolution will always be in -1;1 , y might be smaller or go beyond
    // repending on the viewport ratio
    // we do an operation that will always result in 0.5 on the width offset,
    // but the math is cleaner this way so I'll keep it like that
    vec2 screen_coordinates =   ((gl_FragCoord.xy / u_resolution.x) 
                                - (u_resolution / (u_resolution.x * 2))) * 2;
    
    // Calculates in which direction the ray should go
    vec3 ray_direction_pre_camera = 
        normalize(vec3(screen_coordinates.x, screen_coordinates.y, 1.0));
    
    // Rotates the ray direction to take into account the camera angle
    // r = q * p * q^(-1)
    vec4 ray_direction_q = 
        quat_mul(
                quat_mul(   u_camera_rotation_q,
                            vec4(ray_direction_pre_camera, 0.)
                        ), 
                vec4(-u_camera_rotation_q.xyz, u_camera_rotation_q.w)
                );
    
    frag_color = vec4(ray_march(u_camera_origin, ray_direction_q.xyz), 1.0);
}
