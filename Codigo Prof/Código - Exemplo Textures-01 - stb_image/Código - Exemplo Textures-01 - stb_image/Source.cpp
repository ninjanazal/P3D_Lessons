#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")

#include <iostream>
#include <string>
using namespace std;

#define GLEW_STATIC
#include <GL\glew.h>

#define GLFW_USE_DWM_SWAP_INTERVAL
#include <GLFW\glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#define WIDTH 800
#define HEIGHT 600


int main(void) {
	GLFWwindow *window;

	if (!glfwInit()) return -1;

	window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL - stb_image", NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);

	// Inicia o gestor de extensões GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	int w, h, c;
	string imagesFilePath[] {
		"textures/Coin.tga",
		"textures/MappingBuilding.jpg",
		"textures/MetalBridge.jpg",
		"textures/Normal.jpg", // Gray (1 channel)
		"textures/StoneWall.gif",
		"textures/LensFlareTransparent.png",
		"textures/LensFlareTransparent.tga"
	};
	
	for (string file : imagesFilePath) {
		unsigned char *image = (unsigned char *)stbi_load(file.c_str(), &w, &h, &c, 0);
		stbi_image_free(image);
		cout << "FILE: " << file << endl;
		cout << "\twidth: " << w << "\theight: " << h << "\tchannels: " << c << endl;
	}

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
