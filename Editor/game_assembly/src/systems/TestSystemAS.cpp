#include "TestSystemAS.h"
#include <iostream>

void TestSystemAs::OnAwake()
{
	std::cout << "Test System on awake" << std::endl;
}

void TestSystemAs::OnInit()
{
	std::cout << "Test System on init" << std::endl;
}

void TestSystemAs::OnUpdate()
{
	std::cout << "Test System on update" << std::endl;
}

TestSystemAs::TestSystemAs()
{

}

TestSystemAs::~TestSystemAs()
{

}