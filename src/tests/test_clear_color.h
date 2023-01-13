#pragma once

#include "test.h"

namespace test
{
	class TestClearColor : public Test
	{
	private:
		float m_clear_color[4];
	public:
		TestClearColor();
		~TestClearColor();

		void OnUpdate(float delta_time) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}