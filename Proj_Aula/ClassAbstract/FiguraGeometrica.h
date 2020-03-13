#pragma once
#include <iostream>

namespace ClassAbstract
{
	class FiguraGeometrica
	{
	public:
		//FiguraGeometrica();	// construtor
		virtual ~FiguraGeometrica() { std::cout << "\nForma Destruida!\n"; }	//destrutor
		virtual float Perimetro() = 0;	// defini�ao do metodo de calculo do perimetro
		virtual float Area() = 0;	//defini�ao do metodo de calculo da area
		virtual void Imprime() = 0;	// defini�ao de metrodo para impressao
	};
}
