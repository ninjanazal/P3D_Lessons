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
#define NumBuffers 2 // Vértices, Cores
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
	
	// Inicia o gestor de extensões GLEW
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
	// Informações de Uniform
	// ****************************************************

	GLint maxUniformLocations;

	glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &maxUniformLocations);

	cout << "\nUniform info:\n";
	// N.º máximo de localizações que o programador pode atribuir a uniforms.
	cout << "\tGL_MAX_UNIFORM_LOCATIONS = " << maxUniformLocations;
	cout << " -> Maximum uniform locations = " << maxUniformLocations << endl;

	// ****************************************************
	// Informações do Vertex Shader
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
	// N.º máximo de atributos de vértices genéricos vec4.
	cout << "\tGL_MAX_VERTEX_ATTRIBS = " << maxVertexAttribs;
	cout << " -> Maximum Vertex Shader input locations = " << maxVertexAttribs << endl;
	// N.º máximo de unidades de textura, acessíveis pelo vertex shader.
	cout << "\tGL_MAX_VERTEX_TEXTURE_IMAGE_UNITS = " << maxVertexTextureUnits << endl;
	// N.º máximo de valores (reais, inteiros ou booleanos) que podem ser armazenados numa variável uniform num vertex shader.
	cout << "\tGL_MAX_VERTEX_UNIFORM_COMPONENTS = " << maxVertexUniformComponents << endl;
	// N.º máximo de vec4 que podem ser armazenados numa variável uniform num vertex shader.
	cout << "\tGL_MAX_VERTEX_UNIFORM_VECTORS = " << maxVertexUniformsvec4 << endl;
	// N.º máximo de blocos uniform por vertex shader.
	cout << "\tGL_MAX_VERTEX_UNIFORM_BLOCKS = " << maxVertexUniformBlocks << endl;
	// N.º máximo de componentes de saída de um vertex shader.
	// Por exemplo, um vec3 tem 3 componentes. Um vec4 tem 4 componentes.
	cout << "\tGL_MAX_VERTEX_OUTPUT_COMPONENTS = " << maxVertexOutputComponents;
	cout << " -> Maximum Vertex Shader output locations = " << maxVertexOutputComponents / 4 /*vec4*/ << endl;

	// ****************************************************
	// Informações do Fragment Shader
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
	// N.º máximo de componentes de entrada em um fragment shader.
	// Por exemplo, um vec3 tem 3 componentes. Um vec4 tem 4 componentes.
	cout << "\tGL_MAX_FRAGMENT_INPUT_COMPONENTS = " << maxFragmentInputComponents;
	cout << " -> Maximum Fragment Shader input locations = " << maxFragmentInputComponents / 4 /*vec4*/ << endl;
	// N.º máximo de valores (reais, inteiros ou booleanos) que podem ser armazenados numa variável uniform num fragment shader.
	cout << "\tGL_MAX_FRAGMENT_UNIFORM_COMPONENTS = " << maxFragmentUniformComponents << endl;
	// N.º máximo de vec4 que podem ser armazenados numa variável uniform num fragment shader.
	cout << "\tGL_MAX_FRAGMENT_UNIFORM_VECTORS = " << maxFragmentUniformsvec4 << endl;
	// N.º máximo de blocos uniform por fragment shader.
	cout << "\tGL_MAX_FRAGMENT_UNIFORM_BLOCKS = " << maxFragmentUniformBlocks << endl;
	// N.º máximo de saídas de um fragment shader.
	cout << "\tGL_MAX_DRAW_BUFFERS = " << maxDrawBuffers;
	cout << " -> Maximum Fragment Shader output locations = " << maxDrawBuffers << endl;

	// ****************************************************
	// Informações de Textura
	// ****************************************************

	GLint maxTextureImageUnits, maxTextureSize;

	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureImageUnits);
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);

	cout << "\nTexture info:\n";
	// N.º máximo de unidades de textura de imagem suportadas e que estão acessíveis ao fragment shader.
	cout << "\tGL_MAX_TEXTURE_IMAGE_UNITS = " << maxTextureImageUnits << endl;
	// Dimensão, em pixéis, do maior tamanho de textura que o OpenGL pode suportar.
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
		{ -1.0f, -1.0f, 0.0f }, { 1.0f, -1.0f, 0.0f }, { 0.0f,  1.0f, 0.0f } // Triângulo
	};

	GLfloat cores[NumVertices][3] = {
		{ 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } // Triângulo
	};

	Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	View = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 5.0f),	// eye (posição da câmara). Câmara na posição (x=0, y=0, z=5). Nota que movo o mundo em sentido contrário.
		glm::vec3(0.0f, 0.0f, 0.0f),	// center (para onde está a "olhar")
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
	// Faz bind do VAO, cujo nome está definido em 'VAOs[0]', com o contexto do OpenGL.
	// Um VAO é criado no primero bind que lhe seja feito.
	// Este VAO passa a estar ativo até que seja feito o bind a outro VAO, ou seja feito o bind com valor 0.
	glBindVertexArray(VAOs[0]);

	// ****************************************************
	// VBOs - Vertex Buffer Objects
	// ****************************************************

	// Gera 'NumBuffers' nomes para VBOs.
	// Neste caso, como NumBuffers = 2, gera 2 nomes
	// Esta função pode ser chamada antes da criação de VAOs.
	glGenBuffers(NumBuffers, Buffers);
	// Para cada nome de VBO
	for (int i = 0; i < NumBuffers; i++) {
		// Faz bind do VBO ao buffer GL_ARRAY_BUFFER.
		// GL_ARRAY_BUFFER é utilizado para dados de atributos de vértices.
		// Um VBO é criado no primeiro bind que lhe seja feito.
		// Este VBO passa a estar ativo até que seja feito o bind a outro VBO ou seja feito o bind com valor 0.
		glBindBuffer(GL_ARRAY_BUFFER, Buffers[i]);
		if (i == 0) {
			// Inicializa o VBO (que está ativo) com memória de tamanho imutável.
			glBufferStorage(GL_ARRAY_BUFFER, sizeof(vertices) /*3 * 3 * sizeof(float)*/, vertices, 0);
			// Inicializa o VBO (que está ativo) com memória de tamanho mutável.
			//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		}
		else {
			// Inicializa o VBO (que está ativo) com memória de tamanho imutável.
			glBufferStorage(GL_ARRAY_BUFFER, sizeof(cores) /*3 * 3 * sizeof(float)*/, cores, 0);
			// Inicializa o VBO (que está ativo) com memória de tamanho mutável.
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

	// As localizações de atributos fazem parte do programa shader, e são completamente independentes do VAO.
	// Podemos, assim, obter as localizações de atributos sem que antes se tenha criado qualquer VAO.
	// ------------------------------------------------------------------------------------------------------
	// Obtém a localização do atributo 'vPosition' no 'programa'.
	//GLint coordsId = glGetAttribLocation(programa, "vPosition"); // Para versões anteriores à 4.3
	GLint coordsId = glGetProgramResourceLocation(programa, GL_PROGRAM_INPUT, "vPosition"); // Para versões >= 4.3
	// Obtém a localização do atributo 'vColors' no 'programa'.
	//GLint coresId = glGetAttribLocation(programa, "vColors"); // Para versões anteriores à 4.3
	GLint coresId = glGetProgramResourceLocation(programa, GL_PROGRAM_INPUT, "vColors"); // Para versões >= 4.3
	
	// Atribui valor ao uniform MVP
	GLint mvpId = glGetProgramResourceLocation(programa, GL_UNIFORM, "MVP");
	glProgramUniformMatrix4fv(programa, mvpId, 1, GL_FALSE, glm::value_ptr(ModelViewProjection));

	//glBindVertexArray(VAOs[0]); // Não é necessário fazer o bind do VAO, pois ele já é o que está ativo no contexto do OpenGL.

	// Ativa o VBO 'Buffers[0]'.
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	// Liga a localização do atributo 'vPosition' dos shaders do 'programa', ao VBO e VAO (ativos).
	// Especifica também como é que a informação do atributo 'coordsId' deve ser interpretada.
	// Neste caso, o atributo irá receber, por vértice, 3 elementos do tipo float.
	glVertexAttribPointer(coordsId, 3 /*3 elementos por vértice*/, GL_FLOAT/*do tipo float*/, GL_FALSE, 0, nullptr);

	// Ativa o VBO 'Buffers[1]'.
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
	// Liga a localização do atributo 'vColors' dos shaders do 'programa', ao VBO e VAO (ativos).
	// Especifica também como é que a informação do atributo 'coordsId' deve ser interpretada.
	// Neste caso, o atributo irá receber, por vértice, 3 elementos do tipo float.
	glVertexAttribPointer(coresId, 3 /*3 elementos por vértice*/, GL_FLOAT/*do tipo float*/, GL_FALSE, 0, nullptr);
	
	// Habitita o atributo com localização 'coresId' para o VAO ativo.
	// Os atributos de um VAO estão desativados por defeito.
	glEnableVertexAttribArray(coordsId);
	// Habitita o atributo com localização 'coresId' para o VAO ativo.
	// Os atributos de um VAO estão desativados por defeito.
	glEnableVertexAttribArray(coresId);

	// ****************************************************
	// Definir a janela de visualização (viewport)
	// ****************************************************

	glViewport(0, 0, WIDTH, HEIGHT);
}

void display(void) {
	static const float black[] = {
		0.0f, 0.0f, 0.0f, 0.0f
	};

	// A função glClearBufferfv(GL_COLOR, ...) é semelhante à função glClearColor().
	glClearBufferfv(GL_COLOR, 0, black);

	// Atualiza os dados do Uniform
	Model = glm::rotate(glm::mat4(1.0f), angle += 0.002f, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 ModelViewProjection = Projection * View * Model;
	GLint mvpId = glGetProgramResourceLocation(programa, GL_UNIFORM, "MVP");
	glProgramUniformMatrix4fv(programa, mvpId, 1, GL_FALSE, glm::value_ptr(ModelViewProjection));

	// Vincula (torna ativo) o VAO
	glBindVertexArray(VAOs[0]);
	// Envia comando para desenho de primitivas GL_TRIANGLES, que utilizará os dados do VAO vinculado.
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}
