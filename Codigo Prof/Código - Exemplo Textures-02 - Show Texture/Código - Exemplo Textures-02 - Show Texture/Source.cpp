#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")


#include <iostream>
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
void load_texture(void);
void init(void);
void display(void);


#define WIDTH 800
#define HEIGHT 600


GLuint VAO;
GLuint Buffer;
const GLuint NumVertices = 3;
GLuint programa;


int main(void) {
	GLFWwindow *window;

	glfwSetErrorCallback(print_error);

	if (!glfwInit()) return -1;

	window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL - Show Texture", NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);
	
	// Inicia o gestor de extens�es GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	print_gl_info();

	load_texture();

	init();

	// Indica��o da Unidade de Textura a ligar ao sampler 'TexSampler'.
	//glUniform1i(glGetUniformLocation(programa, "TexSampler"), 0 /* Unidade de Textura #0 */);
	GLint locationTexSampler = glGetProgramResourceLocation(programa, GL_UNIFORM, "TexSampler");
	glProgramUniform1i(programa, locationTexSampler, 0 /* Unidade de Textura #0 */);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

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
		// Posi��es			 // Cores			// Coordenadas de textura
		-0.5f, -0.5f, 0.0f,	 1.0f, 0.0f, 0.0f,	0.0f, 0.0f,	// bottom left
		0.5f,  -0.5f, 0.0f,	 0.0f, 1.0f, 0.0f,	1.0f, 0.0f,	// bottom right
		0.0f,   0.5f, 0.0f,	 0.0f, 0.0f, 1.0f,	0.5f, 1.0f	// top
	};

	// ****************************************************
	// VAOs - Vertex Array Objects
	// ****************************************************

	// Gerar nomes para VAOs.
	// Neste caso gera apenas 1 nome.
	glGenVertexArrays(1, &VAO);
	// Faz bind do VAO, cujo nome est� definido em 'VAO', com o contexto do OpenGL.
	// Um VAO � criado no primero bind que lhe seja feito.
	// Este VAO passa a estar ativo at� que seja feito o bind a outro VAO, ou seja feito o bind com valor 0.
	glBindVertexArray(VAO);

	// ****************************************************
	// VBOs - Vertex Buffer Objects
	// ****************************************************

	// Gera 'NumBuffers' nomes para VBOs.
	// Neste caso gera 1 nome
	// Esta fun��o pode ser chamada antes da cria��o de VAOs.
	glGenBuffers(1, &Buffer);
	glBindBuffer(GL_ARRAY_BUFFER, Buffer);
	// Inicializa o VBO (que est� ativo) com dados imut�veis.
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

	// Obt�m a localiza��o do atributo 'vPosition' no 'programa'.
	GLint coordsId = glGetProgramResourceLocation(programa, GL_PROGRAM_INPUT, "vPosition");
	// Obt�m a localiza��o do atributo 'vColors' no 'programa'.
	GLint coresId = glGetProgramResourceLocation(programa, GL_PROGRAM_INPUT, "vColors");
	// Obt�m a localiza��o do atributo 'vTextureCoords' no 'programa'.
	GLint texturaId = glGetProgramResourceLocation(programa, GL_PROGRAM_INPUT, "vTextureCoords");
	
	// Ativa o VBO 'Buffer'.
	glBindBuffer(GL_ARRAY_BUFFER, Buffer);
	// Liga a localiza��o do atributo 'vPosition' dos shaders do 'programa', ao VBO e VAO (ativos).
	// Especifica tamb�m como � que a informa��o do atributo 'coordsId' deve ser interpretada.
	// Neste caso, o atributo ir� receber, por v�rtice, 3 elementos do tipo float. Stride de 8 floats e offset de 0 bytes.
	glVertexAttribPointer(coordsId, 3 /*3 elementos por v�rtice*/, GL_FLOAT/*do tipo float*/, GL_FALSE, 8 * sizeof(float) /*stride*/, (void*) 0);
	// Liga a localiza��o do atributo 'vColors' dos shaders do 'programa', ao VBO e VAO (ativos).
	glVertexAttribPointer(coresId, 3 /*3 elementos por v�rtice*/, GL_FLOAT/*do tipo float*/, GL_FALSE, 8 * sizeof(float) /*stride*/, (void*) (3 * sizeof(float)));
	// Liga a localiza��o do atributo 'vTextureCoords' dos shaders do 'programa', ao VBO e VAO (ativos).
	glVertexAttribPointer(texturaId, 2 /*2 elementos por v�rtice*/, GL_FLOAT/*do tipo float*/, GL_FALSE, 8 * sizeof(float) /*stride*/, (void*) ((3 + 3) * sizeof(float)));

	// Habitita o atributo com localiza��o 'coresId' para o VAO ativo.
	glEnableVertexAttribArray(coordsId);
	// Habitita o atributo com localiza��o 'coresId' para o VAO ativo.
	glEnableVertexAttribArray(coresId);
	// Habitita o atributo com localiza��o 'texturaId' para o VAO ativo.
	glEnableVertexAttribArray(texturaId);

	// ****************************************************
	// Definir a janela de visualiza��o (viewport)
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
	// Envia comando para desenho de primitivas GL_TRIANGLES, que utilizar� os dados do VAO vinculado.
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void load_texture(void) {
	GLuint textureName = 0;

	// Gera um nome de textura
	glGenTextures(1, &textureName);

	// Ativa a Unidade de Textura #0
	// A Unidade de Textura 0 j� est� ativa por defeito.
	// S� uma Unidade de Textura est� ativa a cada momento.
	glActiveTexture(GL_TEXTURE0);

	// Vincula esse nome de textura ao target GL_TEXTURE_2D da Unidade de Textura ativa.
	glBindTexture(GL_TEXTURE_2D, textureName);

	// Define os par�metros de filtragem (wrapping e ajuste de tamanho)
	// para a textura que est� vinculada ao target GL_TEXTURE_2D da Unidade de Textura ativa.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Leitura/descompress�o do ficheiro com imagem de textura
	int width, height, nChannels;
	// Ativa a invers�o vertical da imagem, aquando da sua leitura para mem�ria.
	stbi_set_flip_vertically_on_load(true);
	// Leitura da imagem para mem�ria do CPU
	unsigned char *imageData = stbi_load("textures/StoneWall.gif", &width, &height, &nChannels, 0);
	if (imageData) {
		// Carrega os dados da imagem para o Objeto de Textura vinculado ao target GL_TEXTURE_2D da Unidade de Textura ativa.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, nChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, imageData);
		
		// Gera o Mipmap para essa textura
		glGenerateMipmap(GL_TEXTURE_2D);

		// Liberta a imagem da mem�ria do CPU
		stbi_image_free(imageData);
	}
	else {
		cout << "Error loading texture!" << endl;
	}
}
