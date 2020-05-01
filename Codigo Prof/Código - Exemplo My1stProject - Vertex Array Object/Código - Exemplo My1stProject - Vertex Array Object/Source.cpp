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

void print_gl_info();
void print_error(int error, const char *description);
void init_two_VBO_Packed_Arrays(void);
void init_single_VBO_Interleaved_Attributes(void);
void init_single_VBO_Interleaved_Arrays(void);
void display(void);

#define WIDTH 800
#define HEIGHT 600

#define NumVAOs 1
#define NumBuffers 2 // V�rtices, Cores
GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];
const GLuint NumVertices = 6;

int main(void) {
	GLFWwindow *window;

	glfwSetErrorCallback(print_error);

	if (!glfwInit()) return -1;

	window = glfwCreateWindow(WIDTH, HEIGHT, "GLSL - My1stProject - Vertex Array Object", NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);
	
	// Inicia o gestor de extens�es GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	print_gl_info();
	init_two_VBO_Packed_Arrays();
	//init_single_VBO_Interleaved_Attributes();
	//init_single_VBO_Interleaved_Arrays();

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		display();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void print_gl_info() {
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
	cout << "Higher supported version:\n" << glslversion << endl;
	GLint numglslversions;
	cout << "Other supported versions:\n";
	glGetIntegerv(GL_NUM_SHADING_LANGUAGE_VERSIONS, &numglslversions);
	for (int n = 0; n < numglslversions; n++) {
		cout << glGetStringi(GL_SHADING_LANGUAGE_VERSION, n) << endl;
	}
}

void print_error(int error, const char *description) {
	cout << description << endl;
}

void init_two_VBO_Packed_Arrays(void) {
	// ****************************************************
	// Criar arrays de dados na RAM
	// ****************************************************

	GLfloat vertices[NumVertices][2 /*xy*/] = {
		{ -0.90f, -0.90f }, { 0.85f, -0.90f }, { -0.90f,  0.85f }, // Tri�ngulo 1
		{  0.90f, -0.85f }, { 0.90f,  0.90f }, { -0.85f,  0.90f }  // Tri�ngulo 2
	};

	GLfloat cores[NumVertices][3 /*rgb*/] = {
		{ 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, // Tri�ngulo 1
		{ 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }  // Tri�ngulo 2
	};

	// ****************************************************
	// VAOs - Vertex Array Objects
	// ****************************************************

	// Gerar 'NumVAOs' nomes para VAOs.
	// Neste caso, como NumVAOS = 1, gera apenas 1 nome.
	glGenVertexArrays(NumVAOs, VAOs);
	// Faz bind do VAO, cujo nome est� definido em 'VAOs[0]', com o contexto do OpenGL.
	// Um VAO � criado no primero bind que lhe seja feito.
	// Este VAO passa a estar ativo at� que seja feito o bind a outro VAO, ou seja feito o bind com valor 0.
	glBindVertexArray(VAOs[0]);

	// ****************************************************
	// VBOs - Vertex Buffer Objects
	// ****************************************************
	
	// Gera 'NumBuffers' nomes para VBOs.
	// Neste caso, como NumBuffers = 2, gera 2 nomes
	// Esta fun��o pode ser chamada antes da cria��o de VAOs.
	glGenBuffers(NumBuffers, Buffers);
	// Para cada nome de VBO
	for (int i = 0; i < NumBuffers; i++) {
		// Faz bind do VBO ao buffer GL_ARRAY_BUFFER.
		// GL_ARRAY_BUFFER � utilizado para dados de atributos de v�rtices.
		// Um VBO � criado no primeiro bind que lhe seja feito.
		// Este VBO passa a estar ativo at� que seja feito o bind a outro VBO ou seja feito o bind com valor 0.
		// Para alocar a mem�ria para os BO, n�o � necess�rio que j� esteja vinculado um VAO.
		glBindBuffer(GL_ARRAY_BUFFER, Buffers[i]);
		if (i == 0) {
			// Inicializa o VBO (que est� ativo) com mem�ria de tamanho imut�vel.
			glBufferStorage(GL_ARRAY_BUFFER, sizeof(vertices) /*2 * 6 * sizeof(float)*/, vertices, 0);
			// Inicializa o VBO (que est� ativo) com mem�ria de tamanho mut�vel.
			//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		}
		else {
			// Inicializa o VBO (que est� ativo) com mem�ria de tamanho imut�vel.
			glBufferStorage(GL_ARRAY_BUFFER, sizeof(cores) /*3 * 6 * sizeof(float)*/, cores, 0);
			// Inicializa o VBO (que est� ativo) com mem�ria de tamanho mut�vel.
			//glBufferData(GL_ARRAY_BUFFER, sizeof(cores), vertices, GL_STATIC_DRAW);
		}
	}
	
	// ****************************************************
	// Shaders
	// ****************************************************

	ShaderInfo  shaders[] = {
		{ GL_VERTEX_SHADER,   "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};

	GLuint programa = LoadShaders(shaders);
	if (!programa) exit(EXIT_FAILURE);
	glUseProgram(programa);

	// ****************************************************
	// Ligar Atributos aos Shaders
	// ****************************************************

	// As localiza��es de atributos fazem parte do programa shader, e s�o completamente independentes do VAO.
	// Podemos, assim, obter as localiza��es de atributos sem que antes se tenha criado qualquer VAO.
	// ------------------------------------------------------------------------------------------------------
	// Obt�m a localiza��o do atributo 'vPosition' no 'programa'.
	//GLint coordsId = glGetAttribLocation(programa, "vPosition"); // Para vers�es anteriores � 4.3
	GLint coordsId = glGetProgramResourceLocation(programa, GL_PROGRAM_INPUT, "vPosition"); // Para vers�es >= 4.3
	// Obt�m a localiza��o do atributo 'vColors' no 'programa'.
	//GLint coresId = glGetAttribLocation(programa, "vColors"); // Para vers�es anteriores � 4.3
	GLint coresId = glGetProgramResourceLocation(programa, GL_PROGRAM_INPUT, "vColors"); // Para vers�es >= 4.3

	//glBindVertexArray(VAOs[0]); // N�o � necess�rio fazer o bind do VAO, pois ele j� � o que est� ativo no contexto do OpenGL.
	
	// Ativa o VBO 'Buffers[0]'.
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	// Liga a localiza��o do atributo 'vPosition' dos shaders do 'programa', ao VBO e VAO (ativos).
	// Aqui j� � necess�rio ter o VAO e o VBO vinculados ao contexto do OpenGL.
	// � neste ponto que o VBO fica associado ao VAO.
	// Especifica tamb�m como � que a informa��o do atributo 'coordsId' deve ser interpretada.
	// Neste caso, o atributo ir� receber, por v�rtice, 2 elementos do tipo float.
	glVertexAttribPointer(coordsId, 2 /*2 elementos por v�rtice*/, GL_FLOAT/*do tipo float*/, GL_FALSE, 0, nullptr);

	// Ativa o VBO 'Buffers[1]'.
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
	// Liga a localiza��o do atributo 'vColors' dos shaders do 'programa', ao VBO e VAO (ativos).
	// Aqui j� � necess�rio ter o VAO e o VBO vinculados ao contexto do OpenGL.
	// � neste ponto que o VBO fica associado ao VAO.
	// Especifica tamb�m como � que a informa��o do atributo 'coordsId' deve ser interpretada.
	// Neste caso, o atributo ir� receber, por v�rtice, 3 elementos do tipo float.
	glVertexAttribPointer(coresId, 3 /*3 elementos por v�rtice*/, GL_FLOAT/*do tipo float*/, GL_FALSE, 0, nullptr);
	
	// Habitita o atributo com localiza��o 'coresId' para o VAO ativo.
	// Os atributos de um VAO est�o desativados por defeito.
	glEnableVertexAttribArray(coordsId);
	// Habitita o atributo com localiza��o 'coresId' para o VAO ativo.
	// Os atributos de um VAO est�o desativados por defeito.
	glEnableVertexAttribArray(coresId);
}

void init_single_VBO_Interleaved_Attributes(void) {
	// ****************************************************
	// Criar array de dados na RAM
	// ****************************************************

	GLfloat verticesCores[NumVertices][2 /*xy*/ + 3 /*rgb*/] = {
		{ -0.90f, -0.90f, 1.0f, 0.0f, 0.0f }, // XYRGB
		{ 0.85f, -0.90f, 0.0f, 1.0f, 0.0f },
		{ -0.90f,  0.85f, 0.0f, 0.0f, 1.0f },
		{ 0.90f, -0.85f, 1.0f, 0.0f, 0.0f },
		{ 0.90f,  0.90f, 0.0f, 1.0f, 0.0f },
		{ -0.85f,  0.90f, 0.0f, 0.0f, 1.0f }
	};

	// ****************************************************
	// VAOs - Vertex Array Objects
	// ****************************************************

	// Gerar 'NumVAOs' nomes para VAOs.
	// Neste caso, como NumVAOS = 1, gera apenas 1 nome.
	glGenVertexArrays(NumVAOs, VAOs);
	// Faz bind do VAO, cujo nome est� definido em 'VAOs[0]', com o contexto do OpenGL.
	// Um VAO � criado no primero bind que lhe seja feito.
	// Este VAO passa a estar ativo at� que seja feito o bind a outro VAO, ou seja feito o bind com valor 0.
	glBindVertexArray(VAOs[0]);

	// ****************************************************
	// VBOs - Vertex Buffer Objects
	// ****************************************************

	// Gera nomes para VBOs.
	// Neste caso, gera apenas 1 nome
	// Esta fun��o pode ser chamada antes da cria��o de VAOs.
	glGenBuffers(1, Buffers);
	// Faz bind do VBO ao buffer GL_ARRAY_BUFFER.
	// GL_ARRAY_BUFFER � utilizado para dados de atributos de v�rtices.
	// Um VBO � criado no primeiro bind que lhe seja feito.
	// Este VBO passa a estar ativo at� que seja feito o bind a outro VBO ou seja feito o bind com valor 0.
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	// Inicializa o VBO (que est� ativo) com mem�ria de tamanho imut�vel.
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(verticesCores), verticesCores, 0);
	// Inicializa o VBO (que est� ativo) com mem�ria de tamanho mut�vel.
	//glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCores), verticesCores, GL_STATIC_DRAW);

	// ****************************************************
	// Shaders
	// ****************************************************

	ShaderInfo  shaders[] = {
		{ GL_VERTEX_SHADER,   "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};

	GLuint programa = LoadShaders(shaders);
	if (!programa) exit(EXIT_FAILURE);
	glUseProgram(programa);

	// ****************************************************
	// Ligar Atributos aos Shaders
	// ****************************************************

	// As localiza��es de atributos fazem parte do programa shader, e s�o completamente independentes do VAO.
	// Podemos, assim, obter as localiza��es de atributos sem que antes se tenha criado qualquer VAO.
	// ------------------------------------------------------------------------------------------------------
	// Obt�m a localiza��o do atributo 'vPosition' no 'programa'.
	//GLint coordsId = glGetAttribLocation(programa, "vPosition"); // Para vers�es anteriores � 4.3
	GLint coordsId = glGetProgramResourceLocation(programa, GL_PROGRAM_INPUT, "vPosition"); // Para vers�es >= 4.3
	// Obt�m a localiza��o do atributo 'vColors' no 'programa'.
	//GLint coresId = glGetAttribLocation(programa, "vColors"); // Para vers�es anteriores � 4.3
	GLint coresId = glGetProgramResourceLocation(programa, GL_PROGRAM_INPUT, "vColors"); // Para vers�es >= 4.3
	
	//glBindVertexArray(VAOs[0]); // N�o � necess�rio fazer o bind do VAO, pois ele j� � o que est� ativo no contexto do OpenGL.

	// Ativa o VBO 'Buffers[0]'.
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);

	// Liga a localiza��o do atributo 'vPosition' dos shaders do 'programa', ao VBO e VAO (ativos).
	// Especifica tamb�m como � que a informa��o do atributo 'coordsId' deve ser interpretada.
	// Neste caso, o atributo ir� receber, por v�rtice, 2 elementos do tipo float.
	glVertexAttribPointer(coordsId, 2 /*2 elementos por v�rtice*/, GL_FLOAT/*do tipo float*/, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *) 0);

	// Liga a localiza��o do atributo 'vColors' dos shaders do 'programa', ao VBO e VAO (ativos).
	// Especifica tamb�m como � que a informa��o do atributo 'coordsId' deve ser interpretada.
	// Neste caso, o atributo ir� receber, por v�rtice, 3 elementos do tipo float.
	glVertexAttribPointer(coresId, 3 /*3 elementos por v�rtice*/, GL_FLOAT/*do tipo float*/, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *) 8 /*2*sizeof(GLfloat)*/);

	// Habitita o atributo com localiza��o 'coresId' para o VAO ativo.
	// Os atributos de um VAO est�o desativados por defeito.
	glEnableVertexAttribArray(coordsId);
	// Habitita o atributo com localiza��o 'coresId' para o VAO ativo.
	// Os atributos de um VAO est�o desativados por defeito.
	glEnableVertexAttribArray(coresId);
}

void init_single_VBO_Interleaved_Arrays(void) {
	// ****************************************************
	// Criar array de dados na RAM
	// ****************************************************

	GLfloat verticesCores[(NumVertices * 2 /*xy*/) + (NumVertices * 3 /*rgb*/)] = {
		-0.90f, -0.90f, 0.85f, -0.90f, -0.90f,  0.85f, 0.90f, -0.85f, 0.90f,  0.90f, -0.85f,  0.90f, // XY
		1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f // RGB
	};

	// ****************************************************
	// VAOs - Vertex Array Objects
	// ****************************************************

	// Gerar 'NumVAOs' nomes para VAOs.
	// Neste caso, como NumVAOS = 1, gera apenas 1 nome.
	glGenVertexArrays(NumVAOs, VAOs);
	// Faz bind do VAO, cujo nome est� definido em 'VAOs[0]', com o contexto do OpenGL.
	// Um VAO � criado no primero bind que lhe seja feito.
	// Este VAO passa a estar ativo at� que seja feito o bind a outro VAO, ou seja feito o bind com valor 0.
	glBindVertexArray(VAOs[0]);

	// ****************************************************
	// VBOs - Vertex Buffer Objects
	// ****************************************************

	// Gera nomes para VBOs.
	// Neste caso, gera apenas 1 nome
	// Esta fun��o pode ser chamada antes da cria��o de VAOs.
	glGenBuffers(1, Buffers);
	// Faz bind do VBO ao buffer GL_ARRAY_BUFFER.
	// GL_ARRAY_BUFFER � utilizado para dados de atributos de v�rtices.
	// Um VBO � criado no primeiro bind que lhe seja feito.
	// Este VBO passa a estar ativo at� que seja feito o bind a outro VBO ou seja feito o bind com valor 0.
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	// Inicializa o VBO (que est� ativo) com mem�ria de tamanho imut�vel.
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(verticesCores), verticesCores, 0);
	// Inicializa o VBO (que est� ativo) com mem�ria de tamanho mut�vel.
	//glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCores), verticesCores, GL_STATIC_DRAW);

	// ****************************************************
	// Shaders
	// ****************************************************

	ShaderInfo  shaders[] = {
		{ GL_VERTEX_SHADER,   "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};

	GLuint programa = LoadShaders(shaders);
	if (!programa) exit(EXIT_FAILURE);
	glUseProgram(programa);

	// ****************************************************
	// Ligar Atributos aos Shaders
	// ****************************************************

	// As localiza��es de atributos fazem parte do programa shader, e s�o completamente independentes do VAO.
	// Podemos, assim, obter as localiza��es de atributos sem que antes se tenha criado qualquer VAO.
	// ------------------------------------------------------------------------------------------------------
	// Obt�m a localiza��o do atributo 'vPosition' no 'programa'.
	//GLint coordsId = glGetAttribLocation(programa, "vPosition"); // Para vers�es anteriores � 4.3
	GLint coordsId = glGetProgramResourceLocation(programa, GL_PROGRAM_INPUT, "vPosition"); // Para vers�es >= 4.3
	// Obt�m a localiza��o do atributo 'vColors' no 'programa'.
	//GLint coresId = glGetAttribLocation(programa, "vColors"); // Para vers�es anteriores � 4.3
	GLint coresId = glGetProgramResourceLocation(programa, GL_PROGRAM_INPUT, "vColors"); // Para vers�es >= 4.3
	
	//glBindVertexArray(VAOs[0]); // N�o � necess�rio fazer o bind do VAO, pois ele j� � o que est� ativo no contexto do OpenGL.
	
	// Ativa o VBO 'Buffers[0]'.
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);

	// Liga a localiza��o do atributo 'vPosition' dos shaders do 'programa', ao VBO e VAO (ativos).
	// Especifica tamb�m como � que a informa��o do atributo 'coordsId' deve ser interpretada.
	// Neste caso, o atributo ir� receber, por v�rtice, 2 elementos do tipo float.
	glVertexAttribPointer(coordsId, 2 /*2 elementos por v�rtice*/, GL_FLOAT/*do tipo float*/, GL_FALSE, 0, (GLvoid *) 0);

	// Liga a localiza��o do atributo 'vColors' dos shaders do 'programa', ao VBO e VAO (ativos).
	// Especifica tamb�m como � que a informa��o do atributo 'coordsId' deve ser interpretada.
	// Neste caso, o atributo ir� receber, por v�rtice, 3 elementos do tipo float.
	glVertexAttribPointer(coresId, 3 /*3 elementos por v�rtice*/, GL_FLOAT/*do tipo float*/, GL_FALSE, 0, (GLvoid *) (NumVertices * 2 * sizeof(GLfloat)));

	// Habitita o atributo com localiza��o 'coresId' para o VAO ativo.
	// Os atributos de um VAO est�o desativados por defeito.
	glEnableVertexAttribArray(coordsId);
	// Habitita o atributo com localiza��o 'coresId' para o VAO ativo.
	// Os atributos de um VAO est�o desativados por defeito.
	glEnableVertexAttribArray(coresId);
}

void display(void) {
	static const float black[] = {
		0.0f, 0.0f, 0.0f, 0.0f
	};

	// A fun��o glClearBufferfv(GL_COLOR, ...) � semelhante � fun��o glClearColor().
	glClearBufferfv(GL_COLOR, 0, black);

	// Vincula (torna ativo) o VAO
	glBindVertexArray(VAOs[0]);

	// Envia comando para desenho de primitivas GL_TRIANGLES, que utilizar� os dados do VAO vinculado.
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}
