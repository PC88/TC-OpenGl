#pragma once

#include <GL\glew.h>

// NOTE glGetError() can be replaced with the improved glDebugMessageCallback() - V4.3 and above only

#define ASSERT(x) if (!(x)) __debugbreak(); // MSVC compiler intrinsic which will break upon a false return, __ denotes it -PC
// below Macro treats x as a type, and will execute x if it is a function. "\" on below lines means no new line so we can clearly define the Macro -PC
#define GLCall(x) GLClearError();\
   x;\
   ASSERT(GLLogCall(#x, __FILE__, __LINE__))


// note: the types for tutorial functions are using primitive types and not the OpenGL typedefs for them, GL_ENUM etc.

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

