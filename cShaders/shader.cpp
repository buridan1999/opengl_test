// OpenGL Trenning.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//



#include "pch.h"

#include "shader.h"

#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <fstream>

#include <gl/glew.h>
#include <gl/glut.h>

#include <glm/glm.hpp>




int Shaders::loadShaderFromFile(GLuint shaderProgram, GLuint shader, std::string filename, char* error_description, int error_description_size)
{
	std::vector<char> shaderCode = readFile(filename);
	char* vertShaderCode = shaderCode.data();
	
	ShaderCodeInfo codeInfo;
	{
		codeInfo.codeParts = (const GLchar**)&vertShaderCode;
		codeInfo.sizes = { strlen(vertShaderCode) };
	}
	return Shaders::compile(shaderProgram, shader, &codeInfo, error_description, error_description_size);
}

int Shaders::compile(GLuint shaderProgram, GLuint shader, ShaderCodeInfo* shaderCodeInfo, char* error_description, int error_description_size)
{
	glShaderSource(shader, 1, shaderCodeInfo->codeParts, (GLint*)shaderCodeInfo->sizes.data());
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		if (error_description)
		{
			glGetShaderInfoLog(shader, error_description_size, NULL, error_description);
		}
		return -1;
	}
	else
	{
		glAttachShader(shaderProgram, shader);
		return 0;
	}
}
