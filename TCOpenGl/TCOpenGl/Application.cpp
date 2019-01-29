#include <iostream>

#define GLEW_STATIC

#include <GL\glew.h>
#include <GLFW/glfw3.h>

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	

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

	glfwTerminate();
	return 0;
}
