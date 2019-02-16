#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#include <GL\glew.h>
#include <GLFW/glfw3.h>
// note: the types for tutorial functions are using primitive types and not the OpenGL typedefs for them, GL_ENUM etc.

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
	{
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // changed to Core over Compat, VAO`s added -PC

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // sets frame rate = refresh rate

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error!" << std::endl;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;
	{ // this scope exists to deal with the issue of GL`s error being infinte looped - TODO heap allocate so this dosent exist -PC
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

		VertexArray va;
		VertexBuffer vb(positons, 4 * 2 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		// NOTE the above line replaced:
		//GLCall(glEnableVertexAttribArray(0));
		//GLCall(glVertexAttribPointer(0,2, GL_FLOAT, GL_ FALSE, sizeof(float) * 2, 0));
		va.AddBuffer(vb, layout);


		IndexBuffer ib(indices, 6);

		Shader shader("res/shaders/Basic.shader");
		shader.Bind(); // gl use program shader -PC
		float r = 0.0f;
		float increment = 0.05f;
		shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

		/*std::cout << "VERTEX" << std::endl;
		std::cout << source.VertexSource << std::endl;

		std::cout << "FRAGMENT" << std::endl;
		std::cout << source.FragmentSource << std::endl;*/


		

		va.Unbind();
		vb.UnBind();
		ib.UnBind();
		shader.Unbind();


		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

			
			va.Bind();  // GLCall(glBindVertexArray(vao)); - replaced
			ib.Bind();  // GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo)); - replaced

			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // Draw call, wrapped in debug macro - PC

			if (r > 1.0f)
			{
				increment = -0.05f;
			}
			else if (r < 0.0f)
			{
				increment = 0.05f;
			}
			r += increment;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
	glfwTerminate();
	return 0;
}
