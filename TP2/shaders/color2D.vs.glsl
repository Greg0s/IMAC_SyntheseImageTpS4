#version 330 core

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec3 aVertexColor;

out vec3 vFragColor;

mat3 translate(float tx, float ty){
    return mat3(vec3(1.,0.,0.), vec3(0.,1.,0.), vec3(tx,ty,1.));
}

mat3 scale(float sx, float sy){
    return mat3(vec3(sx,0.,0.), vec3(0.,sy,0.), vec3(0.,0.,1.));
}

mat3 rotate(float a){
    return mat3(vec3(cos(a),sin(a),0.), vec3(-sin(a),cos(a),0.), vec3(0.,0.,1.));
}

void main() {
    float att = radians(15) * exp(-radians(30) * 1);
    vFragColor = aVertexColor * att;
    //mat3 M = mat3(vec3(aVertexPosition.x,0,0), vec3(0,aVertexPosition.y,0), vec3(0,0,1));

    gl_Position = vec4((translate(0.5,0.5) * scale(0.5, 1.) * rotate(radians(45)) * vec3(aVertexPosition,1.)).xy, 0, 1);
}