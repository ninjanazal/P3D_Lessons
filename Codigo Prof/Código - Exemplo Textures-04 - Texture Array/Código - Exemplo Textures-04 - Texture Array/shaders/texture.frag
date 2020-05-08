#version 440 core

layout (location = 0) in vec3 color;
layout (location = 1) in vec2 textureCoord;

// binding = 0 faz com que 'textureArray' corresponda à Unidade de Textura 0.
// Se se definisse binding = 1, então 'textureArray' corresponderia à Unidade de Textura 1.
layout(binding = 0) uniform sampler2DArray textureArray;
layout(location = 0) uniform int nLayers;	// Número de texturas no array.
layout(location = 1) uniform int showLayer;	// Índice da textura que se pretende exibir.

// Se não fizessemos aqui o binding, teriamos de o efetuar obrigatoriamente no lado da aplicação OpenGL.
// Se o fizermos na aplicação OpenGL, esses comandos vão-se sobrepor ao que aqui for definido.
// Exemplo sem binding nem especificação da localização:
//uniform sampler2DArray textureArray;
//uniform int nLayers;
//uniform int showLayer;

layout (location = 0) out vec4 fColor; // Cor final do fragmento

void main()
{
	fColor = texture(textureArray, vec3(textureCoord, showLayer /* e.g., 1.ª imagem do array, i.e., imagem com índice 0 */));
	
	//// Cor final do fragmento é obtida pela mistura de todas as texturas inseridas no array de texturas 2D.
	//for(int layer = 1; layer<nLayers; layer++) {
	//	fColor = mix(fColor, texture(textureArray, vec3(textureCoord, layer)), 0.5);
	//}
}
