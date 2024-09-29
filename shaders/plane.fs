#version 330 core

out vec4 _color;

uniform vec2 resolution;
uniform float scale;

vec2 p = (gl_FragCoord.xy - 0.5 * resolution.xy) / resolution.y * scale * 2.0;

const float thickness = 0.003; 

float drawCircle(vec2 o, float r) {
    return 1.0 - smoothstep(0, thickness, clamp(abs(distance(o, p) - r), 0, thickness));
}

float drawLine(vec2 a, vec2 b) {
    vec2 ab = b - a, ap = p - a;
    
    float projfrac = clamp(dot(ab, ap) / dot(ab, ab), 0.0, 1.0);
    float d = distance(ap, ab * projfrac);
    
    return 1.0 - smoothstep(0.0, thickness, d); 
}

float color = 0.0;
vec2 drawEpicycle(vec2 o, float r, float phi) {
    vec2 _p = vec2(cos(phi) * r, sin(phi) * r);
    color += drawCircle(o, r);
    color += drawLine(o, o + _p);
    return _p; 
}

const int maxn = 1024;
uniform float r[maxn], phi[maxn];
uniform int n; 

void main() {
    vec2 o = vec2(0.0);
    for (int i = 0; i < n; ++i) {
        o += drawEpicycle(o, r[i], phi[i]);
    }
    _color = vec4(vec3(color), 1.0);
}