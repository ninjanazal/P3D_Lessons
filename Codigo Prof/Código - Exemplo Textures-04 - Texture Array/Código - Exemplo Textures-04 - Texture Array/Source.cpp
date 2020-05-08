#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")


#include <iostream>
#include <string>
#include <vector>
#include <chrono>
using namespace std;

#define GLEW_STATIC
#include <GL\glew.h>

#define GLFW_USE_DWM_SWAP_INTERVAL
#include <GLFW\glfw3.h>

#include "LoadShaders.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void print_error(int error, const char *description);
void print_gl_info(void);
int load_textures(vector<string> textureFiles);
void init(void);
void display(void);
bool showNext(double secondsInterval);


#define WIDTH 800
#define HEIGHT 600


GLuint VAO;
GLuint Buffer;
const GLuint NumVertices = 6;
GLuint programa;


int main(void) {
	GLFWwindow *window;

	glfwSetErrorCallback(print_error);

	if (!glfwInit()) return -1;

	window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL - Texture Array", NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);
	
	// Inicia o gestor de extensões GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	print_gl_info();

	vector<string> textureFiles{
		"textures/Teleporter0.png",
		"textures/Teleporter1.png",
		"textures/Teleporter2.png",
		"textures/Teleporter3.png",
		"textures/Teleporter4.png"
	};
	int numberOfTextureImages = load_textures(textureFiles);

	init();

	// Indicação da Unidade de Textura a ligar ao sampler 'textureArray'.
	GLint locationTextureArray = glGetProgramResourceLocation(programa, GL_UNIFORM, "textureArray");
	glProgramUniform1i(programa, locationTextureArray, 0 /* Unidade de Textura #0 */);
	// Atribuição de um valor numérico (que indica o número de elementos no array) ao uniform 'nLayers'
	glProgramUniform1i(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "nLayers"), numberOfTextureImages /* Número de imagens no array */);
	
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		// Em alguns casos, pode ser útil passar também como uniform um valor inteiro que indique qual a imagem do array que se pretende exibir.
		static int showTexture = 0;
		glProgramUniform1i(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "showLayer"), showTexture /* Índice da imagem a exibir */);
		// Verifica se já se atingiu o tempo necessário para se efetuar a transição de textura.
		if (showNext(0.2 /* segundos */)) {
			showTexture++;
			if (showTexture >= numberOfTextureImages)
				showTexture = 0;
		}

		display();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void print_gl_info(void) {
	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	cout << "\nOpenGL version " << major << '.' << minor << endl;

	const GLubyte *glversion = glGetString(GL_VERSION);
	const GLubyte *glvendor = glGetString(GL_VENDOR);
	const GLubyte *glrenderer = glGetString(GL_RENDERER);
	cout << "\nVersion:  " << glversion << endl << 
		    "Vendor:   " << glvendor << endl << 
		    "Renderer: " << glrenderer << endl;

	cout << "\nSupported GLSL versions:\n";
	const GLubyte *glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	cout << "Higher supported version:\n\t" << glslversion << endl;
	GLint numglslversions;
	cout << "Other supported versions:\n";
	glGetIntegerv(GL_NUM_SHADING_LANGUAGE_VERSIONS, &numglslversions);
	for (int n = 0; n < numglslversions; n++) {
		cout << '\t' << glGetStringi(GL_SHADING_LANGUAGE_VERSION, n) << endl;
	}
}

void print_error(int error, const char *description) {
	cout << description << endl;
}

void init(void) {
	// ****************************************************
	// Criar arrays de dados na RAM
	// ****************************************************
	
	GLfloat vertices[NumVertices * 8] = {
		// Primeiro triângulo
		// Posições			 // Cores			// Coordenadas de textura
		-0.4f, -0.8f, 0.0f,	 1.0f, 0.0f, 0.0f,	0.0f, 0.0f,	// bottom left
		0.4f,  -0.8f, 0.0f,	 0.0f, 1.0f, 0.0f,	1.0f, 0.0f,	// bottom right
		-0.4f,  0.8f, 0.0f,	 1.0f, 0.0f, 1.0f,	0.0f, 1.0f,	// top left

		// Segundo triângulo
		// Posições			 // Cores			// Coordenadas de textura
		0.4f,  -0.8f, 0.0f,	 0.0f, 1.0f, 0.0f,	1.0f, 0.0f,	// bottom right
		0.4f,   0.8f, 0.0f,	 0.0f, 0.0f, 1.0f,	1.0f, 1.0f,	// top right
		-0.4f,  0.8f, 0.0f,	 1.0f, 0.0f, 1.0f,	0.0f, 1.0f	// top left
	};

	// ****************************************************
	// VAOs - Vertex Array Objects
	// ****************************************************

	// Gerar nomes para VAOs.
	// Neste caso gera apenas 1 nome.
	glGenVertexArrays(1, &VAO);
	// Faz bind do VAO, cujo nome está definido em 'VAO', com o contexto do OpenGL.
	// Um VAO é criado no primero bind que lhe seja feito.
	// Este VAO passa a estar ativo até que seja feito o bind a outro VAO, ou seja feito o bind com valor 0.
	glBindVertexArray(VAO);

	// ****************************************************
	// VBOs - Vertex Buffer Objects
	// ****************************************************

	// Gera 'NumBuffers' nomes para VBOs.
	// Neste caso gera 1 nome
	// Esta função pode ser chamada antes da criação de VAOs.
	glGenBuffers(1, &Buffer);
	glBindBuffer(GL_ARRAY_BUFFER, Buffer);
	// Inicializa o VBO (que está ativo) com dados imutáveis.
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(vertices), vertices, 0);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// ****************************************************
	// Shaders
	// ****************************************************

	ShaderInfo  shaders[] = {
		{ GL_VERTEX_SHADER,   "shaders/texture.vert" },
		{ GL_FRAGMENT_SHADER, "shaders/texture.frag" },
		{ GL_NONE, NULL }
	};

	programa = LoadShaders(shaders);
	if (!programa) exit(EXIT_FAILURE);
	glUseProgram(programa);

	// ****************************************************
	// Ligar Atributos aos Shaders
	// ****************************************************

	// Obtém a localização do atributo 'vPosition' no 'programa'.
	GLint coordsId = glGetProgramResourceLocation(programa, GL_PROGRAM_INPUT, "vPosition");
	// Obtém a localização do atributo 'vColors' no 'programa'.
	GLint coresId = glGetProgramResourceLocation(programa, GL_PROGRAM_INPUT, "vColors");
	// Obtém a localização do atributo 'vTextureCoords' no 'programa'.
	GLint texturaId = glGetProgramResourceLocation(programa, GL_PROGRAM_INPUT, "vTextureCoords");
	
	// Ativa o VBO 'Buffer'.
	glBindBuffer(GL_ARRAY_BUFFER, Buffer);
	// Liga a localização do atributo 'vPosition' dos shaders do 'programa', ao VBO e VAO (ativos).
	// Especifica também como é que a informação do atributo 'coordsId' deve ser interpretada.
	// Neste caso, o atributo irá receber, por vértice, 3 elementos do tipo float. Stride de 8 floats e offset de 0 bytes.
	glVertexAttribPointer(coordsId, 3 /*3 elementos por vértice*/, GL_FLOAT/*do tipo float*/, GL_FALSE, 8 * sizeof(float) /*stride*/, (void*) 0);
	// Liga a localização do atributo 'vColors' dos shaders do 'programa', ao VBO e VAO (ativos).
	glVertexAttribPointer(coresId, 3 /*3 elementos por vértice*/, GL_FLOAT/*do tipo float*/, GL_FALSE, 8 * sizeof(float) /*stride*/, (void*) (3 * sizeof(float)));
	// Liga a localização do atributo 'vTextureCoords' dos shaders do 'programa', ao VBO e VAO (ativos).
	glVertexAttribPointer(texturaId, 2 /*2 elementos por vértice*/, GL_FLOAT/*do tipo float*/, GL_FALSE, 8 * sizeof(float) /*stride*/, (void*) ((3 + 3) * sizeof(float)));

	// Habitita o atributo com localização 'coresId' para o VAO ativo.
	glEnableVertexAttribArray(coordsId);
	// Habitita o atributo com localização 'coresId' para o VAO ativo.
	glEnableVertexAttribArray(coresId);
	// Habitita o atributo com localização 'texturaId' para o VAO ativo.
	glEnableVertexAttribArray(texturaId);

	// ****************************************************
	// Definir a janela de visualização (viewport)
	// ****************************************************

	glViewport(0, 0, WIDTH, HEIGHT);
}

void display(void) {
	static const GLfloat black[] = {
		0.0f, 0.0f, 0.0f, 0.0f
	};
	// Limpa o buffer de cor
	glClearBufferfv(GL_COLOR, 0, black);

	// Vincula (torna ativo) o VAO
	glBindVertexArray(VAO);
	// Envia comando para desenho de primitivas GL_TRIANGLES, que utilizará os dados do VAO vinculado.
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

int load_textures(vector<string> textureFiles) {
	GLuint textureName = 0;

	// Gera um nome de textura
	glGenTextures(1, &textureName);

	// Ativa a Unidade de Textura #0
	// A Unidade de Textura 0 está ativa por defeito.
	// Só uma Unidade de Textura pode estar ativa.
	glActiveTexture(GL_TEXTURE0);

	// Vincula esse nome de textura ao target GL_TEXTURE_2D_ARRAY da Unidade de Textura ativa.
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureName);

	// NOTA:
	// Num array de texturas, todas as texturas devem:
	// - ter a mesma resolução;
	// - possuir o mesmo número de níveis de mipmap; e,
	// - partilhar os mesmos parâmetros.

	// Define os parâmetros de filragem (wrapping e ajuste de tamanho)
	// para a textura que está vinculada ao target GL_TEXTURE_2D_ARRAY da Unidade de Textura ativa.
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Aloca memória para o array de texturas
	// Textura imutável, i.e., apenas é possível alterar a imagem.
	{
		// Leitura da resolução e número de canais da imagem.
		int width, height, nChannels;
		unsigned char *imageData = stbi_load(textureFiles[0].c_str(), &width, &height, &nChannels, 0);
		if (imageData) {
			stbi_image_free(imageData);

			// Alocação de memória
			glTexStorage3D(GL_TEXTURE_2D_ARRAY,
				1,					// Número de níveis de Mipmap para as texturas. 1 se não forem utilizados Mipmaps.
				nChannels == 4 ? GL_RGBA8 : GL_RGB8,	// Formato interno da imagem de textura
				width, height,		// width, height
				textureFiles.size()	// Dimensão do array de textura (i.e., número de imagens a colocar no array)
			);
		}
		else {
			cout << "Error loading texture!" << endl;
		}
	}

	int layer = 0;
	for (auto file : textureFiles) {
		// Leitura/descompressão do ficheiro com imagem de textura
		int width, height, nChannels;
		// Ativa a inversão vertical da imagem, aquando da sua leitura para memória.
		stbi_set_flip_vertically_on_load(true);
		unsigned char *imageData = stbi_load(file.c_str(), &width, &height, &nChannels, 0);
		if (imageData) {
			// Carrega os dados da imagem para o Objeto de Textura vinculado ao target GL_TEXTURE_2D_ARRAY
			// para a posição layer do array de texturas.
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
					0,					// Nível do Mipmap
					0, 0, layer++,		// xoffset, yoffset, zoffset (layer é o índice da imagem no array de texturas 2D)
					width, height, 1,	// width, height, depth
					nChannels == 4 ? GL_RGBA : GL_RGB,	// Formato da imagem
					GL_UNSIGNED_BYTE,	// Tipos dos dados da imagem
					imageData);			// Apontador para os dados da imagem de textura

			// Liberta a imagem da memória do CPU
			stbi_image_free(imageData);
		}
		else {
			cout << "Error loading texture!" << endl;
		}
	}

	return layer; // Retorna um valor inteiro que indica o número de texturas carregadas.
}

bool showNext(double secondsInterval) {
	bool show = false;
	std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
	static std::chrono::steady_clock::time_point previousTime = std::chrono::steady_clock::now();
	std::chrono::steady_clock::duration elapsedTime = currentTime - previousTime;

	// Tempo em segundos.
	double nseconds = double(elapsedTime.count()) * std::chrono::steady_clock::period::num / std::chrono::steady_clock::period::den;
	if (nseconds > secondsInterval) {
		show = true;
		previousTime = currentTime;
	}

	return show;
}
