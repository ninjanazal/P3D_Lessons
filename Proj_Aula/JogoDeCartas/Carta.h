#pragma once
#include <iostream>
#include <string.h>
#include <algorithm>


namespace JogoDeCartas {
	// enumerados com os Naipes e valor da carta existentes
	enum class kNaipe { Ouros, Espadas, Copas, Paus };
	enum class kValor {
		Dois, Tres, Quatro, Cinco, Seis, Sete, Oito, Nove, Dez,
		Valete, Dama, Rei, As
	};
	// definiçao em string dos nomes dos naipes
	std::string strNaipe[] = { "Ouros", "Espadas", "Copas","Paus" };

	// definiçao em string dos nomes de valor
	std::string strValor[] = {
		"Dois", "Tres", "Quatro", "Cinco", "Seis", "Sete", "Oito", "Nove", "Dez",
		"Valete", "Dama", "Rei", "As" };

	class Carta
	{
		kNaipe naipe_ = kNaipe::Copas;	// Naipe a que a carta pertence
		kValor valor_ = kValor::As;	// valor da carta
	public:
		Carta() {};	// construtor default
		Carta(kNaipe n, kValor v) { naipe_ = n; valor_ = v; };		// construtor da classe
		~Carta() {};	// destrutor da class
		int GetNaipe() { return static_cast<int>(naipe_); }	// getter do naipe
		int GetValor() { return static_cast<int>(valor_); }	// getter para o valor
	};
}
