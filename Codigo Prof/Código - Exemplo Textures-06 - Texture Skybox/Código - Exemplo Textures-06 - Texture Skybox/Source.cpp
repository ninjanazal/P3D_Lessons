#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")


#include <iostream>
#include <string>
#include <vector>
using namespace std;

#define GLEW_STATIC
#include <GL\glew.h>

#define GLFW_USE_DWM_SWAP_INTERVAL
#include <GLFW\glfw3.h>

#include "LoadShaders.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>


void print_error(int error, const char *description);
void print_gl_info(void);
void load_textures(vector<string> textureFiles);
void init(void);
void display(void);


#define WIDTH 800
#define HEIGHT 600


GLuint VAO;
GLuint Buffer;
const GLuint NumVertices = 6 * 2 * 3; // 6 faces * 2 triângulos/face * 3 vértices/triângulo

GLuint programa;

glm::mat4 Model, View, Projection;
GLfloat angle = 0.0f;


int main(void) {
	GLFWwindow *window;

	glfwSetErrorCallback(print_error);

	if (!glfwInit()) return -1;

	window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL - Texture Skybox", NULL, NULL);
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
		"textures/skyboxRight.jpg",
		"textures/skyboxLeft.jpg",
		"textures/skyboxUp.jpg",
		"textures/skyboxDown.jpg",
		"textures/skyboxFront.jpg",
		"textures/skyboxBack.jpg"
	};
	load_textures(textureFiles);

	init();

	// Indicação da Unidade de Textura a ligar ao sampler 'skybox'.
	GLint location_textureArray = glGetProgramResourceLocation(programa, GL_UNIFORM, "skybox");
	glProgramUniform1i(programa, location_textureArray, 0 /* Unidade de Textura #0 */);

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
	
	GLfloat verticesSkybox[NumVertices * 3] = {
		// Posições
		 1.0f, -1.0f, -1.0f,     1.0f, -1.0f,  1.0f,     1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,     1.0f,  1.0f, -1.0f,     1.0f, -1.0f, -1.0f,
		 
		-1.0f, -1.0f,  1.0f,    -1.0f, -1.0f, -1.0f,    -1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,    -1.0f,  1.0f,  1.0f,    -1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,     1.0f,  1.0f, -1.0f,     1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,    -1.0f,  1.0f,  1.0f,    -1.0f,  1.0f, -1.0f,

	    -1.0f, -1.0f, -1.0f,     -1.0f, -1.0f,  1.0f,    1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,     -1.0f, -1.0f,  1.0f,    1.0f, -1.0f,  1.0f,

		-1.0f, -1.0f,  1.0f,    -1.0f,  1.0f,  1.0f,     1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,     1.0f, -1.0f,  1.0f,    -1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,    -1.0f, -1.0f, -1.0f,     1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,     1.0f,  1.0f, -1.0f,    -1.0f,  1.0f, -1.0f
	};

	Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	View = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 5.0f),	// eye (posição da câmara).
		glm::vec3(0.0f, 0.0f, 0.0f),	// center (para onde está a "olhar")
		glm::vec3(0.0f, 1.0f, 0.0f)		// up
	);
	Model = glm::mat4(1.0f);
	glm::mat4 ModelViewProjection = Projection * View * Model;

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
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(verticesSkybox), verticesSkybox, 0);
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
	
	// Ativa o VBO 'Buffer'.
	glBindBuffer(GL_ARRAY_BUFFER, Buffer);
	// Liga a localização do atributo 'vPosition' dos shaders do 'programa', ao VBO e VAO (ativos).
	// Especifica também como é que a informação do atributo 'coordsId' deve ser interpretada.
	// Neste caso, o atributo irá receber, por vértice, 3 elementos do tipo float. Stride de 3 floats e offset de 0 bytes.
	glVertexAttribPointer(coordsId, 3 /*3 elementos por vértice*/, GL_FLOAT/*do tipo float*/, GL_FALSE, 3 * sizeof(float) /*stride*/, (void*) 0);

	// Habitita o atributo com localização 'coresId' para o VAO ativo.
	glEnableVertexAttribArray(coordsId);

	// Atribui valor ao uniform MVP
	GLint mvpId = glGetProgramResourceLocation(programa, GL_UNIFORM, "MVP");
	glProgramUniformMatrix4fv(programa, mvpId, 1, GL_FALSE, glm::value_ptr(ModelViewProjection));

	// ****************************************************
	// Definir a janela de visualização (viewport)
	// ****************************************************

	glViewport(0, 0, WIDTH, HEIGHT);

	// ****************************************************
	// Outros parâmetros do OpenGL
	// ****************************************************
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE); // Por defeito está desativado
	glCullFace(GL_BACK); // GL_FRONT, [GL_BACK], GL_FRONT_AND_BACK
}

void display(void) {
	static const GLfloat black[] = {
		0.0f, 0.0f, 0.0f, 0.0f
	};
	// Limpa o buffer de cor
	glClearBufferfv(GL_COLOR, 0, black);

	glClear(GL_DEPTH_BUFFER_BIT);

	// Atualiza os dados do Uniform
	Model = glm::mat4(1.0f);
	View = glm::rotate(glm::mat4(1.0f), angle += 0.0002f, glm::vec3(0.0f, 1.0f, 0.0f));
	View = glm::mat4(glm::mat3(View)); // Poderia colocar apenas 'glm::mat3(View);', pois seria criada uma matriz 4x4 a atribuir a 'View'.
	glm::mat4 ModelViewProjection = Projection * View * Model;
	GLint mvpId = glGetProgramResourceLocation(programa, GL_UNIFORM, "MVP");
	glProgramUniformMatrix4fv(programa, mvpId, 1, GL_FALSE, glm::value_ptr(ModelViewProjection));

	// Vincula (torna ativo) o VAO
	glBindVertexArray(VAO);
	// Envia comando para desenho de primitivas GL_TRIANGLES, que utilizará os dados do VAO vinculado.
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void load_textures(vector<string> textureFiles) {
	GLuint textureName = 0;

	// Gera um nome de textura
	glGenTextures(1, &textureName);

	// Ativa a Unidade de Textura #0
	// A Unidade de Textura 0 está ativa por defeito.
	// Só uma Unidade de Textura pode estar ativa.
	glActiveTexture(GL_TEXTURE0);

	// Vincula esse nome de textura ao target GL_TEXTURE_CUBE_MAP da Unidade de Textura ativa.
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureName);

	// NOTA:
	// Num cube map de texturas, todas as texturas devem:
	// - ter a mesma resolução;
	// - possuir o mesmo número de níveis de mipmap; e,
	// - partilhar os mesmos parâmetros.

	// Define os parâmetros de filtragem (wrapping e ajuste de tamanho)
	// para a textura que está vinculada ao target GL_TEXTURE_CUBE_MAP da Unidade de Textura ativa.
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Ativa a inversão vertical da imagem, aquando da sua leitura para memória.
	stbi_set_flip_vertically_on_load(true);

	// Para cada face do cubo
	GLint face = 0;

	for (auto file : textureFiles) {
		// Leitura/descompressão do ficheiro com imagem de textura
		int width, height, nChannels;
		unsigned char *imageData = stbi_load(file.c_str(), &width, &height, &nChannels, 0);
		if (imageData) {
			// Carrega os dados da imagem para o Objeto de Textura vinculado ao target da face
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
				0,					// Nível do Mipmap
				GL_RGB,				// Formato interno do OpenGL
				width, height,		// width, height
				0,					// border
				nChannels == 4 ? GL_RGBA : GL_RGB,	// Formato da imagem
				GL_UNSIGNED_BYTE,	// Tipos dos dados da imagem
				imageData);			// Apontador para os dados da imagem de textura

			face++;

			// Liberta a imagem da memória do CPU
			stbi_image_free(imageData);
		}
		else {
			cout << "Error loading texture!" << endl;
		}
	}
}
