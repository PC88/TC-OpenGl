#pragma once
#include "Test.h"
#include <functional>
#include <Vector>

namespace tests {

	class TestMenu :
		public Test
	{
	public:
		TestMenu(Test*& currentTestPointer);
		virtual ~TestMenu();

		void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Registering test" << name << std::endl;
			m_Tests.push_back(std::make_pair(name, []() {return new T(); }));
		}

	private:
		// this is used so owner ship/creation is managed by a lambda on construction - instead of holding an already existing object ref -PC
		std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;
		Test*& m_CurrentTest;
	};

}
