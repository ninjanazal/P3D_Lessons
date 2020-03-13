#include <iostream>
#include <string>
#include "my_stack.h"

using namespace std;
using namespace my_stack;

void func1(void)
{
	try
	{
		Stack<int> s1(-1);
	}
	catch (const char *e)
	{
		string msg = e;
		if (e == "Tamanho negativo")
		{
			std::cerr << "Erro: " << msg << std::endl
					  << std::endl;
			return;
		}
	}
	catch (...)
	{
		std::cerr << "Erro: ?????\n"
				  << std::endl;
		return;
	}
}

void func2(void)
{
	Stack<int> s(2);

	s.Push(1);
	s.Push(2);

	s.Print();

	s.Pop();
	s.Print();

	s.Pop();
	s.Print();
}

void func3(void)
{
	Stack<string> s(4);

	s.Push("Escola");
	s.Push("Superior");
	s.Push("De");
	s.Push("Tecnologia");
	s.Print();

	s.Pop();
	s.Print();
}

int main(void)
{
	func1();
	func2();
	func3();

	return 0;
}