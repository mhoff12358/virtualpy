#include "Logging.h"

void OutputFormatted(const char* format, ...) {
	static char output_string[2048];
	va_list args;
	va_start(args, format);
	if (format != NULL && *format != 0) {
		vsprintf_s(output_string, format, args);
		OutputDebugString(output_string);
	}
	OutputDebugString("\n");
}

std::string floatToString(float a) {
	std::ostringstream ss;
	ss << a;
	return ss.str();
}

std::string doubleToString(double a) {
	std::ostringstream ss;
	ss << a;
	return ss.str();
}

std::string floatTripleToString(float a, float b, float c) {
	return floatToString(a) + ", " + floatToString(b) + ", " + floatToString(c);
}

std::string intToString(int a) {
	std::ostringstream ss;
	ss << a;
	return ss.str();
}

HRESULT HandleHResult(HRESULT result) {
	if (!SUCCEEDED(result)) {
		std::ostringstream ss;
		ss << "Failing HR code: ";
		ss << std::hex << std::setfill('0');
		ss << std::setw(8) << result;
		ss << std::endl;
		OutputDebugString(ss.str().c_str());
	}
	return result;
}