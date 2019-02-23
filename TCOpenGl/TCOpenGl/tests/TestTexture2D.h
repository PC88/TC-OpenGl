#pragma once
#include "Test.h"


#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include <memory>

namespace tests {
	class TestTexture2D :
		public Test
	{
	public:
		TestTexture2D();
		virtual ~TestTexture2D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		glm::vec3 m_TranslationA, m_TranslationB;
		std::unique_ptr<VertexArray>  m_VAO;
		std::unique_ptr<IndexBuffer> m_IBO;
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;

		glm::mat4 m_Proj, m_View;

	};
}
