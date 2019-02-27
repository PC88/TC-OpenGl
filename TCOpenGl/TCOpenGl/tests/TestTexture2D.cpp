#include "TestTexture2D.h"
#include "Renderer.h"

#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace tests {

	TestTexture2D::TestTexture2D()
		: m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0),
		m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
	{
		float positons[] // array of float to define out vertexes - PC
		{
		  -50.0f, -50.0f, 0.0f, 0.0f,// 0 // each line now is basically 2 vec2`s
		  50.0f, -50.0f, 1.0f, 0.0f,// 1 // the first 2 are vertex positions, the second 2 are Texture coordinates -PC
		  50.0f, 50.0f, 1.0f, 1.0f, // 2
		  -50.0,  50.0f, 0.0f, 1.0f // 3
		};

		unsigned int indices[] =
		{
			0, 1, 2,
			2, 3, 0
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		
		m_VAO = std::make_unique<VertexArray>();
		m_VBO = std::make_unique<VertexBuffer>(positons, 4 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2); // here is the separation of the data in the float array "positions" -PC
		layout.Push<float>(2);
		m_VAO->AddBuffer(*m_VBO, layout);
		m_IBO = std::make_unique<IndexBuffer>(indices, 6);

		m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
		m_Shader->Bind(); // gl use program shader -PC
		m_Shader->SetUniform4f("u_Color", 0.8, 0.3f, 0.8f, 1.0f);

		m_Texture = std::make_unique<Texture>("res/textures/testPNG.png");
		m_Shader->SetUniform1i("u_Texture", 0); // second param matches above "slot", default is 0
	}

	TestTexture2D::~TestTexture2D()
	{
	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{
	}

	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;
		m_Texture->Bind();

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
			glm::mat4 mvp = m_Proj * m_View * model; // this is in coloumb Major ordering as per OpenGL, this means in DX or Row Major ordering this would be multiplied M*V*P -PC
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
		}

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
			glm::mat4 mvp = m_Proj * m_View * model; // this is in coloumb Major ordering as per OpenGL, this means in DX or Row Major ordering this would be multiplied M*V*P -PC
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
		}
	}

	void TestTexture2D::OnImGuiRender()
	{
		ImGui::Begin("Debug!");                          // Create a window 
		ImGui::SliderFloat3("translation A", &m_TranslationA.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::SliderFloat3("translation B", &m_TranslationB.x, 0.0f, 960.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
}