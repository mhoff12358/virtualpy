#ifndef __LOGGING_H_INCLUDED__
#define __LOGGING_H_INCLUDED__

#include "Windows.h"
#include <sstream>
#include <string>
#include <iomanip>
#include "D3D11.h"

void OutputFormatted(const char* format, ...);

std::string floatToString(float a);

std::string doubleToString(double a);

std::string floatTripleToString(float a, float b, float c);

std::string intToString(int a);

HRESULT HandleHResult(HRESULT result);

#endif