#version 440 core

layout (location = 0) uniform vec2 uMousePos;
layout (location = 1) uniform vec2 uResolution;

// Redeclaração de gl_FragCoord, com o qualificador 'origin_upper_left'.
// Caso contrário, a origem seria no canto inferior esquerdo.
// Assim o valor de gl_FragCoord tem a mesma origem que a posição do rato, obtida por 'glfwGetCursorPos()'
layout(origin_upper_left) in vec4 gl_FragCoord;

void main( void ) 
{
	// Distância entre a posição do rato e o fragmento.
	// Cada elemento do vetor 'mouse_distance' irá ter um valor no intervalo [-1, 1].
	vec2 mouse_distance = (uMousePos / uResolution) - (gl_FragCoord.xy / uResolution);

	// O tamanho do vetor é calculado pela função 'length()'. O tamanho será sempre um valor positivo entre [0:Próximo, 1:Distante].
	// A variável 'brightness' irá tomar um valor no intervalo [0:Distante, 1:Próximo].
	float brightness = 1.0 - length(mouse_distance);

	// Atribui o valor do brilho (variável 'brightness') às componentes Red, Green e Blue.
	gl_FragColor = vec4(brightness, brightness, brightness, 1.0f);
}
