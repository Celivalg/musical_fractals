#version 460

// Width and height of the shader
uniform vec2 u_resolution;  
// Camera pos in xyz
uniform vec3 u_camera_origin;
// Camera rotation as a quaternion
uniform vec4 u_camera_rotation_q;
// steps 
uniform uint u_max_steps;
uniform float u_max_dist;
uniform float u_surface_tresh;

#define PI 3.1415926538


out vec4 frag_color;
// maybe needs a layout (location = 0)  

// rotation tools
vec4 quat_mul(vec4 a, vec4 b){
    mat4 m;
    vec4 v = a.wxyz;
    m[0] = b.wxyz;
    m[1] = vec4(-b.x, b.w, -b.z, b.y);
    m[2] = vec4(-b.y, b.z, b.w, -b.x);
    m[3] = vec4(-b.z, -b.y, b.x, b.w);
    //m[1] = vec4(-b.x, b.w, b.z, -b.y);
    //m[2] = vec4(-b.y, -b.z, b.w, b.x);
    //m[3] = vec4(-b.z, b.y, -b.x, b.w);
    return vec4(m * v).yzwx;
}

// r = q * p * q^(-1)
vec4 quat_rot(vec4 p, vec4 rotation){
    return quat_mul(quat_mul(rotation, p), vec4(-rotation.xyz, rotation.w));
}

vec3 point_rot(vec3 p, vec3 axis, float rads){
    float s = sin(rads/2);
    float c = cos(rads/2);
    vec4 q = vec4(axis.x * s, axis.y * s, axis.y * s, c);
    return quat_rot(vec4(p, 0.0), q).xyz;
}



float triangle_dist(vec3 z)
{
    vec3 a1 = vec3(1,1,1);
    vec3 a2 = vec3(-1,-1,1);
    vec3 a3 = vec3(1,-1,-1);
    vec3 a4 = vec3(-1,1,-1);
    vec3 c;
    float scale = 2.0;
    int n = 0;
    float dist, d;
    while (n < 10) {
        c = a1; dist = length(z-a1);

        d = length(z-a2); 
        if (d < dist) { c = a2; dist=d; }

        d = length(z-a3);
        if (d < dist) { c = a3; dist=d; }

        d = length(z-a4); 
        if (d < dist) { c = a4; dist=d; }

        z = scale*z-c*(scale-1.0);
        n++;
    }

    return length(z) * pow(scale, float(-n));
}

float triangle_dist_2(vec3 z)
{
    float r;
    int n = 0;
    float scale = 2.0;
    float offset = 1.0;

    while (n < 20) {
        if(z.x+z.y<0) z.xy = -z.yx; // fold 1
        if(z.x+z.z<0) z.xz = -z.zx; // fold 2
        if(z.y+z.z<0) z.zy = -z.yz; // fold 3
        z = z*scale - offset*(scale-1.0);
        n++;
    }
    return (length(z) ) * pow(scale, -float(n));
}

//scale=2
//bailout=1000
float cube_1(vec3 p){
    p /= 2.0;
    float scale = 2.0;
    vec3 C = normalize(vec3(0.5, .2, .5));
    int i;

    float r = p.x * p.x + p.y * p.y + p.z * p.z;
    for(i = 0; i < 20 && r < 1000; i++){
        //point_rot(p, normalize(vec3(0., 2., 4.)), PI/2);

        p = abs(p);
        if (p.x - p.y < 0){
            p = p.yxz;
        }
        if (p.x - p.z < 0){
            p = p.zyx;
        }
        if (p.y - p.z < 0){
            p = p.xzy;
        }

        //point_rot(p, normalize(vec3(0.0, 1., 0.)), PI/2);

        p = scale * p - C * (scale - 1);
        r= p.x * p.x + p.y * p.y + p.z * p.z;
    }
    return (sqrt(r) - 2) * pow(scale, -i);//the estimated distance
}


// credit: knighty on https://www.fractalforums.com/sierpinski-gasket/kaleidoscopic-(escape-time-ifs)/
// not mine, honestly It's going to take me a while to understand what's hapenning
float sierpinski3(vec3 p){
    float scale = 2.0;
    float CX = 1;
    float CY = 1;
    float CZ = 1;
    float x1 = 0;
    float y1 = 0;
    float z1 = 0;
    int i;

    float r = p.x * p.x + p.y * p.y + p.z * p.z;
    for(i = 0; i < 10 && r < 1000; i++){
        point_rot(p, vec3(1., 0., 0.), PI/2);

        if (p.x + p.y < 0){
            x1 = -p.y;
            p.y = -p.x;
            p.x = x1;
        }
        if (p.x + p.z < 0){
            x1 = -p.z;
            p.z = -p.x;
            p.x = x1;
        }
        if (p.y + p.z < 0){
            y1 = -p.z;
            p.z = -p.y;
            p.y = y1;
        }

        point_rot(p, vec3(0.5, 0.5, 0.), PI);

        p.x = scale * p.x - CX * (scale - 1);
        p.y = scale * p.y - CY * (scale - 1);
        p.z = scale * p.z - CZ * (scale - 1);
        r= p.x * p.x + p.y * p.y + p.z * p.z;
    }
    return (sqrt(r) - 2) * pow(scale, -i);//the estimated distance
}


float sphere_dist(vec3 point)
{
    float repetition = 1;
    float sphere_radius = 0.25;
    vec3 sphere_pos = vec3(repetition / 2);
    //sphere_radius *= (repetition / 2);
    return distance(mod(point, repetition), sphere_pos) - sphere_radius;
}

float dist(vec3 point){
    return cube_1(point);
}

vec3 ray_march(vec3 point, vec3 direction) 
{
    float origin_distance = 0;
    int ray_step = 0;
    for(; ray_step < u_max_steps; ray_step++)
    {
        float surface_distance = dist(point);
        origin_distance += surface_distance;
        point += direction * surface_distance;

        if (origin_distance > u_max_dist ||
                surface_distance < u_surface_tresh)
            break;
    }

    if (ray_step == u_max_steps || origin_distance > u_max_dist)
        return vec3(0., 0., 0.);


    return vec3(1. - (ray_step / float(u_max_steps)), 1. - (ray_step / float(u_max_steps)), 1. - (ray_step / float(u_max_steps)));

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
    vec4 ray_direction_q = quat_rot(vec4(ray_direction_pre_camera, 0.), u_camera_rotation_q);

    frag_color = vec4(ray_march(u_camera_origin, ray_direction_q.xyz), 1.0);
}
