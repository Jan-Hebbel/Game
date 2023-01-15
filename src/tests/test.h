#pragma once
#include <functional>
#include <string>
#include <iostream>
#include <memory>

namespace test
{
	class Test
	{
	public:
		Test() {}
		virtual ~Test() {}

		virtual void OnUpdate(float delta_time) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};

	class TestMenu : public Test
	{
	private:
		Test*& m_current_test;
		std::vector<std::pair<std::string, std::function<Test* ()>>> m_tests;
	public:
		TestMenu(Test*& current_test_pointer);

		void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Registering test " << name << '\n';
			m_tests.push_back(std::make_pair(name, []() { return new T(); }));
		}
	};
}