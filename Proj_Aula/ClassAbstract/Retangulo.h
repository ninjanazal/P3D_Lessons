#pragma once
#include "FiguraGeometrica.h"
#include <math.h>

namespace ClassAbstract
{
	class Retangulo :public FiguraGeometrica
	{
	public:
		Retangulo(int, int);
		~Retangulo() { std::cout << "\nRetangulo destruido!\n"; }	// destrutor
		float Area() { return largura_ * altura_; }	// overide da area
		float Perimetro() { return (largura_ * 2) + (altura_ * 2); }// overide de perimetro
		void Imprime();	// override de imprime

	private:
		int largura_, altura_;	// vars para definiçao da largura e altura
	};
	// definiççao do construtor
	Retangulo::Retangulo(int l, int a)
	{
		// guarda valores atribuidos no objecto
		largura_ = l;
		altura_ = a;
		// imprime uma confirmaçao da criaçao
		std::cout << "Rectangulo criado\n";
	}
	// definiçao do metodo Imprime
	void Retangulo::Imprime()
	{
		// imprime valores de altura e largura
		std::cout << "\n\nRectangulo com:\nLargura: " << largura_ <<
			"\nAltura: " << altura_ << std::endl;

		// imprime valor da area e perimetro
		std::cout << "Com area: " << Area() << "\nPerimetro: " << Perimetro() << std::endl;
	}
}
