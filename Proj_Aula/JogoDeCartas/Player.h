#pragma once

#include <vector>
#include "Carta.h"

namespace JogoDeCartas {
	enum class kPlayerType { Jogador, Ai };	// tipo do jogador

	class Player
	{
		kPlayerType type_;	// tipo de jogador
		std::vector<Carta> deck_;	// deck do jogador

	public:
		Player(kPlayerType, Carta[], int);	// assinatura construtor do jogador
		~Player() { deck_.clear(); };	// destrutor do player
		int Play(int index);	// jogada do jogador
		int CounterPlay(int);	// jogada AI
		void Imprimir();	// imprime as cartas do baralho do jogador
	};

	// decleraçao de construtor
	Player::Player(kPlayerType type, Carta deck[], int count)
	{
		// guarda nas vars locais
		this->type_ = type;
		// adiciona as cartas recebidas no deck do jogador
		for (int i = 0; i < count; i++)
			this->deck_.push_back(deck[i]);
	}

	// jogada do jogador
	int Player::Play(int index)
	{
		//retorna o valor da carta jogada
		Carta card = deck_[index];
		// imprime a jogada
		std::cout << "\tJogou: " << strValor[card.GetValor()] <<
			" :: " << strNaipe[card.GetNaipe()] << std::endl;

		// remove o elemento do vector
		deck_.erase(deck_.begin() + (index));
		// retorna o valor da carta
		return card.GetValor();
	}

	// jogada AI
	int Player::CounterPlay(int cardvalue)
	{
		// carta escolhida pela ai
		int cardIndex = 0; // inicia como a primeira carta no baralho
		Carta selectedCard;	// carta escolhida

		// procura pelo baralho a menor carta para jogar
		for (int i = 1; i < deck_.size() -1; i++)
			if (deck_[i].GetValor() >= cardvalue)
				cardIndex = i;

		selectedCard = deck_[cardIndex];
		// remove a carta selecionada
		deck_.erase(deck_.begin() + (cardIndex));

		// imprime a jogada da AI
		std::cout << "\t AI Jogou: " << strValor[selectedCard.GetValor()] <<
			" :: " << strNaipe[selectedCard.GetNaipe()] << std::endl;

		// retorna a carta selecionada
		return selectedCard.GetValor();
	}

	// decleraçao do metodo de imprimir do jogador
	void Player::Imprimir()
	{
		// imprime o titulo
		// caso seja o jogador
		if (type_ == kPlayerType::Jogador)
			std::cout << "\n\n:: Deck do Jogador ::\n";
		else
			std::cout << "\n\n:: Deck da AI ::\n";

		for (int index = 0; index < deck_.size(); index++)
		{
			// guarda apontador para o inicio da list
			auto card = deck_.begin();
			// avança index posiçoes
			std::advance(card, index);
			// imprime o numero da carta no deck, e a carta
			std::cout << index << " -> " << strValor[card->GetValor()] << " :: " <<
				strNaipe[card->GetNaipe()] << std::endl;
		}
	}
}