#include <stdio.h>
#include <string.h>
#include <glew.h>
#include <glfw3.h>
#include <windows.h> //PARA SLEEP
#include <stdlib.h> //PARA RAND
#include <time.h> //PARA TIME

// Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 800;
GLuint VAO, VBO, shader;

// Variables RGB
float fondo_Rojo = 1.0f;
float fondo_Verde = 0.0f;
float fondo_Azul = 0.0f;

//TABLA CON LOS COLORES A ELEGIR
const float tabla_RGB[3][3] = {
	{1.0f,0.0f,0.0f},
	{0.0f, 1.0f, 0.0f},
	{0.0f,0.0f, 1.0f}
};

// Bnadera para saber si es la primera vez que se ejecuta el programaa
//int primera_vez = 1 ;

// LENGUAJE DE SHADER (SOMBRAS) GLSL
// Vertex Shader
// recibir color, salida Vcolor
static const char* vShader = "						\n\
#version 330										\n\
layout (location =0) in vec3 pos;					\n\
void main()											\n\
{													\n\
gl_Position=vec4(pos.x,pos.y,pos.z,1.0f); 			\n\
}";

// Fragment Shader
// recibir Vcolor y dar de salida color
static const char* fShader = "						\n\
#version 330										\n\
out vec4 color;										\n\
void main()											\n\
{													\n\
	color = vec4(0.0f,0.0f,0.0f,1.0f);	 			\n\
}";



void CrearTriangulo()
{
	GLfloat vertices[] = {

		//S
		-0.45f, 0.31f, 0.0f,
		-0.42f, 0.17f, 0.0f,
		-0.39f, 0.25f, 0.0f,//60

		-0.45f, 0.31f, 0.0f,
		-0.47f, 0.21f, 0.0f,
		-0.42f, 0.17f, 0.0f,//63

		-0.53f, 0.33f, 0.0f,
		-0.47f, 0.21f, 0.0f,
		-0.45f, 0.31f, 0.0f,//66

		-0.53f, 0.33f, 0.0f,
		-0.54f, 0.23f, 0.0f,
		-0.47f, 0.21f, 0.0f,//69

		-0.6f, 0.34f, 0.0f,
		-0.54f, 0.23f, 0.0f,
		-0.53f, 0.33f, 0.0f,//72

		-0.65f, 0.23f, 0.0f,
		-0.54f, 0.23f, 0.0f,
		-0.6f, 0.34f, 0.0f,//75

		-0.65f, 0.23f, 0.0f,
		-0.64f, 0.19f, 0.0f,
		-0.54f, 0.23f, 0.0f,//78

		-0.64f, 0.19f, 0.0f,
		-0.62f, 0.11f, 0.0f,
		-0.54f, 0.23f, 0.0f,//81

		-0.62f, 0.11f, 0.0f,
		-0.58f, 0.04f, 0.0f,
		-0.54f, 0.23f, 0.0f,//84

		-0.58f, 0.04f, 0.0f,
		-0.54f, 0.17f, 0.0f,
		-0.54f, 0.23f, 0.0f,//87

		-0.58f, 0.04f, 0.0f,
		-0.51f, 0.107f, 0.0f,
		-0.54f, 0.17f, 0.0f,//91

		-0.58f, 0.04f, 0.0f,
		-0.527f, -0.007f, 0.0f,
		-0.51f, 0.107f, 0.0f,//94

		-0.527f, -0.007f, 0.0f,
		-0.47f, 0.042f, 0.0f,
		-0.51f, 0.107f, 0.0f,//97

		-0.527f, -0.007f, 0.0f,
		-0.41f, -0.013f, 0.0f,
		-0.47f, 0.042f, 0.0f,//100

		-0.527f, -0.007f, 0.0f,
		-0.44f,-0.18f, 0.0f,
		-0.41f, -0.013f, 0.0f,//103

		-0.527f, -0.007f, 0.0f,
		-0.53f, -0.07f, 0.0f,
		-0.44f,-0.18f, 0.0f, //106

		-0.41f, -0.013f, 0.0f,
		-0.44f,-0.18f, 0.0f,
		-0.4f, -0.08f, 0.0f, //109

		-0.44f,-0.18f, 0.0f,
		-0.4f, -0.142f,0.0f,
		-0.4f, -0.08f, 0.0f, //112

		-0.53f, -0.07f, 0.0f,
		-0.514f, -0.203f, 0.0f,
		-0.44f,-0.18f, 0.0f, //115

		-0.53f, -0.07f, 0.0f,
		-0.585f, -0.08f, 0.0f,
		-0.514f, -0.203f, 0.0f, //118

		-0.585f, -0.08f, 0.0f,
		-0.676f, -0.14f, 0.0f,
		-0.514f, -0.203f, 0.0f, //121

		-0.676f, -0.14f, 0.0f,
		-0.6f, -0.2f, 0.0f,
		-0.514f, -0.203f, 0.0f, //124

		-0.585f, -0.08f, 0.0f,
		-0.71f, -0.065f, 0.0f,
		-0.676f, -0.14f, 0.0f, //127

		-0.627f, -0.043f, 0.0f,
		-0.71f, -0.065f, 0.0f,
		-0.585f, -0.08f, 0.0f, //130

		-0.671f, -0.002f, 0.0f,
		-0.71f, -0.065f, 0.0f,
		-0.627f, -0.043f, 0.0f, //133

		//J
		-0.24f, 0.3f, 0.0f,
		-0.24f, 0.19f, 0.0f,
		0.24f, 0.29f, 0.0f,//3

		0.24f, 0.29f, 0.0f,
		-0.24f, 0.19f, 0.0f,
		0.24f, 0.19f, 0.0f,//6

		-0.1f, 0.2f, 0.0f,
		0.04f, -0.1f, 0.0f,
		0.05f, 0.0f, 0.0f,//9

		-0.3f, 0.0f, 0.0f,
		-0.1f,-0.1f, 0.0f,
		-0.2f, 0.0f, 0.0f, //12

		-0.3f, 0.0f, 0.0f, 
		-0.2f, -0.2f, 0.0f, 
		-0.1f, -0.1f, 0.0f, //15

		-0.1f, -0.1f, 0.0f, 
		-0.2f, -0.2f, 0.0f, 
		0.04f, -0.1f, 0.0f, //18

		-0.05f, 0.0f, 0.0f,
		-0.1f, -0.1f, 0.0f,
		0.04f, -0.1f, 0.0f, //21

		-0.1f, 0.2f, 0.0f,
		0.05f, 0.09f, 0.0f, 
		0.1f, 0.2f, 0.0f, //24

		-0.1f, 0.2f, 0.0f,
		0.05f, 0.0f, 0.0f,
		0.05f, 0.09f, 0.0f, //27

		-0.05f, 0.1f, 0.0f,
		-0.05f, 0.0f, 0.0f,
		0.04f, -0.1f, 0.0f, //30

		-0.3f, 0.0f, 0.0f,
		-0.3f, -0.1f, 0.0f,
		-0.2f, -0.2f, 0.0f, // 33

		-0.1f, -0.1f, 0.0f,
		-0.2f, -0.2f, 0.0f,
		-0.1f, -0.2f, 0.0f, // 36

		-0.1f, -0.1f, 0.0f,
		-0.1f, -0.2f, 0.0f,
		0.0f, -0.16f, 0.0f, //39

		-0.1f, -0.1f, 0.0f,
		0.0f, -0.16f, 0.0f,
		0.04f, -0.1f, 0.0f, //42

		//V
		0.4f, 0.3f, 0.0f,
		0.5f, -0.2f, 0.0f,
		0.5f, 0.3f, 0.0f, //45

		0.5f, 0.3f, 0.0f,
		0.5f, -0.2f, 0.0f,
		0.6f, -0.07f, 0.0f, //48

		0.6f, -0.07f, 0.0f,
		0.5f, -0.2f, 0.0f,
		0.7f, -0.2f, 0.0f, //51

		0.7f, 0.3f, 0.0f,
		0.6f, -0.07f, 0.0f,
		0.7f, -0.2f, 0.0f, //54

		0.7f, 0.3f, 0.0f,
		0.7f, -0.2f, 0.0f,
		0.8f, 0.3f, 0.0f //57




	};
	glGenVertexArrays(1, &VAO); //generar 1 VAO
	glBindVertexArray(VAO);//asignar VAO

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //pasarle los datos al VBO asignando tamano, los datos y en este caso es estático pues no se modificarán los valores

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);//Stride en caso de haber datos de color por ejemplo, es saltar cierta cantidad de datos
	glEnableVertexAttribArray(0);
	//agregar valores a vèrtices y luego declarar un nuevo vertexAttribPointer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) //Función para agregar los shaders a la tarjeta gráfica

//El Program recibe los datos de theShader

{
	GLuint theShader = glCreateShader(shaderType);//theShader es un shader que se crea de acuerdo al tipo de shader: vertex o fragment
	const GLchar* theCode[1];
	theCode[0] = shaderCode;//shaderCode es el texto que se le pasa a theCode
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);//longitud del texto
	glShaderSource(theShader, 1, theCode, codeLength);//Se le asigna al shader el código
	glCompileShader(theShader);//Se comila el shader
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	//verificaciones y prevención de errores
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al compilar el shader %d es: %s \n", shaderType, eLog);
		return;
	}
	glAttachShader(theProgram, theShader);//Si no hubo problemas se asigna el shader a theProgram el cual asigna el código a la tarjeta gráfica
}

void CompileShaders() {
	shader = glCreateProgram(); //se crea un programa
	if (!shader)
	{
		printf("Error creando el shader");
		return;
	}
	AddShader(shader, vShader, GL_VERTEX_SHADER);//Agregar vertex shader
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);//Agregar fragment shader
	
	//Para terminar de linkear el programa y ver que no tengamos errores
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glLinkProgram(shader);//se linkean los shaders a la tarjeta gráfica
	
	//verificaciones y prevención de errores
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al linkear es: %s \n", eLog);
		return;
	}
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al validar es: %s \n", eLog);
		return;
	}
}

int main()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}

	//****  LAS SIGUIENTES 4 LÍNEAS SE COMENTAN EN DADO CASO DE QUE AL USUARIO NO LE FUNCIONE LA VENTANA Y PUEDA CONOCER LA VERSIÓN DE OPENGL QUE TIENE ****/

	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Segunda ventana", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Asignar valores de la ventana y coordenadas
	//Asignar Viewport
	glViewport(0, 0, BufferWidth, BufferHeight);

	//Llamada a las funciones creadas antes del main
	CrearTriangulo();
	CompileShaders();

	// Periodo de retardo, 3000 ms = 3 segundos
	const int retardoMilisegundos = 2000;

	//Con este se genera un inicio diferente
	srand(time(NULL));

	//Loop mientras no se cierra la ventana
	while (!glfwWindowShouldClose(mainWindow))
	{
		//Recibir eventos del usuario
		glfwPollEvents();

		// Bloque en donde se realiza el cambio de color
		//if (!primera_vez) {
		//	float temp = fondo_Rojo;
		//	fondo_Rojo = fondo_Azul;
		//	fondo_Azul = fondo_Verde;
		//	fondo_Verde = temp;
		//}
		//primera_vez = 0; //interpretar 0 como que ya no es la primera

		//Bloque de asignacíon del color usando la tabla de colores
		int indice = rand() % 3;
		fondo_Rojo = tabla_RGB[indice][0];
		fondo_Verde = tabla_RGB[indice][1];
		fondo_Azul = tabla_RGB[indice][2];



		// Limpiar la ventana
		glClearColor(fondo_Rojo, fondo_Verde, fondo_Azul, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 133);
		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
		//NO ESCRIBIR NINGUNA LÍNEA DESPUÉS DE glfwSwapBuffers(mainWindow); 
		
		// Congela por 3 segs
		Sleep(retardoMilisegundos);
	}

	return 0;
}