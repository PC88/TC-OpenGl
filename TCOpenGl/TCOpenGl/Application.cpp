#include <iostream>

#define GLEW_STATIC

#include <GL\glew.h>
#include <GLFW/glfw3.h>
// note: the types for tutorial functions are using primative types and not the OpenGL type defs for them, GL_ENUM etc.

static unsigned int CompileShader(unsigned int type, const std::string& source )
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);


	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int lenght;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
		char* message = (char*)alloca(lenght * sizeof(char));
		glGetShaderInfoLog(id, lenght, &lenght, message);
		std::cout << "Failed to compile "  
			<< (type == GL_VERTEX_SHADER ? "vertax" : "fragment") <<
			"shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
	}

	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

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

	float positons[6] // array of float to define out vertexes - PC
	{
	 -0.5f, -0.5f,
	  0.0f,  0.5f,
	  0.5f, -0.5f
	};



	unsigned int buffer;
	glGenBuffers(1, &buffer); // this generates a buffer and gives us back a unique ID - PC // create buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer); // bind or select that buffer - PC
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positons, GL_STATIC_DRAW); // 6 times the amount of vertexes held in array "positions" - PC // define size of buffer

	glEnableVertexAttribArray(0); // enable the VAP - PC
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);// detail on this composed in notes for future ref 

	std::string vertexShader = // defining these in files is a lot better -PC
		"#version 330 core/n"
		"/n"
		"layout(location = 0) in vec4 position;/n" // 0 in this line is the index(the enumerated value we gave our VAP)
		"/n"
		"void main()/n"
		"{/n"
		"gl_position = position;/n"
		"}/n";

	std::string fragmentShader = // defining these in files is a lot better -PC
		"#version 330 core/n"
		"/n"
		"layout(location = 0) out vec4 color;/n" // 0 in this line is the index(the enumerated value we gave our VAP)
		"/n"
		"void main()/n"
		"{/n"
		" color = vec4(1.0, 0.0, 0.0, 1.0);/n" //RGBA
		"}/n";
	unsigned int shader = CreateShader(vertexShader, fragmentShader);
	glUseProgram(shader);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3); // Draw call - PC

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(shader);
	glfwTerminate();
	return 0;
}
