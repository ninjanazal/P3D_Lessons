#pragma once
#include "FiguraGeometrica.h"

namespace ClassAbstract
{
	// class circulor
	class Circulo : public FiguraGeometrica
	{
	public:
		Circulo(float r) { raio_ = r; std::cout << "Circulo Criado!\n"; }// construtor
		~Circulo() { std::cout << "\nCirculo destruido!\n"; }	// destrutor
		float Perimetro() { return 2 * 3.14 * raio_; }	// override de perimetro
		float Area() { return 3.14 * raio_ * raio_; } // override da Area
		void Imprime();	// override de Imprime
	private:
		float raio_;	// valor do raio do circulo
	};

	// definiçao do metodo Imprime
	void Circulo::Imprime()
	{
		// imprime valores de raio
		std::cout << "\n\nCirculo de raio: " << raio_ << std::endl;
		// imprime valor do perimetro e area
		std::cout << "Com area: " << Area() << "\nPerimetro: " << Perimetro() << std::endl;
	}
}
