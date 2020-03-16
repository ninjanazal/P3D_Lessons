#include <iostream>
#include <fstream>

void BaseReadWrite();
std::string LoadShader(std::string);

int main()
{
	BaseReadWrite();	// chama funçao que le e escreve para um ficherio

	std::cout << "\n\n";	// spacer

	std::string vert_shader = LoadShader("TestShader.txt");	// chama funcçao que le um shader
	//// imprime a string com info do shader
	std::cout << vert_shader << std::endl;

	// aguarda input
	std::cin.get();
	return 0;
}

// estrutura definida 
struct _s { char a;	int b;	float c; };

void BaseReadWrite()
{
	// estrutura do tipo s em array de 2 elementos
	struct _s data1[2] = { {'A', 1, 1.1f}, {'B', 2, 2.2f} };
	// estrutura s em array com 2 elementos vazios
	struct _s data2[2];
	std::fstream fileRW;	// definiçao da var de fstream

	// utilizando a fstream definida anteriormente, abre um ficheiro com o nome "ficheiro.dat",
	// ios::out  para escrita, tipo de escrita em binario, e caso nao exista criar, caso contrario, reescreve
	fileRW.open("ficheiro.dat", std::ios::out | std::ios::binary | std::ios::trunc);
	// escreve para o ficheiro anteriormente abreto, os valores na estrutra data1
	fileRW.write((char*)data1, 2 * sizeof(struct _s));
	// fecha a stream aberta anteriormente
	fileRW.close();

	// abre na stream o ficheiro, para leitura (ios::in) , do tipo binario, e caso nao exista nao cria
	fileRW.open("ficheiro.dat", std::ios::in | std::ios::binary | std::ios::_Nocreate);
	// le do ficheiro a informaçao presente no ficheiro
	fileRW.read((char*)data2, 2 * sizeof(struct _s));
	// fecha a stream 
	fileRW.close();

	// escreve na consola, os valores lidos do ficheiro
	for (int i = 0; i < 2; i++)
		std::cout << data2[i].a << " " << data2[i].b << " " << data2[i].c << std::endl;
}


std::string LoadShader(std::string file)
{
	std::string file_data;	// informaçao do ficheira

	size_t file_size_;	// tamanho do ficheiro
	std::ifstream file_stream_;	// stream para abrir o file

	file_stream_.open(file, std::ios::in | std::ifstream::ate);	// abre a stream apontado para o ficheiro passado
	if (file_stream_.is_open()) {
		// se o ficheiro foi aberto com sucesso
		// como ficheiro foi aberto no final, indica o tamanho do ficheiro
		file_size_ = rsize_t(file_stream_.tellg());
		// repoem o apontador de stream do file para o inicio
		file_stream_.seekg(0, std::ios::beg);

		// inicia array de dados do tamanho do ficheiro
		char* read_data_ = new char[file_size_ + size_t(1)]{};

		// le dados do ficheiro para o array de dados
		file_stream_.read(read_data_, file_size_);
		// guarda na string a informaçao
		file_data = read_data_;

		// fecha a stream de leitura
		file_stream_.close();
	}
	// retorna a informaçao lida
	return file_data;
}