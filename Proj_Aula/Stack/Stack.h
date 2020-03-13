#pragma once
#include <iostream>

namespace Stack {
	// templade para o tipo de array
	template<class T>
	class MyStack
	{
		T* stack_;	// apontador para o inicio do array da stack
		int top_ = 0;	// indica para o objecto no topo

	public:
		MyStack(int);
		~MyStack() { delete[] stack_; }	// destroi o array inicializado na class
		bool Push(T val);	// definiçao para push
		T Pop();	// definiçao para pop
		void PintStack();	// definiçao para imprimir
	};
	template <class T>
	MyStack<T>::MyStack(int size)
	{
		if (size <= 0) throw "Tamanho negativo";	// avalia se o valor de input é valido
		stack_ = new T[size];		// inicia o array com o tamanho passado por paramentro
	}

	template <class T>
	bool MyStack<T>::Push(T val)
	{
		// push method
		// caso o top ainda esteja dentro do lenth
		if (top_ < stack_.size()) {
			// adiciona o valor ao array
			stack_[top_] = val;
			// incrementa o top
			top_++;
			return true;
		}
		// caso tenha atinjido o maximo
		return false;
	}

	template<class T>
	T MyStack<T>::Pop() {
		// metodo pop
		if (top_ == 0)
			return NULL;	//caso nao tinha nehum elemento no array
		else
		{
			// caso exista
			// guarda o elemento da posiçao
			T val = stack_[top_];
			// decrementa o valor da posiçao em top
			top_--;
			// retorna o elemento
			return val;
		}
	}
	template<class T>
	void MyStack<T>::PintStack() {
		// para cada elemento no array da stack
		for (int i : stack_)
			// imprime o valor
			std::cout << i << std::endl;
	}
}
