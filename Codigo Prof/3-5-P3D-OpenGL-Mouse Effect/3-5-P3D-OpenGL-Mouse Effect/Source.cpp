#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")

#include <iostream>
using namespace std;

#define GLEW_STATIC
#include <GL\glew.h>

#define GLFW_USE_DWM_SWAP_INTERVAL
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "LoadShaders.h"

void print_error(int error, const char *description);
void init(void);
void display(double xpos, double ypos);

#define WIDTH 1920
#define HEIGHT 1080

#define NumVAOs 1
#define NumBuffers 1 // Vértices
GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];
const GLuint NumVertices = 6;

GLuint programa;

int main(void) {
	GLFWwindow *window;

	glfwSetErrorCallback(print_error);

	if (!glfwInit()) return -1;

	window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL - Mouse Effect", NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);

	// Inicia o gestor de extensões GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	init();

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		display(xpos, ypos);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void print_error(int error, const char *description) {
	cout << description << endl;
}

void init(void) {
	// ****************************************************
	// Criar arrays de dados na RAM
	// ****************************************************

	GLfloat vertices[NumVertices][3] = {
		{ -1.0f, -1.0f, 0.0f }, { 1.0f, -1.0f, 0.0f }, { -1.0f,  1.0f, 0.0f },	// Triângulo #1
		{ -1.0f,  1.0f, 0.0f }, { 1.0f, -1.0f, 0.0f }, {  1.0f,  1.0f, 0.0f }	// Triângulo #2
	};

	// ****************************************************
	// VAOs - Vertex Array Objects
	// ****************************************************

	// Gerar 'NumVAOs' nomes para VAOs.
	// Neste caso, como NumVAOS = 1, gera apenas 1 nome.
	glGenVertexArrays(NumVAOs, VAOs);
	// Faz bind do VAO, cujo nome está definido em 'VAOs[0]', com o contexto do OpenGL.
	// Um VAO é criado no primero bind que lhe seja feito.
	// Este VAO passa a estar ativo até que seja feito o bind a outro VAO, ou seja feito o bind com valor 0.
	glBindVertexArray(VAOs[0]);

	// ****************************************************
	// VBOs - Vertex Buffer Objects
	// ****************************************************

	// Gera 'NumBuffers' nomes para VBOs.
	// Neste caso, como NumBuffers = 1, gera 1 nome
	// Esta função pode ser chamada antes da criação de VAOs.
	glGenBuffers(NumBuffers, Buffers);
	// Para cada nome de VBO
	for (int i = 0; i < NumBuffers; i++) {
		// Faz bind do VBO ao buffer GL_ARRAY_BUFFER.
		// GL_ARRAY_BUFFER é utilizado para dados de atributos de vértices.
		// Um VBO é criado no primeiro bind que lhe seja feito.
		// Este VBO passa a estar ativo até que seja feito o bind a outro VBO ou seja feito o bind com valor 0.
		glBindBuffer(GL_ARRAY_BUFFER, Buffers[i]);
		
		// Inicializa o VBO (que está ativo) com dados imutáveis.
		glBufferStorage(GL_ARRAY_BUFFER, sizeof(vertices), vertices, 0);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	}

	// ****************************************************
	// Shaders
	// ****************************************************

	ShaderInfo  shaders[] = {
		{ GL_VERTEX_SHADER,   "mouseEffect.vert" },
		{ GL_FRAGMENT_SHADER, "mouseEffect.frag" },
		{ GL_NONE, NULL }
	};

	programa = LoadShaders(shaders);
	if (!programa) exit(EXIT_FAILURE);
	glUseProgram(programa);

	// ****************************************************
	// Ligar Atributos aos Shaders
	// ****************************************************

	// As localizações de atributos fazem parte do programa shader, e são completamente independentes do VAO.
	// Podemos, assim, obter as localizações de atributos sem que antes se tenha criado qualquer VAO.
	// ------------------------------------------------------------------------------------------------------
	// Obtém a localização do atributo 'vPosition' no 'programa'.
	//GLint coordsId = glGetAttribLocation(programa, "vPosition"); // Para versões anteriores à 4.3
	GLint coordsId = glGetProgramResourceLocation(programa, GL_PROGRAM_INPUT, "vPosition"); // Para versões >= 4.3

	//glBindVertexArray(VAOs[0]); // Não é necessário fazer o bind do VAO, pois ele já é o que está ativo no contexto do OpenGL.

	// Ativa o VBO 'Buffers[0]'.
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	// Liga a localização do atributo 'vPosition' dos shaders do 'programa', ao VBO e VAO (ativos).
	// Especifica também como é que a informação do atributo 'coordsId' deve ser interpretada.
	// Neste caso, o atributo irá receber, por vértice, 3 elementos do tipo float.
	glVertexAttribPointer(coordsId, 3 /*3 elementos por vértice*/, GL_FLOAT/*do tipo float*/, GL_FALSE, 0, nullptr);

	// Habitita o atributo com localização 'coresId' para o VAO ativo.
	// Os atributos de um VAO estão desativados por defeito.
	glEnableVertexAttribArray(coordsId);

	// Atualiza os dados do Uniform
	GLint mousePosId = glGetProgramResourceLocation(programa, GL_UNIFORM, "uMousePos");
	float pos[2] = { 0.0f, 0.0f };
	glProgramUniform2fv(programa, mousePosId, 1, pos);

	GLint resolutionId = glGetProgramResourceLocation(programa, GL_UNIFORM, "uResolution");
	float res[2] = { (float)WIDTH, (float)HEIGHT };
	glProgramUniform2fv(programa, resolutionId, 1, res);

	// ****************************************************
	// Definir a janela de visualização (viewport)
	// ****************************************************

	glViewport(0, 0, WIDTH, HEIGHT);
}

void display(double xpos, double ypos) {
	static const float black[] = {
		0.0f, 0.0f, 0.0f, 0.0f
	};
	
	// A função glClearBufferfv(GL_COLOR, ...) é semelhante à função glClearColor().
	glClearBufferfv(GL_COLOR, 0, black);

	// Atualiza os dados do Uniform
	GLint mousePosId = glGetProgramResourceLocation(programa, GL_UNIFORM, "uMousePos");
	float pos[2] = { (float)xpos, (float)ypos };
	glProgramUniform2fv(programa, mousePosId, 1, pos);

	// Vincula (torna ativo) o VAO
	glBindVertexArray(VAOs[0]);
	// Envia comando para desenho de primitivas GL_TRIANGLES, que utilizará os dados do VAO vinculado.
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}
