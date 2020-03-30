// Tipos de dados:
// vec3, vec4, ivec4, mat4, entre outros
#include <glm/glm.hpp>
// Funçoes de matrizes de transformaçao:
//translate, rotate, scale, frustum, perspective, orth, entre outras.
#include <glm/gtc/matrix_transform.hpp>
// Funçoes que manipulam a intereçao entre apontadores e tipos de vetores e matrizes
#include <glm/gtc/type_ptr.hpp>

// c++
#include <iostream>

int main(void) {
	// resoluçao da janela da aplicaçao
	int with = 800, height = 600;

	// vertices que formam o triangulo
	glm::vec3 vertex[] = {
		glm::vec3(0.5f,-0.5f,0.0f),
		glm::vec3(0.0f,0.5f,0.0f),
		glm::vec3(-0.5f,-0.5f,0.0f)
	};

	// *****************************
	// matriz de projeçao
	glm::mat4 Projection = glm::frustum(-2.0f, 2.0f, -1.5f, 1.5f, 5.0f, 20.0f);

	// *******************************************************
	// matriz de visualizaçao
	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 0, 0),		// eye
		glm::vec3(0, 0, -1),	// center
		glm::vec3(0, 1, 0)		// head up
	);

	// ************************************
	// Matriz de visualizaçao (Camara)
	glm::mat4 Model = glm::mat4(1.0f);

	// translaçao de 10 unidades no eixos dos z negativo
	Model = glm::translate(Model, glm::vec3(0.0f, 0.0f, -10.0f));

	// **************************
	// Matriz de modelaçao, visualizaçao, projeçao
	glm::mat4 MVP = Projection * View * Model;

	// ***************************************************
	// Imprimir coordenadas de recorte
	for (int i = 0; i < 3; i++) {
		glm::vec4 position = MVP * glm::vec4((glm::vec3)vertex[i], 1.0f);

		// imprime as coordenada resultantes na consola
		std::cout << "Coordenadas de recorte do vertex" << i << ":\n";
		std::cout << "-> Xclip= " << position.x <<
			"\n-> Yclip= " << position.y <<
			"\n-> Zclip= " << position.z <<
			"\n-> Wclip= " << position.w << std::endl;
	}

	// aguarda input do utilizador
	std::cin.get();
	return 0;
}