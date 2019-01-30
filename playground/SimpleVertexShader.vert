#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;

//mat4 myMatrix;
//vec4 myVector;

uniform mat4 MVP;

out vec3 fragmentColor;

void main(){

	//vec4 transformedVector = myMatrix * myVector;

	//gl_Position.xyz = vertexPosition_modelspace;
    //gl_Position.w = 1.0;
	//gl_Position = MVP * gl_Position;
	gl_Position = MVP * vec4(vertexPosition_modelspace, 1);
	fragmentColor = vertexColor;
}

