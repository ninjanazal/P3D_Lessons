// *****************************************************************************************************
// GLM 0.9.8
// https://glm.g-truc.net/0.9.8/api/modules.html
// *****************************************************************************************************

// Tipos de dados:
// vec3, vec4, ivec4, mat4, entre outros.
#include <glm/glm.hpp>
// Funções de matrizes de transformação:
// translate, rotate, scale, frustum, perspective, ortho, entre outras.
#include <glm/gtc/matrix_transform.hpp>
// Funções que manipulam a interação entre apontadores e tipos de vetores e matrizes:
// value_ptr
#include <glm/gtc/type_ptr.hpp>

// C++
#include <iostream>

int main(void)
{
	// Vértices na coodenada (1, 0, 0)
	glm::vec3 vertex = glm::vec3(1.0f, 0.0f, 0.0f);

	// Matriz de Modelação: Matriz Identidade (o modelo inicia na origem)
	glm::mat4 Model = glm::mat4(1.0f);

	// Rotação de 45 graus no eixos dos z
	Model = glm::rotate(Model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	// Translação de 10 unidades no eixos dos x
	Model = glm::translate(Model, glm::vec3(10.0f, 0.0f, 0.0f));

	vertex = Model * glm::vec4(vertex, 1.0f);

	std::cout << "(" << vertex.x << "," << vertex.y << "," << vertex.z << ")" << std::endl;

	return 0;
}