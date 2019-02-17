// OpenGL Trenning.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "pch.h"





//#define CONST_OPTION_WIREFRAME GL_FILL
#define CONST_OPTION_WIREFRAME GL_LINE
//#define CONST_ON_GEOMETRY_SHADER


#define VALIDATE_OPENGL_ERRORS

#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>

#include <gl/glew.h>
#include <gl/glut.h>
#include <gl/freeglut.h>

#include <gl/gl.h>

#include <glm/glm.hpp>

#include "cShaders/shader.h"

std::vector<glm::fvec3> vertices
{
	/*
	glm::fvec3(0.5f, 0.5f, 0.0f),  // Верхний правый угол
	glm::fvec3(0.5f, -0.5f, 0.0f),  // Нижний правый угол
	glm::fvec3(-0.5f, -0.5f, 0.0f),  // Нижний левый угол
	glm::fvec3(-0.5f, 0.5f, 0.0f)   // Верхний левый угол
	*/
	
	glm::fvec3(-1.0f, -1.0f, 0.0f),
	glm::fvec3(0.0f, -1.0f, 1.0f),
	glm::fvec3(1.0f, -1.0f, 0.0f),
	glm::fvec3(0.0f, 1.0f, 0.0f)
	
	/*
	glm::fvec3(0.0f, 1.0f, 0.0f),
	glm::fvec3(1.0f, -1.0f, 0.0f),
	glm::fvec3(-1.0f, -1.0f, 0.0f),
	glm::fvec3(0.0f, 0.0f, 5.0f)
	*/
};
//glm::fvec3 vertices[4];

std::vector<GLuint> indices
{
	/*
	0, 1, 3,
	1, 2, 3
	*/
	
	0, 3, 1,
	1, 3, 2,
	2, 3, 0,
	0, 2, 1
	
	/*
	0, 1, 2,
	0, 3, 2,
	0, 3, 1,
	1, 3, 2
	*/
};

GLuint VBO;
GLuint vertex_array_object;	// Vertex Array Object
GLuint EBO;

GLint gScaleLocation;
GLuint gWorldLocation;
GLuint rendering_program;

std::chrono::steady_clock::time_point timeStart;

void render(double currentTime);

void ValidateOpenGLErrors()
{
#ifdef VALIDATE_OPENGL_ERRORS
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::string message;
		switch (error)
		{
		case GL_INVALID_ENUM:
			message = "invalid enum passed to GL function (GL_INVALID_ENUM)";
			break;
		case GL_INVALID_VALUE:
			message = "invalid parameter passed to GL function (GL_INVALID_VALUE)";
			break;
		case GL_INVALID_OPERATION:
			message = "cannot execute some of GL functions in current state (GL_INVALID_OPERATION)";
			break;
		case GL_STACK_OVERFLOW:
			message = "matrix stack overflow occured inside GL (GL_STACK_OVERFLOW)";
			break;
		case GL_STACK_UNDERFLOW:
			message = "matrix stack underflow occured inside GL (GL_STACK_UNDERFLOW)";
			break;
		case GL_OUT_OF_MEMORY:
			message = "no enough memory to execute GL function (GL_OUT_OF_MEMORY)";
			break;
		default:
			message = "error in some GL extension (framebuffers, shaders, etc)";
			break;
		}
		std::cout << "OpenGL error: " << message << std::endl;
		std::abort();
	}
#endif
}

void RenderSceneCB()
{
	auto time = std::chrono::high_resolution_clock::now();
	double timeElapsed = std::chrono::duration<double>(time - timeStart).count();
	render(timeElapsed);
	glutSwapBuffers();
	ValidateOpenGLErrors();
	glutPostRedisplay();
	ValidateOpenGLErrors();
}

void render(double currentTime)
{
	const GLfloat background_color[] = {	(float)sin(currentTime + 0.1f) * 0.5f + 0.3f,
											(float)cos(currentTime + 0.4f) * 0.5f + 0.3f,
											0.0f, 1.0f										};

	glClearBufferfv(GL_COLOR, 0, background_color);
	ValidateOpenGLErrors();
	// Use the program object we created earlier for rendering
	glUseProgram(rendering_program);
	ValidateOpenGLErrors();

	GLfloat attrib[] = {	(float)sin(currentTime) * 0.5f,
							(float)cos(currentTime) * 0.6f,
							0.0f, 0.0f							};

	GLfloat triangle_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	
	glVertexAttrib4fv(0, attrib);
	//glVertexAttrib4fv(1, triangle_color);
	ValidateOpenGLErrors();

	// Draw one point
	//glDrawArrays(GL_TRIANGLES, 0, 3);	// if no tesselation
	glDrawArrays(GL_PATCHES, 0, 3);		// with teselation
	
	ValidateOpenGLErrors();
}

GLuint CompileShaders()
{
	int result;
	GLuint shadProg;
	shadProg = glCreateProgram();

	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	ValidateOpenGLErrors();
	result = Shaders::loadShaderFromFile(shadProg, vertShader, "shaders/vertex.glsl");
	RESULT_VALID(result);

	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	ValidateOpenGLErrors();
	result = Shaders::loadShaderFromFile(shadProg, fragShader, "shaders/fragment.glsl");
	RESULT_VALID(result);

	
	GLuint tessellationControl = glCreateShader(GL_TESS_CONTROL_SHADER);
	ValidateOpenGLErrors();
	char error[1024];
	result = Shaders::loadShaderFromFile(shadProg, tessellationControl, "shaders/tessellationControl.glsl", error, sizeof(error));
	if (result) std::cout << error << std::endl;
	RESULT_VALID(result);

	GLuint tessellationEvaluation = glCreateShader(GL_TESS_EVALUATION_SHADER);
	ValidateOpenGLErrors();
	result = Shaders::loadShaderFromFile(shadProg, tessellationEvaluation, "shaders/tessellationEvaluation.glsl", error, sizeof(error));
	if (result) std::cout << error << std::endl;
	RESULT_VALID(result);
	

	#ifdef CONST_ON_GEOMETRY_SHADER
	
	GLuint geometry = glCreateShader(GL_GEOMETRY_SHADER);
	ValidateOpenGLErrors();
	result = Shaders::loadShaderFromFile(shadProg, geometry, "shaders/geometry.glsl", error, sizeof(error));
	if (result) std::cout << error << std::endl;
	RESULT_VALID(result);
	
	#endif

	glLinkProgram(shadProg);
	ValidateOpenGLErrors();
	{
		GLint success;
		glGetProgramiv(shadProg, GL_LINK_STATUS, &success);
		if (success == 0) {
			GLchar ErrorLog[1024];
			glGetProgramInfoLog(shadProg, sizeof(ErrorLog), NULL, ErrorLog);
			//fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
			std::cout << "Error linking shader program: " << ErrorLog << std::endl;
		}
		glDeleteShader(vertShader);
		glDeleteShader(fragShader);
		glDeleteShader(tessellationControl);
		glDeleteShader(tessellationEvaluation);
		//glDeleteShader(geometry);
	}

	glValidateProgram(shadProg); // todo: can be delete in released
	ValidateOpenGLErrors();
	//glUseProgram(ShaderProgram);
	return shadProg;
}

void startup()
{
	rendering_program = CompileShaders();
	glCreateVertexArrays(1, &vertex_array_object);
	ValidateOpenGLErrors();
	glBindVertexArray(vertex_array_object);
	ValidateOpenGLErrors();
	glPointSize(5.0f);
	ValidateOpenGLErrors();
	glPatchParameteri(GL_PATCH_VERTICES, 3);
	ValidateOpenGLErrors();

	timeStart = std::chrono::high_resolution_clock::now();
}

void shutdown()
{
	glDeleteVertexArrays(1, &vertex_array_object);
	glDeleteProgram(rendering_program);
	glDeleteVertexArrays(1, &vertex_array_object);
}

int main(int argc, char* argv[])
{
	int result;
	
	glutInit(&argc, argv);

	//std::cout << (void*)glutGetProcAddress("glutInitWindowPosition") << std::endl;
	//std::cout << (void*)wglGetProcAddress("glPolygonMode") << std::endl;
	//std::cout << (void*)__glewGetProcAddressREGAL("glPolygonMode") << std::endl;

	glutInitContextVersion(4, 5);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Tutorial 01");

	glPolygonMode(GL_FRONT_AND_BACK, CONST_OPTION_WIREFRAME);
	ValidateOpenGLErrors();

	glutDisplayFunc(RenderSceneCB);
	glutIdleFunc(RenderSceneCB);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	ValidateOpenGLErrors();

	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		std::cout << "Error: '" << glewGetErrorString(res) << "'\n";
		return 1;
	}
	std::cout << glGetString(GL_VERSION) << std::endl;

	startup();
	
	
	/*

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::fvec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

	//glGenVertexArrays(1, &VAO);
	glCreateVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::fvec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	
	gWorldLocation = glGetUniformLocation(ShaderProgram, "gWorld");
	assert(gWorldLocation != 0xFFFFFFFF);
	*/



	glutMainLoop();


	return 0;	
}
