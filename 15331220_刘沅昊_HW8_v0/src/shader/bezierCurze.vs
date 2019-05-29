#version 330 core
layout (location = 0) in float t;

uniform vec3 p0;
uniform vec3 p1;
uniform vec3 p2;
uniform vec3 p3;
uniform vec3 p4;
uniform int pointNum;

void main()
{
    vec3 qt;
    if (pointNum <= 1) {
        return;
    } else if (pointNum == 2) {
        qt = (1-t) * p0 + t * p1;
    } else if (pointNum == 3) {
        qt = pow((1-t), 2) * p0 + 2 * t * (1-t) * p1 + t * t * p2;
    } else if (pointNum == 4) {
        qt = pow((1-t), 3) * p0 + 3 * t * (1-t) * (1-t) * p1 + 3 * t * t * (1-t) * p2 + pow(t, 3) * p3;
    } else if (pointNum == 5) {
        qt = pow((1-t), 4) * p0 + 4 * t * pow((1-t), 3) * p1 + 6 * t * t * pow((1-t), 2) * p2 + +  4 * pow((t), 3) * (1 - t) * p3 + pow(t, 4) * p4;
    }
    gl_Position = vec4(qt.x, qt.y, qt.z, 1.0);
}