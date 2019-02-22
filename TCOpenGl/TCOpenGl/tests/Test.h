#pragma once

namespace tests {

	class Test
	{
	public:
		Test();
		virtual ~Test();

		// none of there are pure virtual as this means some can be used, but its not mandatory to implement ALL of em. -PC
		virtual void OnUpdate(float deltaTime) {};
		virtual void OnRender() {};
		virtual void OnImGuiRender() {};

	};

}

