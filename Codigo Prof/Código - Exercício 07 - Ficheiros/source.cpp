#include <iostream>
#include <fstream>

int main(void) {
	std::string data_from_shader_file;

	// Abre o ficheiro "shader.vert" para leitura, e coloca-se na última posição do ficheiro.
	std::fstream shader_file("shader.vert", std::ios::in | std::ifstream::ate);

	// Se o ficheiro foi aberto.
	if (shader_file.is_open()) {
		// Leitura da próxima posição de leitura.
		size_t tamanhoDoFicheiroEmBytes = size_t(shader_file.tellg()); // size_t = inteiro de 8 bytes
		// Reposiciona a leitura do ficheiro no seu início.
		shader_file.seekg(0, std::ios::beg);

		// Alocação de espaço de memória para dados do ficheiro.
		char* data = new char[tamanhoDoFicheiroEmBytes + size_t(1)]{};

		// Leitura do ficheiro para o array 'source'.
		shader_file.read(data, tamanhoDoFicheiroEmBytes);

		// Fecha a string.
		data[tamanhoDoFicheiroEmBytes] = 0;

		data_from_shader_file = data;

		delete[] data;

		shader_file.close();
	}

	std::cout << data_from_shader_file << std::endl;

	return 0;
}