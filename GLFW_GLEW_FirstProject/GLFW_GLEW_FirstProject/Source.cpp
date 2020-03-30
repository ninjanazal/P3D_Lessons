// especifica as bibliotecas com que o programa devera linkar
#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <iostream>

#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")

int main(void) {
	// definiçao de um apontador do tipo glfwwindow
	GLFWwindow* window;

	// tamanho da janela
	int windowSize[] = { 800, 600 };
	// debug do tamanho da janela
	std::cout << "Janela do tamanho: " << windowSize[0] << "x" << windowSize[1] << std::endl;

	if (!glfwInit()) return -1;	// caso o glfw nao tenha sido iniciado
	// define a nova janela, com um tamanha e titulo
	window = glfwCreateWindow(windowSize[0], windowSize[1], "My 1st glfwWindow", NULL, NULL);

	// se a janela nao for iniciada, termina e retorna -1
	if (!window) { glfwTerminate(); return -1; }

	// cria a janela
	glfwMakeContextCurrent(window);
	std::cout << "Janela iniciada\n";	// debug que a janela foi criada


	// enquanto a janela nao receber um sinal de fecho
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);	// limpa 
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// fora do loop, a janela deve ser destruida
	glfwDestroyWindow(window);

	std::cout << "Janela Terminada\n ->Press any key\n";

	// termina a aplicaçao
	glfwTerminate();

	// aguarda input para nao fechar imediatamenta
	std::cin.get();
	return 0;
}