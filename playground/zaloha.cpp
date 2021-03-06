#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h> // The OpenGL Extension Wrangler Library

#include <GLFW/glfw3.h> // A library for OpenGL, window and input
GLFWwindow* window;

#include <glm/glm.hpp> // 3D mathematics
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
//using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 840, 460, "Playground", NULL, NULL); // glfwGetPrimaryMonitor()
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	printf("%s\n", glGetString(GL_VERSION));

	GLuint programID = LoadShaders("SimpleVertexShader.vert", "SimpleFragmentShader.frag");

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	//glm::mat4 myMatrix = glm::translate(glm::mat4(), glm::vec3(10.0f, 0.0f, 0.0f));
	//glm::vec4 myVector(10.0f, 10.0f, 10.0f, 0.0f);
	//glm::vec4 transformedVector = myMatrix * myVector;
	//glm::mat4 myIdentityMatrix = glm::mat4(1.0f);
	//glm::mat4 myScaleMatrix = glm::scale(glm::mat4(), glm::vec3(2.0f, 2.0f, 2.0f));
	//glm::rotate(glm::radians(45.0), myRotationAxis);
	//glm::mat4 rotationMatrix = glm::mat4(1.0f);
	//glm::mat4 myRotationMatrix = glm::rotate(rotationMatrix, glm::radians(45.0), glm::vec3(1.0f, 1.0f, 1.0f));
	//glm::mat4 ViewMatrix = glm::translate(glm::mat4(), glm::vec3(-0.5f, 0.0f, 0.0f));
	

	glm::vec3 myRotationAxis(0.0f, -1.0f, 0.0f);

	glm::mat4 myScalingMatrix = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 modelRotationMatrix = glm::rotate(glm::radians(45.0f), myRotationAxis);
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 trScale = glm::scale(glm::vec3(1.5f, 1.5f, 1.0f));
	glm::mat4 trRotate = glm::rotate(glm::radians(45.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	glm::mat4 trTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.6f));
	
	glm::vec3 cameraPosition(0.0f, 3.0f, -10.0f); // 4.0f
	glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);
	glm::vec3 upVector(0.0f, 1.0f, 0.0f);
	
	glm::mat4 cameraMatrix = glm::lookAt(cameraPosition, cameraTarget, upVector);
	
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(49.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	//glm::mat4 projectionMatrix = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.0f, 100.0f);
	
	glm::mat4 MVP = projectionMatrix * cameraMatrix * modelMatrix * modelRotationMatrix * myScalingMatrix;
	glm::mat4 trMVP = projectionMatrix * cameraMatrix * trTranslate * trRotate * trScale;
	//glm::mat4 trMVP = projectionMatrix * cameraMatrix * modelMatrix * modelRotationMatrix * myScalingMatrix;
	
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int width, height;
	unsigned int imageSize;
	unsigned char * data;

	FILE * file = fopen("dilek_000.bmp", "rb");
	if (!file) {
		printf("Image could not be opened\n");
		return 0;
	}
	else {
		printf("Otevrel jsem obrazek\n");
	}
	if (fread(header, 1, 54, file) != 54) {
		printf("Not a correct BMP file\n");
		return false;
	}
	else {
		printf("Soubor je BMP\n");
	}
	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not correct BMP file\n");
	}
	else {
		printf("Jde o spravny format obrazku BMP\n");
	}
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	if (imageSize == 0) imageSize = width * height * 3;
	if (dataPos == 0) dataPos = 54;

	data = new unsigned char[imageSize];
	fread(data, 1, imageSize, file);
	fclose(file);

	printf("dataPos: %i\n", dataPos);
	printf("imageSize: %i\n", imageSize);
	printf("width: %i\n", width);
	printf("height: %i\n", height);
	printf("data: %p\n", data);

	static const GLfloat g_vertex_buffer_data[] = {
//		-1.0f, -1.0f, 1.1f,
//		0.0f, 1.5f, 1.1f,
//		1.0f, -1.0f, 1.1f,
		-1.0f,-1.0f,-1.0f, // triangle 1 : begin
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, // triangle 1 : end
	1.0f, 1.0f,-1.0f, // triangle 2 : begin
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f, // triangle 2 : end
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	//-1.0f, -1.0f, 0.0f, // tr
	//0.0f, 1.0f, 0.0f,
	//1.0f, -1.0f, 0.0f,
	};
	/*
	static GLfloat zaloha_g_color_buffer_data[12 * 3 * 3];
	for (int v = 0; v < 12 * 3; v++) {
		g_color_buffer_data[3 * v + 0] = 0.15f;
		g_color_buffer_data[3 * v + 1] = 0.15f;
		g_color_buffer_data[3 * v + 2] = 0.15f;
	}
	*/
	static const GLfloat g_color_buffer_data[] = {
	0.583f,  0.771f,  0.014f,
	0.609f,  0.115f,  0.436f,
	0.327f,  0.483f,  0.844f,
	0.822f,  0.569f,  0.201f,
	0.435f,  0.602f,  0.223f,
	0.310f,  0.747f,  0.185f,
	0.597f,  0.770f,  0.761f,
	0.559f,  0.436f,  0.730f,
	0.359f,  0.583f,  0.152f,
	0.483f,  0.596f,  0.789f,
	0.559f,  0.861f,  0.639f,
	0.195f,  0.548f,  0.859f,
	0.014f,  0.184f,  0.576f,
	0.771f,  0.328f,  0.970f,
	0.406f,  0.615f,  0.116f,
	0.676f,  0.977f,  0.133f,
	0.971f,  0.572f,  0.833f,
	0.140f,  0.616f,  0.489f,
	0.997f,  0.513f,  0.064f,
	0.945f,  0.719f,  0.592f,
	0.543f,  0.021f,  0.978f,
	0.279f,  0.317f,  0.505f,
	0.167f,  0.620f,  0.077f,
	0.347f,  0.857f,  0.137f,
	0.055f,  0.953f,  0.042f,
	0.714f,  0.505f,  0.345f,
	0.783f,  0.290f,  0.734f,
	0.722f,  0.645f,  0.174f,
	0.302f,  0.455f,  0.848f,
	0.225f,  0.587f,  0.040f,
	0.517f,  0.713f,  0.338f,
	0.053f,  0.959f,  0.120f,
	0.393f,  0.621f,  0.362f,
	0.673f,  0.211f,  0.457f,
	0.820f,  0.883f,  0.371f,
	0.982f,  0.099f,  0.879f,
	//1.0f, 0.0f, 0.0f, // tr
	//0.0f, 1.0f, 0.0f,
	//0.0f, 0.0f, 1.0f,
	};

	static const GLfloat g_uv_buffer_data[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.5f, 0.5f
	};

	static const GLfloat t_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
	};

	static const GLfloat t_color_buffer_data[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
	};



	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	GLuint trBuffer;
	glGenBuffers(1, &trBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, trBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t_vertex_buffer_data), t_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint trColorbuffer;
	glGenBuffers(1, &trColorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, trColorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t_color_buffer_data), t_color_buffer_data, GL_STATIC_DRAW);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//GLuint Texture = loadBMP_custom("dilek_000.bmp");



	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark gray background
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

	do{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw nothing, see you in tutorial 2 !
		glUseProgram(programID);

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		glEnableVertexAttribArray(1);
		/*for (int i = 0; i < 12 * 3; ++i) {
			g_color_buffer_data[i] = 1.0f;
		}*/
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
		glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		glDrawArrays(GL_TRIANGLES, 0, 12*3);

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &trMVP[0][0]);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, trBuffer);
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, trColorbuffer);
		glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);


		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

