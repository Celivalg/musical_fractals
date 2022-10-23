#include "camera.h"
#include <math.h>
#include <stdio.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

// This is getting quite ugly

void mouse_to_angle(struct context *context, float x, float y) {
    float revolution = 4000.0f;

    float new_pitch = (y * context->camera->camera_sensitivity) / revolution +
                      context->camera->camera_rotation_pitch;
    float new_yaw = (x * context->camera->camera_sensitivity) / revolution +
                    context->camera->camera_rotation_yaw;

    if (new_pitch > 0.5f)
        new_pitch = 0.5f;
    if (new_pitch < -0.5f)
        new_pitch = -0.5f;
    // TODO do something about the yaw being left to climb to infinity,
    // the cos/sin or the remained isn't precise enough to not cause a jump in
    // angle (in float at least, tbh I dunno why I am still using floats and not
    // doubles here, it's cpu land, it should be fine to use doubles), although
    // to be fair, you would have to spin quite long to cause the angle too get
    // high enough to get precision issues
    //
    // new_yaw = (float)remainder(new_yaw, 2.0f);

    context->camera->camera_rotation_pitch = new_pitch;
    context->camera->camera_rotation_yaw = new_yaw;

    // printf("yaw:%f pitch:%f\n", new_yaw, new_pitch);
}

void key_forward(struct context *context, enum direction dir, bool pressed) {
    calc_camera(context);

    if (pressed != context->camera->pressed[dir]) {
        calc_camera(context);
        context->camera->pressed[dir] = pressed;
    }
}

float to_rads(float angle) { return M_PI * angle; }

void quat_mul(float *a, float *b, float *c) {
    c[0] = (a[3] * b[0]) + (a[0] * b[3]) + (a[1] * b[2]) - (a[2] * b[1]);
    c[1] = (a[3] * b[1]) - (a[0] * b[2]) + (a[1] * b[3]) + (a[2] * b[0]);
    c[2] = (a[3] * b[2]) + (a[0] * b[1]) - (a[1] * b[0]) + (a[2] * b[3]);
    c[3] = (a[3] * b[3]) - (a[0] * b[0]) - (a[1] * b[1]) - (a[2] * b[2]);
    /*
     * bad math?
        c[0] = (a[3] * b[0]) + (a[0] * b[3]) - (a[1] * b[2]) + (a[2] * b[1]);
        c[1] = (a[3] * b[1]) + (a[0] * b[2]) + (a[1] * b[3]) - (a[2] * b[0]);
        c[2] = (a[3] * b[2]) - (a[0] * b[1]) + (a[1] * b[0]) + (a[2] * b[3]);
        c[3] = (a[3] * b[3]) - (a[0] * b[0]) - (a[1] * b[1]) - (a[2] * b[2]);
    */
}

void quat_rot(float *quat_a, float *quat_p) {
    float quat_ai[4] = {-quat_a[0], -quat_a[1], -quat_a[2], quat_a[3]};
    float temp[4] = {0.0, 0.0, 0.0, 0.0};
    quat_mul(quat_a, quat_p, temp);
    quat_mul(temp, quat_ai, quat_p);
}

// orients the calculated velocity towards camera direction
void orient_vel(struct camera_data *camera, float *velocity) {
    float quat_vel[4] = {velocity[0], velocity[1], velocity[2], 0.0};
    quat_rot((camera->camera_rotation_q), quat_vel);
    velocity[0] = quat_vel[0];
    velocity[1] = quat_vel[1];
    velocity[2] = quat_vel[2];
}

void quat_norm(float *q) {

    // not sure that's actually needed now, but it's an extra safety to make
    // sure I'm working with normalized vectors
    float n = sqrtf(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
    q[0] /= n;
    q[1] /= n;
    q[2] /= n;
    q[3] /= n;
}

void calc_camera_rotation(struct camera_data *camera) {
    float pitch = to_rads(camera->camera_rotation_pitch);
    float yaw = to_rads(camera->camera_rotation_yaw);

    // avoiding to repeat those operations, though they probably would have been
    // cached
    float sp2 = sinf(pitch / 2.0f);
    float cp2 = cosf(pitch / 2.0f);
    float sy2 = sinf(yaw / 2.0f);
    float cy2 = cosf(yaw / 2.0f);

    // axis for pitch is rotated 90° from yaw angle
    // also it took me way too long to realise that I was calculating my axis
    // with the sy2 and cy2 values...
    float sy = sinf(yaw + (M_PI / 2.0f));
    float cy = cosf(yaw + (M_PI / 2.0f));

    float quat_pitch[4] = {sy * sp2, 0.0f, -cy * sp2, -cp2};
    float quat_yaw[4] = {0.0f, -1.0f * sy2, 0.0f, cy2};
    quat_norm(quat_pitch);
    quat_norm(quat_yaw);
    quat_mul(quat_pitch, quat_yaw, camera->camera_rotation_q);
    /*
     * this math doesn't work, because i'm an idiot
        camera->camera_rotation_q[0] = sp * sy;
        camera->camera_rotation_q[1] = cp * sy;
        camera->camera_rotation_q[2] = sp * cy;
        camera->camera_rotation_q[3] = cp * cy;
     * neither does this one
        camera->camera_rotation_q[0] = -sp * sy;
        camera->camera_rotation_q[1] = sp * cy;
        camera->camera_rotation_q[2] = cp * sy;
        camera->camera_rotation_q[3] = cp * cy;
    */
}

float get_interval(struct camera_data *camera) {

    struct timeval time_now;
    gettimeofday(&time_now, NULL);
    // yes I hate type conversions in C, only gripe I have with the language tbh
    // basically this will give us a float with s.mmm with s as the seconds
    // and m as milisecond (s is not limited to only one significant digit, but
    // it probably will stay 0 most of the time if the code works normally
    float interval =
        ((float)(((time_now.tv_usec - camera->last_update.tv_usec) / 1000) +
                 ((time_now.tv_sec - camera->last_update.tv_sec) * 1000))) /
        1000.0f;
    camera->last_update = time_now;
    return interval;
}

void calc_camera_velocity(struct camera_data *camera, float interval) {

    float vel_comp[3];
    vel_comp[2] = camera->camera_accel * interval *
                  (((int)camera->pressed[d_forward]) -
                   ((int)camera->pressed[d_backward]));
    vel_comp[1] =
        camera->camera_accel * interval *
        (((int)camera->pressed[d_up]) - ((int)camera->pressed[d_down]));
    vel_comp[0] =
        camera->camera_accel * interval *
        (((int)camera->pressed[d_right]) - ((int)camera->pressed[d_left]));
    orient_vel(camera, vel_comp);

    camera->camera_vel[0] +=
        vel_comp[0] - camera->camera_vel[0] * camera->camera_decel * interval;
    camera->camera_vel[1] +=
        vel_comp[1] - camera->camera_vel[1] * camera->camera_decel * interval;
    camera->camera_vel[2] +=
        vel_comp[2] - camera->camera_vel[2] * camera->camera_decel * interval;

    float l = sqrtf(camera->camera_vel[0] * camera->camera_vel[0] +
                    camera->camera_vel[1] * camera->camera_vel[1] +
                    camera->camera_vel[2] * camera->camera_vel[2]);
    if (l > camera->camera_speed) {
        camera->camera_vel[0] *= camera->camera_speed / l;
        camera->camera_vel[1] *= camera->camera_speed / l;
        camera->camera_vel[2] *= camera->camera_speed / l;
    }
}

void calc_camera_position(struct camera_data *camera, float interval) {
    camera->camera_origin[0] += camera->camera_vel[0] * interval;
    camera->camera_origin[1] += camera->camera_vel[1] * interval;
    camera->camera_origin[2] += camera->camera_vel[2] * interval;
}

void calc_camera(struct context *context) {
    float interval = get_interval(context->camera);
    calc_camera_rotation(context->camera);
    calc_camera_velocity(context->camera, interval);
    calc_camera_position(context->camera, interval);
}

