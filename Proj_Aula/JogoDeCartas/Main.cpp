#include "Baralho.h"
#include "Player.h"
#include <sstream>

int main()
{
	using namespace JogoDeCartas;	// utiliza o namespace

	std::string last_input_;	// valor de input
	int input_valid_;	// valor introduzido valido
	int play_count = 1;	// numero de jogadas
	Carta* p_decK_ = new Carta[26];	// deck de cartas para os jogadores

	Baralho* baralho;	// cria apontador para o baralho
	baralho = new Baralho();	// inicia um baralho
	baralho->Imprimir();	// imprime um baralho

	baralho->Shuffle();	// baralha
	baralho->Imprimir();	//imprime o baralho

	Player* local_player_, * ai_local_;	// jogador local e ai
	// inicia o jogador
	local_player_ = new Player(kPlayerType::Jogador, baralho->GetDeck(p_decK_), 26);
	// inicia AI local
	ai_local_ = new Player(kPlayerType::Ai, baralho->GetDeck(p_decK_), 26);

	// loop de jogo
	while (true)
	{
		// diferencia loop de jogadas
		std::cout << "------------------ Jogada -> " << play_count << "------------------\n\n";
		local_player_->Imprimir();	// imprime deck do jogador
		ai_local_->Imprimir();	// imprime deck da AI
		// indica para o jogador escolher uma carta
		std::cout << "\n-> -1 para terminar\nEscolher uma carta: ";

		// guarda a linha de entrada na var de last input
		std::getline(std::cin, last_input_);
		// inprime o que foi introduzido
		std::cout << "Your input: " << last_input_ << std::endl;
		// converte string para int, caso nao consiga, retorna 0
		std::istringstream(last_input_) >> input_valid_;

		// valida se o input reproduz o final do jogo
		if (input_valid_ == -1) break;
		// regista a jogada
		int playerval = local_player_->Play(input_valid_);
		// determina se a jogada da ai 
		if (playerval > ai_local_->CounterPlay(playerval))
		{
			// indica que o jogador ganhou
			std::cout << "Jogador Ganha!" << std::endl; 
			std::cin.get();	// aguarda input
			break;
			// aumenta o numero de jogadas
			play_count++;
		}
		std::cin.get();	// aguarda input
	}
	return 0;
}