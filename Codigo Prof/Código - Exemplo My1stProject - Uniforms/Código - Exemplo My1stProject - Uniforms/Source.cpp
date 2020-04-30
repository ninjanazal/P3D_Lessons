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

void print_gl_info();
void print_error(int error, const char *description);
void init(void);
void display(void);

#define WIDTH 800
#define HEIGHT 600

#define NumVAOs 1
#define NumBuffers 2 // V�rtices, Cores
GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];
const GLuint NumVertices = 3;

GLuint programa;

glm::mat4 Model, View, Projection;
GLfloat angle = 0.0f;

int main(void) {
	GLFWwindow *window;

	glfwSetErrorCallback(print_error);

	if (!glfwInit()) return -1;

	window = glfwCreateWindow(WIDTH, HEIGHT, "GLSL - My1stProject - Uniforms", NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);
	
	// Inicia o gestor de extens�es GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	print_gl_info();
	init();

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
	cout << "Higher supported version:\n\t" << glslversion << endl;
	GLint numglslversions;
	cout << "Other supported versions:\n";
	glGetIntegerv(GL_NUM_SHADING_LANGUAGE_VERSIONS, &numglslversions);
	for (int n = 0; n < numglslversions; n++) {
		cout << '\t' << glGetStringi(GL_SHADING_LANGUAGE_VERSION, n) << endl;
	}

	// ****************************************************
	// Informa��es de Uniform
	// ****************************************************

	GLint maxUniformLocations;

	glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &maxUniformLocations);

	cout << "\nUniform info:\n";
	// N.� m�ximo de localiza��es que o programador pode atribuir a uniforms.
	cout << "\tGL_MAX_UNIFORM_LOCATIONS = " << maxUniformLocations;
	cout << " -> Maximum uniform locations = " << maxUniformLocations << endl;

	// ****************************************************
	// Informa��es do Vertex Shader
	// ****************************************************

	GLint maxVertexAttribs, maxVertexTextureUnits;
	GLint maxVertexUniformComponents, maxVertexUniformsvec4, maxVertexUniformBlocks;
	GLint maxVertexOutputComponents;

	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttribs);
	glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &maxVertexTextureUnits);
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &maxVertexUniformComponents);
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &maxVertexUniformsvec4);
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &maxVertexUniformBlocks);
	glGetIntegerv(GL_MAX_VERTEX_OUTPUT_COMPONENTS, &maxVertexOutputComponents);

	cout << "\nVertex Shader info:\n";
	// N.� m�ximo de atributos de v�rtices gen�ricos vec4.
	cout << "\tGL_MAX_VERTEX_ATTRIBS = " << maxVertexAttribs;
	cout << " -> Maximum Vertex Shader input locations = " << maxVertexAttribs << endl;
	// N.� m�ximo de unidades de textura, acess�veis pelo vertex shader.
	cout << "\tGL_MAX_VERTEX_TEXTURE_IMAGE_UNITS = " << maxVertexTextureUnits << endl;
	// N.� m�ximo de valores (reais, inteiros ou booleanos) que podem ser armazenados numa vari�vel uniform num vertex shader.
	cout << "\tGL_MAX_VERTEX_UNIFORM_COMPONENTS = " << maxVertexUniformComponents << endl;
	// N.� m�ximo de vec4 que podem ser armazenados numa vari�vel uniform num vertex shader.
	cout << "\tGL_MAX_VERTEX_UNIFORM_VECTORS = " << maxVertexUniformsvec4 << endl;
	// N.� m�ximo de blocos uniform por vertex shader.
	cout << "\tGL_MAX_VERTEX_UNIFORM_BLOCKS = " << maxVertexUniformBlocks << endl;
	// N.� m�ximo de componentes de sa�da de um vertex shader.
	// Por exemplo, um vec3 tem 3 componentes. Um vec4 tem 4 componentes.
	cout << "\tGL_MAX_VERTEX_OUTPUT_COMPONENTS = " << maxVertexOutputComponents;
	cout << " -> Maximum Vertex Shader output locations = " << maxVertexOutputComponents / 4 /*vec4*/ << endl;

	// ****************************************************
	// Informa��es do Fragment Shader
	// ****************************************************

	GLint maxFragmentInputComponents;
	GLint maxFragmentUniformComponents, maxFragmentUniformsvec4, maxFragmentUniformBlocks;
	GLint maxDrawBuffers;

	glGetIntegerv(GL_MAX_FRAGMENT_INPUT_COMPONENTS, &maxFragmentInputComponents);
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &maxFragmentUniformComponents);
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &maxFragmentUniformsvec4);
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &maxFragmentUniformBlocks);
	glGetIntegerv(GL_MAX_DRAW_BUFFERS, &maxDrawBuffers);

	cout << "\nFragment Shader info:\n";
	// N.� m�ximo de componentes de entrada em um fragment shader.
	// Por exemplo, um vec3 tem 3 componentes. Um vec4 tem 4 componentes.
	cout << "\tGL_MAX_FRAGMENT_INPUT_COMPONENTS = " << maxFragmentInputComponents;
	cout << " -> Maximum Fragment Shader input locations = " << maxFragmentInputComponents / 4 /*vec4*/ << endl;
	// N.� m�ximo de valores (reais, inteiros ou booleanos) que podem ser armazenados numa vari�vel uniform num fragment shader.
	cout << "\tGL_MAX_FRAGMENT_UNIFORM_COMPONENTS = " << maxFragmentUniformComponents << endl;
	// N.� m�ximo de vec4 que podem ser armazenados numa vari�vel uniform num fragment shader.
	cout << "\tGL_MAX_FRAGMENT_UNIFORM_VECTORS = " << maxFragmentUniformsvec4 << endl;
	// N.� m�ximo de blocos uniform por fragment shader.
	cout << "\tGL_MAX_FRAGMENT_UNIFORM_BLOCKS = " << maxFragmentUniformBlocks << endl;
	// N.� m�ximo de sa�das de um fragment shader.
	cout << "\tGL_MAX_DRAW_BUFFERS = " << maxDrawBuffers;
	cout << " -> Maximum Fragment Shader output locations = " << maxDrawBuffers << endl;

	// ****************************************************
	// Informa��es de Textura
	// ****************************************************

	GLint maxTextureImageUnits, maxTextureSize;

	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureImageUnits);
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);

	cout << "\nTexture info:\n";
	// N.� m�ximo de unidades de textura de imagem suportadas e que est�o acess�veis ao fragment shader.
	cout << "\tGL_MAX_TEXTURE_IMAGE_UNITS = " << maxTextureImageUnits << endl;
	// Dimens�o, em pix�is, do maior tamanho de textura que o OpenGL pode suportar.
	cout << "\tGL_MAX_TEXTURE_SIZE = " << maxTextureSize << endl;
}

void print_error(int error, const char *description) {
	cout << description << endl;
}

void init(void) {
	// ****************************************************
	// Criar arrays de dados na RAM
	// ****************************************************

	GLfloat vertices[NumVertices][3] = {
		{ -1.0f, -1.0f, 0.0f }, { 1.0f, -1.0f, 0.0f }, { 0.0f,  1.0f, 0.0f } // Tri�ngulo
	};

	GLfloat cores[NumVertices][3] = {
		{ 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } // Tri�ngulo
	};

	Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	View = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 5.0f),	// eye (posi��o da c�mara). C�mara na posi��o (x=0, y=0, z=5). Nota que movo o mundo em sentido contr�rio.
		glm::vec3(0.0f, 0.0f, 0.0f),	// center (para onde est� a "olhar")
		glm::vec3(0.0f, 1.0f, 0.0f)		// up
	);
	Model = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 ModelViewProjection = Projection * View * Model;

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
		glBindBuffer(GL_ARRAY_BUFFER, Buffers[i]);
		if (i == 0) {
			// Inicializa o VBO (que est� ativo) com mem�ria de tamanho imut�vel.
			glBufferStorage(GL_ARRAY_BUFFER, sizeof(vertices) /*3 * 3 * sizeof(float)*/, vertices, 0);
			// Inicializa o VBO (que est� ativo) com mem�ria de tamanho mut�vel.
			//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		}
		else {
			// Inicializa o VBO (que est� ativo) com mem�ria de tamanho imut�vel.
			glBufferStorage(GL_ARRAY_BUFFER, sizeof(cores) /*3 * 3 * sizeof(float)*/, cores, 0);
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

	programa = LoadShaders(shaders);
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
	
	// Atribui valor ao uniform MVP
	GLint mvpId = glGetProgramResourceLocation(programa, GL_UNIFORM, "MVP");
	glProgramUniformMatrix4fv(programa, mvpId, 1, GL_FALSE, glm::value_ptr(ModelViewProjection));

	//glBindVertexArray(VAOs[0]); // N�o � necess�rio fazer o bind do VAO, pois ele j� � o que est� ativo no contexto do OpenGL.

	// Ativa o VBO 'Buffers[0]'.
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	// Liga a localiza��o do atributo 'vPosition' dos shaders do 'programa', ao VBO e VAO (ativos).
	// Especifica tamb�m como � que a informa��o do atributo 'coordsId' deve ser interpretada.
	// Neste caso, o atributo ir� receber, por v�rtice, 3 elementos do tipo float.
	glVertexAttribPointer(coordsId, 3 /*3 elementos por v�rtice*/, GL_FLOAT/*do tipo float*/, GL_FALSE, 0, nullptr);

	// Ativa o VBO 'Buffers[1]'.
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
	// Liga a localiza��o do atributo 'vColors' dos shaders do 'programa', ao VBO e VAO (ativos).
	// Especifica tamb�m como � que a informa��o do atributo 'coordsId' deve ser interpretada.
	// Neste caso, o atributo ir� receber, por v�rtice, 3 elementos do tipo float.
	glVertexAttribPointer(coresId, 3 /*3 elementos por v�rtice*/, GL_FLOAT/*do tipo float*/, GL_FALSE, 0, nullptr);
	
	// Habitita o atributo com localiza��o 'coresId' para o VAO ativo.
	// Os atributos de um VAO est�o desativados por defeito.
	glEnableVertexAttribArray(coordsId);
	// Habitita o atributo com localiza��o 'coresId' para o VAO ativo.
	// Os atributos de um VAO est�o desativados por defeito.
	glEnableVertexAttribArray(coresId);

	// ****************************************************
	// Definir a janela de visualiza��o (viewport)
	// ****************************************************

	glViewport(0, 0, WIDTH, HEIGHT);
}

void display(void) {
	static const float black[] = {
		0.0f, 0.0f, 0.0f, 0.0f
	};

	// A fun��o glClearBufferfv(GL_COLOR, ...) � semelhante � fun��o glClearColor().
	glClearBufferfv(GL_COLOR, 0, black);

	// Atualiza os dados do Uniform
	Model = glm::rotate(glm::mat4(1.0f), angle += 0.002f, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 ModelViewProjection = Projection * View * Model;
	GLint mvpId = glGetProgramResourceLocation(programa, GL_UNIFORM, "MVP");
	glProgramUniformMatrix4fv(programa, mvpId, 1, GL_FALSE, glm::value_ptr(ModelViewProjection));

	// Vincula (torna ativo) o VAO
	glBindVertexArray(VAOs[0]);
	// Envia comando para desenho de primitivas GL_TRIANGLES, que utilizar� os dados do VAO vinculado.
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}
