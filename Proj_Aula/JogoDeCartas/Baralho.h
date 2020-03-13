#pragma once
#include "Carta.h"
#include <deque>

namespace JogoDeCartas {
	class Baralho
	{
		std::deque<Carta> baralho_;	// queue das cartas do jogador
		const int num_cartas_naipe_ = 13;	// numero de cartas no baralho
		const int num_naipes_ = 4;	// numero de naipes

	public:
		Baralho();	// construtor da classe
		~Baralho() { baralho_.clear(); std::cout << "Baralho eliminado\n"; };	// destrutor da classe
		void Shuffle() { std::random_shuffle(baralho_.begin(), baralho_.end()); };	// baralha o baralho
		Carta* GetDeck(Carta*);	// funçao que retorna um apontador para o inicio de um array de cartas
		void Imprimir();	//assinatura do metodo que imprime todas as cartas
	};

	// construtor da classe
	Baralho::Baralho()
	{
		// preenche o baralho com cartas
		for (int naipe = 0; naipe < num_naipes_; naipe++)
			for (int valor = 0; valor < num_cartas_naipe_; valor++)
			{
				// da push ao valor de carta definido pelos
				// loops adicionando todas as cartas presentes
				// num baralho na queue
				baralho_.push_front(Carta(static_cast<kNaipe>(naipe),
					static_cast<kValor>(valor)));
			}
		// imprime que criou o baralho
		std::cout << "Baralho Criado\n";
	}

	Carta* Baralho::GetDeck(Carta* deck) {
		// se existirem pelo menos 26 cartas no baralho
		if (baralho_.size() >= 26)
		{
			for (int i = 0; i < 26; i++) {
				// atribui a referencia da carta no topo
				deck[i] = baralho_.front();
				// pop na carta
				baralho_.pop_front();
			}
		}
		return deck;
	}

	// definiçao do metodo de impressao
	void Baralho::Imprimir()
	{
		std::cout << std::endl;	// salta linha

		// enquanto existir cartas no baralho
		for (auto c : baralho_)
		{
			// imprime a carta antes de dar pop
			std::cout << strValor[c.GetValor()] << " :: " <<
				strNaipe[c.GetNaipe()] << std::endl;
		}
	}
}