// OpenGL Trenning.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#pragma once

#include "pch.h"

#include <string>
#include <vector>

__forceinline void RESULT_VALID(int result)
{
	if (result != 0) { __debugbreak(); }
}

std::vector<char> readFile(const std::string& filename);