#version 440 core

layout (location = 0) in vec3 color;			// Cor do vértice
layout (location = 1) in vec2 textureCoord;		// Coordenada de textura do vértice

uniform sampler2D TexSampler1;					// Sampler de uma textura
uniform sampler2D TexSampler2;					// Sampler de uma textura

layout (location = 0) out vec4 fColor;			// Cor final do fragmento

#define _D_EXEMPLO_1	// Comentar esta linha para executar o segundo exemplo.

void main()
{
	vec4 tex1 = texture(TexSampler1, textureCoord);
	vec4 tex2 = texture(TexSampler2, textureCoord);

#ifdef _D_EXEMPLO_1
	// Cor final do fragmento é obtida pela:
	// - mistura de duas texturas; e,
	// - da cor atribuída ao fragmento.
	fColor = mix(tex1, tex2, 0.5) * vec4(color, 1.0);
#else
	vec3 white = vec3(0.9, 0.9, 0.9);
	bvec3 toMixBorderVec3 = greaterThan(tex2.rgb, white);	// Gera um vetor booleano, com o resultado da comparação de cada elemento.
	// Funções de comparação de vetores (estas funções retornam vetores booleanos):
	// bvec greaterThan(vec x, vec y);
	// bvec lessThan(vec x, vec y);
	// bvec greaterThanEqual(vec x, vec y);
	// bvec lessThanEqual(vec x, vec y);
	// bvec equal(vec x, vec y);
	// bvec notEqual(vec x, vec y);
	bool toMixBorder = all(toMixBorderVec3);				// Toma valor 'true' se todos os elementos do vetor forem 'true'.
	// Funções de booleanos em um vetor (estas funções retornam um booleano):
	// bool all(bvec x);
	// bool any(bvec x);
	// Função que inverte um vetor booleano (esta função retorna um vetor booleano):
	// bvec not(bvec x);

	// Se a opacidade do texel da textura referenciada pelo sampler TexSampler2 for 0 (i.e., se o texel for transparente).
	if(tex2.a == 0.0) {
		// O fragmento resultante toma o valor do texel da textura referenciada pelo sampler TexSampler1 (parede).
		fColor = tex1;
	}
	else if(toMixBorder) { // Contorno de pixéis brancos, em torno da imagem do graffiti.
		// Mistura as duas texturas
		fColor = mix(tex1, tex2, 0.2); // 80% tex1 + 20% tex2
	}
	else {
		// O fragmento resultante toma o valor do texel da textura referenciada pelo sampler TexSampler2 (graffiti).
		fColor = tex2;
	}
#endif
}