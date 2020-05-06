#version 440 core

layout (location = 0) uniform vec2 uMousePos;
layout (location = 1) uniform vec2 uResolution;

// Redeclara��o de gl_FragCoord, com o qualificador 'origin_upper_left'.
// Caso contr�rio, a origem seria no canto inferior esquerdo.
// Assim o valor de gl_FragCoord tem a mesma origem que a posi��o do rato, obtida por 'glfwGetCursorPos()'
layout(origin_upper_left) in vec4 gl_FragCoord;

void main( void ) 
{
	// Dist�ncia entre a posi��o do rato e o fragmento.
	// Cada elemento do vetor 'mouse_distance' ir� ter um valor no intervalo [-1, 1].
	vec2 mouse_distance = (uMousePos / uResolution) - (gl_FragCoord.xy / uResolution);

	// O tamanho do vetor � calculado pela fun��o 'length()'. O tamanho ser� sempre um valor positivo entre [0:Pr�ximo, 1:Distante].
	// A vari�vel 'brightness' ir� tomar um valor no intervalo [0:Distante, 1:Pr�ximo].
	float brightness = 1.0 - length(mouse_distance);

	// Atribui o valor do brilho (vari�vel 'brightness') �s componentes Red, Green e Blue.
	gl_FragColor = vec4(brightness, brightness, brightness, 1.0f);
}
