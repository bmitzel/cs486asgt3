# version 120
/*
 * Michael Shafae
 * mshafae at fullerton.edu
 * 
 * A simple Blinn-Phong shader with two light sources.
 * This file is the fragment shader which calculates
 * the halfway vector between the viewer and light source
 * vector and then calculates the color on the surface
 * given the material properties passed in from the CPU program.
 *
 * For more information see:
 *     <http://en.wikipedia.org/wiki/Blinn–Phong_shading_model>
 *
 * $Id: blinn_phong.frag.glsl 4891 2014-04-05 08:36:23Z mshafae $
 *
 * Be aware that for this course, we are limiting ourselves to
 * GLSL v.1.2. This is not at all the contemporary shading
 * programming environment, but it offers the greatest degree
 * of compatability.
 *
 * Please do not use syntax from GLSL > 1.2 for any homework
 * submission.
 *
 */

// These are passed from the vertex shader to here, the fragment shader
// In later versions of GLSL these are 'in' variables.
varying vec3 myNormal;
varying vec4 myVertex;

// These are passed in from the CPU program, camera_control_*.cpp
uniform vec4 light0_position;
uniform vec4 light0_color;
uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform float shininess;

vec4 ComputeLight(const in vec3 direction, const in vec4 lightcolor, const in vec3 normal, const in vec3 halfvec, const in vec4 mydiffuse, const in vec4 myspecular, const in float myshininess) {
    float nDotL = dot(normal, direction);    
    vec4 lambert = mydiffuse * lightcolor * max(nDotL, 0.0);

    float nDotH = dot(normal, halfvec);
    vec4 phong = myspecular * lightcolor * pow(max(nDotH, 0.0), myshininess);    

    vec4 retval = lambert + phong;
    return retval;
}       

void main(void) {
    // The eye is always at (0,0,0) looking down -z axis 
    // Also compute current fragment position and direction to eye 
    const vec3 eyepos = vec3(0,0,0);
    vec4 _mypos = gl_ModelViewMatrix * myVertex;
    vec3 mypos = _mypos.xyz / _mypos.w;
    vec3 eyedirn = normalize(eyepos - mypos);

    // Compute normal, needed for shading. 
    vec3 normal = normalize(gl_NormalMatrix * myNormal);

    // Light 0, point
    vec3 position0 = light0_position.xyz / light0_position.w;
    vec3 direction0 = normalize(position0 - mypos);
    vec3 half0 = normalize(direction0 + eyedirn); 
    vec4 color0 = ComputeLight(direction0, light0_color, normal, half0, diffuse, specular, shininess);

    gl_FragColor = ambient + color0;
}
