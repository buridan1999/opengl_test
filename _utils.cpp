// OpenGL Trenning.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "pch.h"




#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <fstream>


std::vector<char> readFile(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	if (file.is_open())
	{
		size_t fileSize = (size_t)file.tellg();
		std::vector<char> fileBuffer(fileSize + 1);
		//fileBuffer.reserve(fileSize);
		file.seekg(0);
		file.read(fileBuffer.data(), fileSize);
		file.close();
		fileBuffer[fileSize] = 0;
		//std::cout << "DATA:\n" << fileBuffer.data() << std::endl;
		return fileBuffer;
	}
	else
	{
		throw std::runtime_error("Failed to open file");
	}
}