#version 440 core

layout (location = 0) in vec3 color;
layout (location = 1) in vec2 textureCoord;

// binding = 0 faz com que 'textureArray' corresponda � Unidade de Textura 0.
// Se se definisse binding = 1, ent�o 'textureArray' corresponderia � Unidade de Textura 1.
layout(binding = 0) uniform sampler2DArray textureArray;
layout(location = 0) uniform int nLayers;	// N�mero de texturas no array.
layout(location = 1) uniform int showLayer;	// �ndice da textura que se pretende exibir.

// Se n�o fizessemos aqui o binding, teriamos de o efetuar obrigatoriamente no lado da aplica��o OpenGL.
// Se o fizermos na aplica��o OpenGL, esses comandos v�o-se sobrepor ao que aqui for definido.
// Exemplo sem binding nem especifica��o da localiza��o:
//uniform sampler2DArray textureArray;
//uniform int nLayers;
//uniform int showLayer;

layout (location = 0) out vec4 fColor; // Cor final do fragmento

void main()
{
	fColor = texture(textureArray, vec3(textureCoord, showLayer /* e.g., 1.� imagem do array, i.e., imagem com �ndice 0 */));
	
	//// Cor final do fragmento � obtida pela mistura de todas as texturas inseridas no array de texturas 2D.
	//for(int layer = 1; layer<nLayers; layer++) {
	//	fColor = mix(fColor, texture(textureArray, vec3(textureCoord, layer)), 0.5);
	//}
}
