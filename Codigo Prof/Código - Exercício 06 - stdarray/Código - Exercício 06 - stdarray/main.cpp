#include <iostream>
#include <string>
#include <array>

int main(void) {
	using namespace std;
	const int len = 5;
	array<string, len> str_array;

	// Leitura de 5 frases
	// Texto fica armazenado no std::array
	for (int i = 0; i < int(str_array.size()); i++) {
		cout << "Insira frase:\n";
		getline(std::cin, str_array[i]);
	}

	// Ordenação das frases por ordem alfabética
	for (int i = 0; i < int(str_array.size()) - 1; i++) {
		for (int j = i + 1; j < int(str_array.size()); j++) {
			if (str_array[i] > str_array[j])
				//swap(str_array[i], str_array[j]);
				str_array[i].swap(str_array[j]);
		}
	}

	// Imprime o array já ordenado
	cout << endl << "Lista ordenada:\n";
	for (int i = 0; i < int(str_array.size()); i++) {
		cout << str_array[i] << endl;
	}

	return 0;
}