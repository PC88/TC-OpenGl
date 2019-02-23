#pragma once
#include "Test.h"
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

	};
}
