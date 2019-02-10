#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
// NOTE glGetError() can be replaced with the improved glDebugMessageCallback() - V4.3 and above only

#define GLEW_STATIC
#define ASSERT(x) if (!(x)) __debugbreak(); // MSVC compiler intrinsic which will break upon a false return, __ denotes it -PC
// below Macro treats x as a type, and will execute x if it is a function. "\" on below lines means no new line so we can clearly define the Macro -PC
#define GLCall(x) GLClearError();\
   x;\
   ASSERT(GLLogCall(#x, __FILE__, __LINE__))

#include <GL\glew.h>
#include <GLFW/glfw3.h>
// note: the types for tutorial functions are using primitive types and not the OpenGL typedefs for them, GL_ENUM etc.

static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR)
	{

	}
}

static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error (" << error << ")" << function <<
			" " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

struct ShaderProgramSource // struct used to end around multiple return types -PC
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << '\n'; // note single quotes -PC
		}
	}
	return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source )
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));


	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int lenght;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght));
		char* message = (char*)alloca(lenght * sizeof(char)); // alloca, C style allocation on the stack, a bit strange but interesting and effective.
		GLCall(glGetShaderInfoLog(id, lenght, &lenght, message));
		std::cout << "Failed to compile "  
			<< (type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<
			"shader!" << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteShader(id));
	}

	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
	{
		return -1;
	}

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error!" << std::endl;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	float positons[] // array of float to define out vertexes - PC
	{
	 -0.5f,  -0.5f, // 0
	  0.5f,  -0.5f, // 1
	  0.5f,   0.5f, // 2
	 -0.5f,   0.5f  // 3
	};

	unsigned int indices[] = 
	{
		0, 1, 2,
		2, 3, 0
	};

	unsigned int buffer;
	GLCall(glGenBuffers(1, &buffer)); // this generates a buffer and gives us back a unique ID - PC // create buffer
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer)); // bind or select that buffer - PC
	GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positons, GL_STATIC_DRAW)); // 6 times the amount of vertexes held in array "positions" - PC // define size of buffer

	GLCall(glEnableVertexAttribArray(0)); // enable the VAP - PC
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));// detail on this composed in notes for future ref 

	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");// this is relative to the "Working Directory" which when run in debug is the ProjectDirectory
	// note above that the slashes in the string literal are reversed to stop an escape sequence

	std::cout << "VERTEX" << std::endl;
	std::cout << source.VertexSource << std::endl;

	std::cout << "FRAGMENT" << std::endl;
	std::cout << source.FragmentSource << std::endl;

	unsigned int ibo;
	GLCall(glGenBuffers(1, &ibo)); // this generates a buffer and gives us back a unique ID - PC // create buffer
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo)); // bind or select that buffer - PC
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource); // changed to take in parsed shader -PC
	GLCall(glUseProgram(shader));

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // Draw call, wrapped in debug macro - PC

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	GLCall(glDeleteProgram(shader));
	glfwTerminate();
	return 0;
}
