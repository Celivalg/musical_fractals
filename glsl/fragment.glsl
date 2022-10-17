#version 460

uniform vec2 u_resolution;  // Width and height of the shader
out vec4 frag_color;
// maybe needs a layout (location = 0)  

#define MAX_STEPS 10000
#define MAX_DIST 1000.
#define SURFACE_TRESH .01

float sphere_dist(vec3 point)
{
    float repetition = 5;
    float sphere_radius = 1;
    vec3 sphere_pos = vec3(repetition / 2);
    sphere_radius *= (repetition / 2);
    return distance(mod(point, repetition), sphere_pos) - sphere_radius;
}
 
float ray_march(vec3 point, vec3 direction) 
{
    float origin_distance = 0;
    for(int ray_step = 0; ray_step < MAX_STEPS; ray_step++)
    {
        float surface_distance = sphere_dist(point);
        origin_distance += surface_distance;
        point += direction * surface_distance;

        if (origin_distance > MAX_DIST ||
                surface_distance < SURFACE_TRESH)
            break;
    }
    return origin_distance;
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
    vec3 origin = vec3(0, 0, 0); // aka Camera position
    vec3 ray_direction = normalize(vec3(screen_coordinates.x, screen_coordinates.y, 1.0));

    float dist = ray_march(origin, ray_direction);
    if (dist > MAX_DIST) {
            frag_color = vec4(0, 0, 0, 1.0);
            return;
    }

    vec3 color = vec3(dist / 100.);
    frag_color = vec4(color.x, color.y, color.z, 1.0);
}
