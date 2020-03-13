#pragma once

#include <iostream>

// NOTA:
//
// No caso de utiliza��o de templates, n�o podemos separar a declara��o de uma classe
// da defini��o das suas fun��es-membro. Isto �, n�o podemos colocar a defini��o da classe
// num ficheiro .h e as suas fun��es-membro num ficheiro .c.
// Tudo que seja relativo a um template deve estar descrito num �nico ficheiro.
// Tal � necess�rio porque o template, por si s�, n�o gera c�digo na compila��o.

namespace my_stack
{
template <class T>
class Stack
{
public:
	Stack(const int length);
	~Stack();
	bool Push(T value);
	T Pop(void);
	void Print(void);

private:
	T *data_ = nullptr;
	int size_ = 0, top_ = 0;
};

template <class T>
Stack<T>::Stack(const int length)
{
	if (length <= 0)
		throw "Tamanho negativo";
	data_ = new T[length];
	size_ = length;
	std::cout << "Stack Criada" << std::endl;
}

template <class T>
Stack<T>::~Stack()
{
	delete[] data_;
	std::cout << "Stack Destruida\n"
			  << std::endl;
}

template <class T>
bool Stack<T>::Push(T value)
{
	bool ret = false;

	if (top_ < size_)
	{
		data_[top_++] = value;
		ret = true;
	}

	return ret;
}

template <class T>
T Stack<T>::Pop(void)
{
	T ret{};

	if (top_ > 0)
	{
		ret = data_[--top_];
	}

	return ret;
}

template <class T>
void Stack<T>::Print(void)
{
	for (int i = 0; i < top_; i++)
		std::cout << data_[i] << ' ';
	std::cout << std::endl;
}
} // namespace my_stack