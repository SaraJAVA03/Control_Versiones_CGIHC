//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>

//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>

//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"

//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList; //LISTA DE MESH, PUEDE HABER MAS DE UN MESH 
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;

//Vertex Shader, es comun para todos
static const char* vShader = "shaders/shader.vert";

//
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";


//shaders nuevos para cada COLOR
static const char* vShaderRojo = "shaders/shaderRojo.vert";
static const char* vShaderCafe = "shaders/shaderCafe.vert";
static const char* vShaderVerde = "shaders/shaderVerde.vert";
static const char* vShaderAzul = "shaders/shaderAzul.vert";
static const char* vShaderVerdeO = "shaders/shaderVerdeO.vert";


float angulo = 0.0f;



//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = {
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3

	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}


//color café en RGB : 0.478f, 0.255f, 0.067f,

//color verde oscuro en RGB : 0.0f, 0.5f, 0.0f



//---------LETRAS-----
/*

void CrearLetrasyFiguras()
{
	GLfloat vertices_letra_S[] = {
		//X			Y			Z			R		G		B
		-0.45f, 0.31f,		0.5f,			0.592f,	0.459f,	1.0f,
		-0.42f, 0.17f,		0.5f,			0.592f,	0.459f,	1.0f,
		-0.39f, 0.25f,		0.5f,			0.592f,	0.459f,	1.0f, //18 --- 1

		-0.45f, 0.31f,		0.5f,			0.592f,	0.459f,	1.0f,
		-0.47f, 0.21f,		0.5f,			0.592f,	0.459f,	1.0f,
		-0.42f, 0.17f,		0.5f,			0.592f,	0.459f,	1.0f, //2

		-0.53f, 0.33f,		0.5f,			0.592f,	0.459f,	1.0f,
		-0.47f, 0.21f,		0.5f,			0.592f,	0.459f,	1.0f,
		-0.45f, 0.31f,		0.5f,			0.592f,	0.459f,	1.0f, //3

		-0.53f, 0.33f,		0.5f,			0.592f,	0.459f,	1.0f,
		-0.54f, 0.23f,		0.5f,			0.592f,	0.459f,	1.0f,
		-0.47f, 0.21f,		0.5f,			0.592f,	0.459f,	1.0f, //4

		-0.6f, 0.34f,		0.5f,			0.592f,	0.459f,	1.0f,
		-0.54f, 0.23f,		0.5f,			0.592f,	0.459f,	1.0f,
		-0.53f, 0.33f,		0.5f,			0.592f,	0.459f,	1.0f, //5

		-0.65f, 0.23f,		0.5f,			0.592f,	0.459f,	1.0f,
		-0.54f, 0.23f,		0.5f,			0.592f,	0.459f,	1.0f,
		-0.6f, 0.34f,		0.5f,			0.592f,	0.459f,	1.0f, //6

		-0.65f, 0.23f,		0.5f,			0.592f,	0.459f,	1.0f,
		-0.64f, 0.19f,		0.5f,			0.592f,	0.459f,	1.0f,
		-0.54f, 0.23f,		0.5f,			0.592f,	0.459f,	1.0f, //7

		-0.64f, 0.19f,		0.5f,			0.592f,	0.459f,	1.0f,
		-0.62f, 0.11f,		0.5f,			0.592f,	0.459f,	1.0f,
		-0.54f, 0.23f,		0.5f,			0.592f,	0.459f,	1.0f, //8

		-0.62f, 0.11f,		0.5f,			0.592f,	0.459f,	1.0f,
		-0.58f, 0.04f,		0.5f,			0.592f,	0.459f,	1.0f,
		-0.54f, 0.23f,		0.5f,			0.592f,	0.459f,	1.0f, //9

		-0.58f, 0.04f,		0.5f,			0.592f,	0.459f,	1.0f,
		-0.54f, 0.17f,		0.5f,			0.592f,	0.459f,	1.0f,
		-0.54f, 0.23f,		0.5f,			0.592f,	0.459f,	1.0f, //10

		-0.58f, 0.04f,		0.5f,			0.592f,	0.459f,	1.0f,
		-0.51f, 0.107f,		0.5f,			0.592f,	0.459f,	1.0f,
		-0.54f, 0.17f,		0.5f,			0.592f,	0.459f,	1.0f, //11

		-0.58f, 0.04f,		0.5f,			0.592f,	0.459f,	1.0f,
		-0.527f, -0.007f,	0.5f,			0.592f,	0.459f,	1.0f,
		-0.51f, 0.107f,		0.5f,			0.592f,	0.459f,	1.0f, //12

		-0.527f, -0.007f,	0.5f,			0.592f,	0.459f,	1.0f,
		-0.47f, 0.042f,		0.5f,			0.592f,	0.459f,	1.0f,
		-0.51f, 0.107f,		0.5f,			0.592f,	0.459f,	1.0f, //13

		-0.527f, -0.007f,	0.5f,			0.592f,	0.459f,	1.0f,
		-0.41f, -0.013f,	0.5f,			0.592f,	0.459f,	1.0f,
		-0.47f, 0.042f,		0.5f,			0.592f,	0.459f,	1.0f, //14

		-0.41f, -0.013f,	0.5f,			0.592f,	0.459f,	1.0f,
		-0.44f,-0.18f,		0.5f,			0.592f,	0.459f,	1.0f,
		-0.4f, -0.08f,		0.5f,			0.592f,	0.459f,	1.0f, //15

		-0.41f, -0.013f,	0.5f,			0.592f,	0.459f,	1.0f,
		-0.44f,-0.18f,		0.5f,			0.592f,	0.459f,	1.0f,
		-0.4f, -0.08f,		0.5f,			0.592f,	0.459f,	1.0f, //16

		-0.44f,-0.18f,		0.5f,			0.592f,	0.459f,	1.0f,
		-0.4f, -0.142f,		0.5f,			0.592f,	0.459f,	1.0f,
		-0.4f, -0.08f,		0.5f,			0.592f,	0.459f,	1.0f, //17

		-0.53f, -0.07f,		0.5f,			0.592f,	0.459f,	1.0f,
		-0.514f, -0.203f,	0.5f,			0.592f,	0.459f,	1.0f,
		-0.44f,-0.18f,		0.5f,			0.592f,	0.459f,	1.0f, //18

		-0.53f, -0.07f,		0.5f,			0.592f,	0.459f,	1.0f,
		-0.585f, -0.08f,	0.5f,			0.592f,	0.459f,	1.0f,
		-0.514f, -0.203f,	0.5f,			0.592f,	0.459f,	1.0f, //19

		-0.585f, -0.08f,	0.5f,			0.592f,	0.459f,	1.0f,
		-0.676f, -0.14f,	0.5f,			0.592f,	0.459f,	1.0f,
		-0.514f, -0.203f,	0.5f,			0.592f,	0.459f,	1.0f, //20

		-0.676f, -0.14f,	0.5f,			0.592f,	0.459f,	1.0f,
		-0.6f, -0.2f,		0.5f,			0.592f,	0.459f,	1.0f,
		-0.514f, -0.203f,	0.5f,			0.592f,	0.459f,	1.0f, //21

		-0.585f, -0.08f,	0.5f,			0.592f,	0.459f,	1.0f,
		-0.71f, -0.065f,	0.5f,			0.592f,	0.459f,	1.0f,
		-0.676f, -0.14f,	0.5f,			0.592f,	0.459f,	1.0f, //22

		-0.627f, -0.043f,	0.5f,			0.592f,	0.459f,	1.0f,
		-0.71f, -0.065f,	0.5f,			0.592f,	0.459f,	1.0f,
		-0.585f, -0.08f,	0.5f,			0.592f,	0.459f,	1.0f, //23

		-0.671f, -0.002f,	0.5f,			0.592f,	0.459f,	1.0f,
		-0.71f, -0.065f,	0.5f,			0.592f,	0.459f,	1.0f,
		-0.627f, -0.043f,	0.5f,			0.592f,	0.459f,	1.0f, //24

		-0.527f, -0.007f,	0.5f,			0.592f,	0.459f,	1.0f,
		-0.44f, -0.18f,		0.5f,			0.592f,	0.459f,	1.0f,
		-0.41f, -0.013f,	0.5f,			0.592f,	0.459f,	1.0f, //25

		-0.527f, -0.007f,	0.5f,			0.592f,	0.459f,	1.0f,
		-0.53f, -0.07f,		0.5f,			0.592f, 0.459f, 1.0f,
		-0.44f, -0.18f,		0.5f,			0.592f, 0.459f, 1.0f, //26
	};

	MeshColor* letraS = new MeshColor();
	letraS->CreateMeshColor(vertices_letra_S, 468);
	meshColorList.push_back(letraS);

	GLfloat vertices_letra_J[] = {
		//X			Y			Z			R		G		B
		-0.24f,  0.3f,		0.5f,			0.78f,	0.459f,	1.0f,
		-0.24f, 0.19f,		0.5f,			0.78f,	0.459f,	1.0f,
		0.24f,  0.29f,		0.5f,			0.78f,	0.459f,	1.0f, //18 --- 1

		0.24f,  0.29f,		0.5f,			0.78f,	0.459f,	1.0f,
		-0.24f, 0.19f,		0.5f,			0.78f,	0.459f,	1.0f,
		0.24f,  0.19f,		0.5f,			0.78f,	0.459f,	1.0f, //2

		-0.1f,  0.2f,		0.5f,			0.78f,	0.459f,	1.0f,
		0.04f, -0.1f,		0.5f,			0.78f,	0.459f,	1.0f,
		0.05f,  0.0f,		0.5f,			0.78f,	0.459f,	1.0f, //3

		-0.3f,  0.0f,		0.5f,			0.78f,	0.459f,	1.0f,
		-0.1f, -0.1f,		0.5f,			0.78f,	0.459f,	1.0f,
		-0.2f,  0.0f,		0.5f,			0.78f,	0.459f,	1.0f, //4

		-0.3f,  0.0f,		0.5f,			0.78f,	0.459f,	1.0f,
		-0.2f, -0.2f,		0.5f,			0.78f,	0.459f,	1.0f,
		-0.1f, -0.1f,		0.5f,			0.78f,	0.459f,	1.0f, //5

		-0.1f, -0.1f,		0.5f,			0.78f,	0.459f,	1.0f,
		-0.2f, -0.2f,		0.5f,			0.78f,	0.459f,	1.0f,
		0.04f, -0.1f,		0.5f,			0.78f,	0.459f,	1.0f, //6

		-0.05f, 0.0f,		0.5f,			0.78f,	0.459f,	1.0f,
		-0.1f, -0.1f,		0.5f,			0.78f,	0.459f,	1.0f,
		0.04f, -0.1f,		0.5f,			0.78f,	0.459f,	1.0f, //7

		-0.1f,  0.2f,		0.5f,			0.78f,	0.459f,	1.0f,
		0.05f, 0.09f,		0.5f,			0.78f,	0.459f,	1.0f,
		0.1f,   0.2f,		0.5f,			0.78f,	0.459f,	1.0f, //8

		-0.1f,  0.2f,		0.5f,			0.78f,	0.459f,	1.0f,
		0.05f,  0.0f,		0.5f,			0.78f,	0.459f,	1.0f,
		0.05f, 0.09f,		0.5f,			0.78f,	0.459f,	1.0f, //9

		-0.05f, 0.1f,		0.5f,			0.78f,	0.459f,	1.0f,
		-0.05f, 0.0f,		0.5f,			0.78f,	0.459f,	1.0f,
		0.04f, -0.1f,		0.5f,			0.78f,	0.459f,	1.0f, //10

		-0.3f,  0.0f,		0.5f,			0.78f,	0.459f,	1.0f,
		-0.3f, -0.1f,		0.5f,			0.78f,	0.459f,	1.0f,
		-0.2f, -0.2f,		0.5f,			0.78f,	0.459f,	1.0f, //11

		-0.1f, -0.1f,		0.5f,			0.78f,	0.459f,	1.0f,
		-0.2f, -0.2f,		0.5f,			0.78f,	0.459f,	1.0f,
		-0.1f, -0.2f,		0.5f,			0.78f,	0.459f,	1.0f, //12

		-0.1f, -0.1f,		0.5f,			0.78f,	0.459f,	1.0f,
		-0.1f, -0.2f,		0.5f,			0.78f,	0.459f,	1.0f,
		0.0f, -0.16f,		0.5f,			0.78f,	0.459f,	1.0f, //13

		-0.1f, -0.1f,		0.5f,			0.78f,	0.459f,	1.0f,
		0.0f, -0.16f,		0.5f,			0.78f,	0.459f,	1.0f,
		0.04f, -0.1f,		0.5f,			0.78f,	0.459f,	1.0f, //14

			

	};

	MeshColor* letraJ = new MeshColor();
	letraJ->CreateMeshColor(vertices_letra_J, 252);
	meshColorList.push_back(letraJ);

	GLfloat vertices_letra_V[] = {
		//X			Y			Z			R		G		B
		0.4f,	0.3f,		0.5f,			0.941f,	0.459f,	1.0f,
		0.5f,  -0.2f,		0.5f,			0.941f,	0.459f,	1.0f,
		0.5f,   0.3f,		0.5f,			0.941f,	0.459f,	1.0f, //18 -- 1

		0.5f,  0.3f,		0.5f,			0.941f,	0.459f,	1.0f,
		0.5f, -0.2f,		0.5f,			0.941f,	0.459f,	1.0f,
		0.6f, -0.07f,		0.5f,			0.941f,	0.459f,	1.0f, //2

		0.6f, -0.07f,		0.5f,			0.941f,	0.459f,	1.0f,
		0.5f, -0.2f,		0.5f,			0.941f,	0.459f,	1.0f,
		0.7f, -0.2f,		0.5f,			0.941f,	0.459f,	1.0f, //3

		0.7f,  0.3f,		0.5f,			0.941f,	0.459f,	1.0f,
		0.6f, -0.07f,		0.5f,			0.941f,	0.459f,	1.0f,
		0.7f, -0.2f,		0.5f,			0.941f,	0.459f,	1.0f, //4

		0.7f,  0.3f,		0.5f,			0.941f,	0.459f,	1.0f,
		0.7f, -0.2f,		0.5f,			0.941f,	0.459f,	1.0f,
		0.8f, 0.3f,			0.5f,			0.941f,	0.459f,	1.0f, //5 -- 90

	};
	MeshColor* letraV = new MeshColor();
	letraV->CreateMeshColor(vertices_letra_V, 90);
	meshColorList.push_back(letraV);

	
	
	GLfloat vertices_trianguloazul[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.0f,	1.0f,		0.5f,			0.0f,	0.0f,	1.0f,

	};
	MeshColor* trianguloazul = new MeshColor();
	trianguloazul->CreateMeshColor(vertices_trianguloazul, 18);
	meshColorList.push_back(trianguloazul);

	GLfloat vertices_trianguloverde[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	1.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			0.0f,	1.0f,	0.0f,

	};
	MeshColor* trianguloverde = new MeshColor();
	trianguloverde->CreateMeshColor(vertices_trianguloverde, 18);
	meshColorList.push_back(trianguloverde);

	GLfloat vertices_cuadradoverde[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,

	};
	MeshColor* cuadradoverde = new MeshColor();
	cuadradoverde->CreateMeshColor(vertices_cuadradoverde, 36);
	meshColorList.push_back(cuadradoverde);

	GLfloat vertices_cuadradorojo[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,

	};
	MeshColor* cuadradorojo = new MeshColor();
	cuadradorojo->CreateMeshColor(vertices_cuadradorojo, 36);
	meshColorList.push_back(cuadradorojo);

	GLfloat vertices_cuadradocafe[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.478f, 0.255f, 0.067f,
		0.5f,	-0.5f,		0.5f,			0.478f, 0.255f, 0.067f,
		0.5f,	0.5f,		0.5f,			0.478f, 0.255f, 0.067f,
		-0.5f,	-0.5f,		0.5f,			0.478f, 0.255f, 0.067f,
		0.5f,	0.5f,		0.5f,			0.478f, 0.255f, 0.067f,
		-0.5f,	0.5f,		0.5f,			0.478f, 0.255f, 0.067f,
	};
	MeshColor* cuadradocafe = new MeshColor();
	cuadradocafe->CreateMeshColor(vertices_cuadradocafe, 36);
	meshColorList.push_back(cuadradocafe);
	
}
*/



void CreateShaders()
{

	Shader* shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);

	//--------------
	//NUEVOS SHADERS
	//--------------

	//ROJO (2)
	Shader* shaderROJO = new Shader();
	shaderROJO->CreateFromFiles(vShaderRojo, fShader);
	shaderList.push_back(*shaderROJO); 

	//CAFE (3)
	Shader* shaderCAFE = new Shader();
	shaderCAFE->CreateFromFiles(vShaderCafe, fShader);
	shaderList.push_back(*shaderCAFE);

	//VERDE (4)
	Shader* shaderVERDE = new Shader();
	shaderVERDE->CreateFromFiles(vShaderVerde, fShader);
	shaderList.push_back(*shaderVERDE);

	//AZUL (5)
	Shader* shaderAZUL = new Shader();
	shaderAZUL->CreateFromFiles(vShaderAzul, fShader);
	shaderList.push_back(*shaderAZUL);

	//VERDE OSCURO (6)
	Shader* shaderVERDEO = new Shader();
	shaderVERDEO->CreateFromFiles(vShaderVerdeO, fShader);
	shaderList.push_back(*shaderVERDEO);

}



int main()
{
	mainWindow = Window(800, 800);
	mainWindow.Initialise();

	CreaPiramide(); //índice 0 en MeshList, EL ORDEN EN EL QUE SE LLAMEN CAMBIA LA POSCION DE LOS INDICES
	CrearCubo();//índice 1 en MeshList
	//CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();

	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;

	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	
	// OJO
	//OJITO IMPORTANTE
	// OJO
	
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f); //VISUALIAZ EN 3D
	//												FOV																			NEAAAAR FAR
	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.

	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad. siempre antes del while


	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(1.0f, 1.0f, 1.0f, .0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad


		/*
		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

	

		//-----------
			//LETRAS
		//-----------

		//-------
		// meshColorList[2]->RenderMeshColor();
		// 0=S, 1=J, 2=V
		// ------
		
		//LETRA S
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.18f, 0.0f, -4.0f)); //dentro del monitor

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor(); //
		 
		// LETRA J
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.1f, 0.0f, -4.0f)); //dentro del monitor

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[1]->RenderMeshColor(); //
		 
		// LETRA V
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.05f, 0.0f, -4.0f)); //dentro del monitor
		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[2]->RenderMeshColor(); 

		*/
		

		// --------------
		//CUBO Y PIRAMIDE
		//---------------

		//meshList[1]->RenderMesh(); 
		// 0=piramide, 1=cubo

		//shaderList[].useShader();
		//0=clamp, 1= ,2=ROJO, 3=CAFE, 4=VERDE, 5=AZUL, 6=VERDE OSCURO


		//------Cubo ROJO (fachada)--------
		shaderList[2].useShader(); 
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo += 0.1;
			
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.49f, -3.0f)); 

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh(); 

		//------Piramide AZUL (Techo) --------
		shaderList[5].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo += 0.1;

		model = glm::mat4(1.0);

		model = glm::scale(model, glm::vec3(1.1f, 0.9f, 1.1f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -2.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();


		//------Cubo MARRON (tronco) 1--------
		shaderList[3].useShader(); 
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo += 0.1;

		model = glm::mat4(1.0);
		
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::translate(model, glm::vec3(2.58f, -2.79f, -3.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh(); 

		//------Cubo MARRON (tronco) 2--------
		shaderList[3].useShader(); 
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo += 0.1;

		model = glm::mat4(1.0);

		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::translate(model, glm::vec3(-2.58f, -2.79f, -3.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh(); 

		//------Piramide VERDE (Copa) 1--------
		shaderList[6].useShader(); 
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo += 0.1;

		model = glm::mat4(1.0);

		model = glm::scale(model, glm::vec3(0.43f, 0.435f, 0.43f));
		model = glm::translate(model, glm::vec3(1.8f, -1.09f, -1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh(); 

		//------Piramide VERDE (Copa) 2--------
		shaderList[6].useShader(); 
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo += 0.1;

		model = glm::mat4(1.0);

		model = glm::scale(model, glm::vec3(0.43f, 0.435f, 0.43f));
		model = glm::translate(model, glm::vec3(-1.8f, -1.09f, -1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh(); 


		//------Cubo VERDE (Ventana) 1--------
		shaderList[4].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo += 0.1;

		model = glm::mat4(1.0);

		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
		model = glm::translate(model, glm::vec3(0.7f, -0.69f, -3.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//------Cubo VERDE (Ventana) 2--------
		shaderList[4].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo += 0.1;

		model = glm::mat4(1.0);

		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
		model = glm::translate(model, glm::vec3(-0.7f, -0.69f, -3.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh(); 

		
		//------Cubo VERDE (Ventana) 2--------
		shaderList[4].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo += 0.1;

		model = glm::mat4(1.0);

		model = glm::scale(model, glm::vec3(0.35f, 0.4f, 0.35f));
		model = glm::translate(model, glm::vec3(0.0f, -1.968f, -3.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh(); 

		//---------------------------

		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);

//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));

//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/

/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/


