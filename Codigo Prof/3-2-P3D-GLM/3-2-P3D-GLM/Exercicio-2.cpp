#include <glm/glm.hpp> // vec3, vec4, ivec4, mat4
#include <glm/gtc/matrix_transform.hpp> // translate, rotate, scale, perspective
#include <glm/gtc/type_ptr.hpp> // value_ptr

#include <iostream> // C++

int main(void)
{
	glm::vec3 point[3] = {
		glm::vec3(0.5f,  -0.5f, 0.0f),
		glm::vec3(0.0f,   0.5f, 0.0f),
		glm::vec3(-0.5f, -0.5f, 0.0f)
	};
	
	// Model
	glm::mat4 model = glm::mat4(1.0f);

	// Partindo da matriz identidade, move-se o sistema de eixos para o ponto (0, 0, -10)
	model = glm::translate(
		model,
		glm::vec3(0.0f, 0.0f, -10.0f)	// Translação
	);
	
	// View
	glm::mat4 view = glm::lookAt(
		glm::vec3(0.0f, 0.0f,  0.0f),	// Posição da câmara no mundo
		glm::vec3(0.0f, 0.0f, -1.0f),	// Direção para a qual a câmara esta apontada
		glm::vec3(0.0f, 1.0f,  0.0f)	// Vector vertical
	);
	
	// Projection
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 100.0f);

	// MVP
	glm::mat4 mvp = projection * view * model;
	
	for (int i = 0; i < 3; i++) {
		glm::vec4 vertex = glm::vec4(point[i], 1.0f);
		glm::vec3 transformedPoint = mvp * vertex;
		std::cout << "Point:\n" 
			<< "x=" << transformedPoint.x
			<< " y=" << transformedPoint.y
			<< " z=" << transformedPoint.z << std::endl;
	}

	return 0;
}