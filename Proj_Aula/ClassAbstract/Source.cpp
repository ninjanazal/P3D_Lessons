#include <iostream>
#include "Circulo.h"
#include "Retangulo.h"

// ex para classes abstratas
int main()
{
	// utiliza o namespace
	using namespace ClassAbstract;

	// apontadores para formas geometrica
	FiguraGeometrica* rect, * circ;

	// criar object rectangulo
	rect = new Retangulo(3, 5);
	// criar object circulo
	circ = new Circulo(5);

	rect->Imprime();	//chama metodo de impressao
	circ->Imprime();	// chama metodo de impressao

	// destroi objectos
	delete rect;
	delete circ;
	// aguarda press enter
	std::cin.get();
	return 0;
}