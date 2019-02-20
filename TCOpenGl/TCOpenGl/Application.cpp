#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

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
	window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
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
	{ // this scope exists to deal with the issue of GL`s error being infinite looped - TODO heap allocate so this dose not exist -PC
		float positons[] // array of float to define out vertexes - PC
		{
		  100.0f, 100.0f, 0.0f, 0.0f,// 0 // each line now is basically 2 vec2`s
		  200.0f, 100.0f, 1.0f, 0.0f,// 1 // the first 2 are vertex positions, the second 2 are Texture coordinates -PC
		  200.0f, 200.0f, 1.0f, 1.0f,// 2
		  100.0,  200.0f, 0.0f, 1.0f // 3
		};

		unsigned int indices[] =
		{
			0, 1, 2,
			2, 3, 0
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		VertexArray va;
		VertexBuffer vb(positons, 4 * 4 * sizeof(float)); // increased to 4 instead of 2 to take on the load of texture cooridintes -PC

		VertexBufferLayout layout;
		layout.Push<float>(2); // here is the separation of the data in the float array "positions" -PC
		layout.Push<float>(2);
		// NOTE the above line replaced:
		//GLCall(glEnableVertexAttribArray(0));
		//GLCall(glVertexAttribPointer(0,2, GL_FLOAT, GL_ FALSE, sizeof(float) * 2, 0));
		va.AddBuffer(vb, layout);


		IndexBuffer ib(indices, 6);

		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));

		glm::mat4 mvp = proj * view * model; // this is in coloumb Major ordering as per OpenGL, this means in DX or Row Major ordering this would be multiplied M*V*P -PC

		Shader shader("res/shaders/Basic.shader");
		shader.Bind(); // gl use program shader -PC
		float r = 0.0f;
		float increment = 0.05f;
		shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
		shader.SetUniformMat4f("u_MVP", mvp); // was proj now MVP -PC

		Texture texture("res/textures/testPNG.png");
		texture.Bind(); // remember this take in a "slot"
		shader.SetUniform1i("u_Texture", 0); // second param matches above "slot", default is 0

		/*std::cout << "VERTEX" << std::endl;
		std::cout << source.VertexSource << std::endl;

		std::cout << "FRAGMENT" << std::endl;
		std::cout << source.FragmentSource << std::endl;*/

		va.Unbind();
		vb.UnBind();
		ib.UnBind();
		shader.Unbind();

		Renderer renderer;


		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();
			
			shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

			renderer.Draw(va, ib, shader);

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
