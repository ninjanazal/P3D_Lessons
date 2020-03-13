#pragma once
#include <iostream>
#include <string>
#include <cstdlib>	//system

// namespace
namespace jogo_do_galo
{
	// tabuleiroa
	class Board
	{
		// class de tabuleiro, guarda o estado do mesmo
		int board[3][3];
	public:
		// declaraçao das funçoes publicas
		Board();
		void MakeMove(int playerNum, int x, int y);
		void PrintBoard();
	};

	// construtor
	// inicia a matriz com todos os valores a 0
	Board::Board() {
		// completa o array com o valor 0
		std::fill(board, board + sizeof(board), 0);
	}

	// funcs
	// fazer jogada
	void Board::MakeMove(int playerNum, int x, int y) {
		board[x][y] = playerNum;
	}

	// imprime o tabuleiro
	void Board::PrintBoard()
	{
	}
}
