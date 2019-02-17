// OpenGL Trenning.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#pragma once

#include "pch.h"
#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <fstream>

#include <gl/glew.h>
#include <gl/glut.h>

#include <glm/glm.hpp>

#include <_utils.h>

namespace Shaders
{
	// Structure for codeBlocks
	struct ShaderCodeInfo
	{
		const GLchar** codeParts;
		std::vector<size_t> sizes;	// length of codes parts
	};

	

	int loadShaderFromFile(GLuint shaderProgram, GLuint shader, std::string filename, char* error_description = nullptr, int error_description_size = 0);
	int compile(GLuint shaderProgram, GLuint shader, ShaderCodeInfo* shaderCodeInfo, char* error_description = nullptr, int error_description_size = 0);
}
