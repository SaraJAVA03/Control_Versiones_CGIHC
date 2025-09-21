/*Práctica 4: Modelado Jerárquico.
Se implementa el uso de matrices adicionales para almacenar información de transformaciones geométricas que se quiere 
heredar entre diversas instancias para que estén unidas
Teclas de la F a la K para rotaciones de articulaciones
*/
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
#include <gtc\random.hpp>
//clases para dar orden y limpieza al còdigo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z
using std::vector;
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks

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

// Pirámide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

}
/*
Crear cilindro y cono con arreglos dinámicos vector creados en el Semestre 2023 - 1 : por Sánchez Pérez Omar Alejandro
*/
void CrearCilindro(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//ciclo for para crear los vértices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el círculo
		else {
			x = R * cos((0)*dt);
			z = R * sin((0)*dt);
		}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			case 3:
				vertices.push_back(x);
				break;
			case 4:
				vertices.push_back(0.5);
				break;
			case 5:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia inferior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(-0.5f);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia superior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(0.5);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//Se generan los indices de los vértices
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);

	//se genera el mesh del cilindro
	Mesh *cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

//función para crear un cono
void CrearCono(int res,float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;
	
	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);
	
	//ciclo for para crear los vértices de la circunferencia del cono
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	vertices.push_back(R * cos(0) * dt);
	vertices.push_back(-0.5);
	vertices.push_back(R * sin(0) * dt);


	for (i = 0; i < res+2; i++) indices.push_back(i);

	//se genera el mesh del cono
	Mesh *cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}

//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	vector<unsigned int> piramidecuadrangular_indices = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

	};
	vector<GLfloat> piramidecuadrangular_vertices = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh *piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}



void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	//Cilindro y cono reciben resolución (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//índice 0 en MeshList
	CrearPiramideTriangular();//índice 1 en MeshList
	CrearCilindro(11, 1.0f);//índice 2 en MeshList
	CrearCono(25, 2.0f);//índice 3 en MeshList
	CrearPiramideCuadrangular();//índice 4 en MeshList
	CreateShaders();



	/*Cámara se usa el comando: glm::lookAt(vector de posición, vector de orientación, vector up));
	En la clase Camera se reciben 5 datos:
	glm::vec3 vector de posición,
	glm::vec3 vector up,
	GlFloat yaw rotación para girar hacia la derecha e izquierda
	GlFloat pitch rotación para inclinar hacia arriba y abajo
	GlFloat velocidad de desplazamiento,
	GlFloat velocidad de vuelta o de giro
	Se usa el Mouse y las teclas WASD y su posición inicial está en 0,0,1 y ve hacia 0,0,-1.
	*/
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.2f, 0.2f);
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);

	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader




	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4
	glm::mat4 modelaux(1.0);//Inicializar matriz de Modelo 4x4 auxiliar para la jerarquía
	

	//PARA GRUA
	glm::mat4 modelCabinaBase(1.0);//Inicializar matriz de Modelo 4x4 auxiliar 2 para la jerarquía
	glm::mat4 modelBaseRuedas(1.0);


	//PARA GATO
	glm::mat4 modelCuerpoP(1.0);
	glm::mat4 modelCuerpoR(1.0);
	glm::mat4 modelCuerpoJ(1.0);
	glm::mat4 modelCuerpoU(1.0);

	glm::mat4 modelCabeza(1.0);
	glm::mat4 modelOREJAS(1.0);
	glm::mat4 modelBOQUITA(1.0);

	glm::mat4 modelCola(1.0);



	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f); //inicializar Color para enviar a variable Uniform;

	while (!mainWindow.getShouldClose())
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		//Cámara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();



		//meshList[0]->RenderMesh();
		//0=cubo, 1=piramide tringular, 2=cilindro, 3=cono, 4=piramide cuadrangular (AQUI NO)



		/*

		//----------------------------------
		//-------------GRUA-----------------
		//----------------------------------


		//0.91, 0.596, 0 naranja
		//0.529, 0.322, 0.004 naranja fuerte tirando a café

		//CABINA
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 5.5f, -4.0f));

		modelaux = model;
		modelCabinaBase = model;

		model = glm::scale(model, glm::vec3(5.0f, 3.0f, 2.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		color = glm::vec3(10.91, 0.596, 0);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		meshList[0]->RenderMesh();

		//ARTICULACION 1
		model = modelaux;
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.529, 0.322, 0.004);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();


		//BRAZO 1
		model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));

		modelaux = model;
		model = glm::scale(model, glm::vec3(5.0f, 1.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		color = glm::vec3(0.91, 0.596, 0);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh(); //dibuja cubo y pirámide triangular


		//SEGUNDA ARTICULACION
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(0.529, 0.322, 0.004);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render(); //dibuja esfera

		//BRAZO 2
		model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));

		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		color = glm::vec3(0.91, 0.596, 0);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh(); //dibuja cubo y pirámide triangular

		//ARTIUCLACION 3
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.529, 0.322, 0.004);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();


		//BRAZO 3
		model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));

		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		color = glm::vec3(0.91, 0.596, 0);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh(); //dibuja cubo y pirámide triangular


		//ARTIUCLACION 4
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));

		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.529, 0.322, 0.004);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();


		//CABINA EN EL AIRE
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.8f, -1.0f, 0.0f));

		modelaux = model;
		model = glm::scale(model, glm::vec3(1.5f, 2.5f, 2.5f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		color = glm::vec3(0.91, 0.596, 0);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();


		//BASE
		model = modelCabinaBase;
		model = glm::translate(model, glm::vec3(0.0f, -1.8f, 0.0f));

		modelBaseRuedas = model;

		model = glm::scale(model, glm::vec3(6.4f, 1.0f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		color = glm::vec3(0.529, 0.322, 0.004);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[4]->RenderMesh();

		//LLANTA 1 (atras)
		//J
		model = modelBaseRuedas;

		model = glm::translate(model, glm::vec3(2.5f, -1.25f, -1.5f));

		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(1.6f, 1.6f, 1.6f));

		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()),
			glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.09, 0.078, 0.067);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry(); //dibuja cubo y pirámide triangular


		//LLANTA 2 (adelnate)
		//K
		model = modelBaseRuedas;

		model = glm::translate(model, glm::vec3(2.5f, -1.25f, 1.5f));

		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(1.6f, 1.6f, 1.6f));

		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()),
			glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.09, 0.078, 0.067);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry(); //dibuja cubo y pirámide triangular

		//LLANTA 3
		//L
		model = modelBaseRuedas;

		model = glm::translate(model, glm::vec3(-2.5f, -1.25f, -1.5f));

		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(1.6f, 1.6f, 1.6f));

		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()),
			glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.09, 0.078, 0.067);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();

		//LLANTA 4
		//P
		model = modelBaseRuedas;

		model = glm::translate(model, glm::vec3(-2.5f, -1.25f, 1.5f));

		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(1.6f, 1.6f, 1.6f));

		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()),
			glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.09, 0.078, 0.067);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();
		
		*/
	



		//----------------------------------
		//--------ANIMAL ROBOTICO-----------
		//----------------------------------


		//meshList[0]->RenderMesh();
		//0=cubo, 1=piramide tringular, 2=cilindro, 3=cono, 4=piramide cuadrangular 



		//COLORES
		// GRIS 0.635, 0.667, 0.69
		// AZUL 0.475, 0.604, 0.69
		
		//---------
		//CUERPawer
		//---------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 5.5f, -4.0f));

		
		modelCuerpoP = model;
		modelCuerpoR = model;
		modelCuerpoJ = model;
		modelCuerpoU = model;

		modelCola = model;

		modelCabeza = model;


		model = glm::scale(model, glm::vec3(5.0f, 2.0f, 3.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		color = glm::vec3(0.635, 0.667, 0.69);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		meshList[0]->RenderMeshGeometry();

		
		//---------
		//PATAS
		//---------
		//------------------------------------------------------PATA DELANTERA IZQ
		//ARTICULACION 1 (F)
		model = modelCuerpoP;

		model = glm::translate(model, glm::vec3(-1.5f, 0.0f, 1.2f));

		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));

		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.475, 0.604, 0.69);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		//P UNO
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));

		modelCuerpoP = model;

		model = glm::scale(model, glm::vec3(2.0f, 1.5f, 1.5f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		color = glm::vec3(0.635, 0.667, 0.69);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh(); 

		//ARTICULACION 2 (G)
		model = modelCuerpoP;

		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));

		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.475, 0.604, 0.69);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		//P DOS
		model = glm::translate(model, glm::vec3(2.4f, 0.0f, -0.1f));

		modelCuerpoP = model;

		model = glm::scale(model, glm::vec3(3.0f, 1.9f, 1.9f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		color = glm::vec3(0.635, 0.667, 0.69);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		/*
		//H
		model = modelCuerpoP;

		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.3f));

		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.475, 0.604, 0.69);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		
		//Pie
		model = glm::translate(model, glm::vec3(2.4f, 0.0f, -0.1f));

		modelCuerpoP = model;

		model = glm::scale(model, glm::vec3(0.6f, 1.56f, 0.8f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		color = glm::vec3(0.475, 0.604, 0.69);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();
		
		*/

		//----------------------------------------------------PATA DELANTERA DER
		//ARTICULACION  (J)
		model = modelCuerpoJ;

		model = glm::translate(model, glm::vec3(-1.5f, 0.0f, -1.2f));

		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));

		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.475, 0.604, 0.69);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		//P UNO
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));

		modelCuerpoJ = model;

		model = glm::scale(model, glm::vec3(2.0f, 1.5f, 1.5f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		color = glm::vec3(0.635, 0.667, 0.69);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//ARTICULACION 2 (K)
		model = modelCuerpoJ;

		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.3f));

		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));

		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.475, 0.604, 0.69);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		//P DOS
		model = glm::translate(model, glm::vec3(2.4f, 0.0f, -0.1f));

		modelCuerpoP = model;

		model = glm::scale(model, glm::vec3(3.5f, 1.8f, 1.8f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		color = glm::vec3(0.635, 0.667, 0.69);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();
		

		//---------------------------------------------------------PATA TRASERA DER
		//ARTICULACION  (R)
		model = modelCuerpoR;

		model = glm::translate(model, glm::vec3(1.5f, 0.0f, -1.2f));

		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));

		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion8()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.475, 0.604, 0.69);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		//P UNO
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));

		modelCuerpoR = model;

		model = glm::scale(model, glm::vec3(2.0f, 1.5f, 1.5f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		color = glm::vec3(0.635, 0.667, 0.69);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//ARTICULACION 2 (K)
		model = modelCuerpoR;

		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.3f));

		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));

		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion9()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.475, 0.604, 0.69);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		//P DOS
		model = glm::translate(model, glm::vec3(2.4f, 0.0f, -0.1f));

		modelCuerpoR = model;

		model = glm::scale(model, glm::vec3(3.5f, 1.8f, 1.8f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		color = glm::vec3(0.635, 0.667, 0.69);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();
		

		
		//---------------------------------------------------------PATA TRASERA IZQ
		//ARTICULACION  (U)
		model = modelCuerpoU;

		model = glm::translate(model, glm::vec3(1.5f, 0.0f, 1.2f));

		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));

		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.475, 0.604, 0.69);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		//P UNO
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));

		modelCuerpoU = model;

		model = glm::scale(model, glm::vec3(2.0f, 1.5f, 1.5f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		color = glm::vec3(0.635, 0.667, 0.69);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();

		//ARTICULACION 2 (G)
		model = modelCuerpoU;

		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));

		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.475, 0.604, 0.69);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		//P DOS
		model = glm::translate(model, glm::vec3(2.4f, 0.0f, -0.1f));

		modelCuerpoU = model;

		model = glm::scale(model, glm::vec3(3.0f, 1.9f, 1.9f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		color = glm::vec3(0.635, 0.667, 0.69);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[0]->RenderMesh();


		//---------
		//COLITA cilindro 
		//---------
		
		//ARTICULACION  (U)
		model = modelCola;

		model = glm::translate(model, glm::vec3(-2.5f, 0.2f, 0.0f));

		model = glm::scale(model, glm::vec3(0.55f, 0.55f, 0.55f));

		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion10()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.475, 0.604, 0.69);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		//P UNO
		model = glm::rotate(model, glm::radians(120.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));

		modelCuerpoU = model;

		model = glm::scale(model, glm::vec3(2.5f, 1.3f, 1.3f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		color = glm::vec3(0.635, 0.667, 0.69);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();

		//ARTICULACION 2 (G)
		model = modelCuerpoU;

		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));

		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));

		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion11()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.475, 0.604, 0.69);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();

		//P DOS
		model = glm::translate(model, glm::vec3(2.4f, 0.0f, -0.1f));

		modelCuerpoU = model;

		model = glm::scale(model, glm::vec3(3.0f, 1.9f, 1.9f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		color = glm::vec3(0.635, 0.667, 0.69);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMeshGeometry();

		
		//---------
		//CABEZA
		//---------
		 model= modelCabeza;

		 model = glm::translate(model, glm::vec3(2.6f, 2.1f, 0.0f));

		 modelOREJAS = model;
		 modelBOQUITA = model;

		 model = glm::scale(model, glm::vec3(2.7f, 2.1f, 2.5f));

		 glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		 color = glm::vec3(0.635, 0.667, 0.69);
		 glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		 meshList[0]->RenderMesh();

		 //BOCA
		 model = modelBOQUITA;
		 model = glm::translate(model, glm::vec3(1.6f, -0.55f, 0.0f));
		 model = glm::scale(model, glm::vec3(0.5f, 0.9f, 0.9f));

		 glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		 color = glm::vec3(0.475, 0.604, 0.69);
		 glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		 meshList[0]->RenderMesh();

		 //NARIZ
		 model = modelBOQUITA;
		 model = glm::translate(model, glm::vec3(2.0f, -0.55f, -0.1f));

		 model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		 model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		 model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));

		 glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		 color = glm::vec3(0.125, 0.145, 0.18);
		 glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		 meshList[1]->RenderMesh();

		 //OJO 1
		 model = modelBOQUITA;
		 model = glm::translate(model, glm::vec3(1.35f, -0.3f, 0.8f));
		 model = glm::scale(model, glm::vec3(0.3f, 0.6f, 0.6f));

		 glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		 color = glm::vec3(0.0, 0.0, 0.0);
		 glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		 meshList[0]->RenderMesh();
		 
		 //OJO 2
		 model = modelBOQUITA;
		 model = glm::translate(model, glm::vec3(1.35f, -0.3f, -0.8f));
		 model = glm::scale(model, glm::vec3(0.3f, 0.6f, 0.6f));

		 glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		 color = glm::vec3(0.0, 0.0, 0.0);
		 glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		 meshList[0]->RenderMesh();

		//---------
		//GORRO CUMPLEAÑERO jiji
		//---------
		 model = modelOREJAS;

		 model = glm::translate(model, glm::vec3(0.0f, 2.1f, 0.0f));
		
		 model = glm::scale(model, glm::vec3(0.5f, 3.0f, 0.35f));

		 glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		 color = glm::vec3(0.235, 0.569, 0.478);
		 glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		 meshList[3]->RenderMeshGeometry();

		 //pompon
		 model = modelOREJAS;

		 model = glm::translate(model, glm::vec3(0.0f, 3.5f, 0.0f));

		 model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));

		 glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		 color = glm::vec3(0.137, 0.412, 0.337);
		 glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		 sp.render();

		//Oreja1
		 model = modelOREJAS;

		 model = glm::translate(model, glm::vec3(0.5f, 1.7f, 0.70f));

		 model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		 model = glm::scale(model, glm::vec3(1.55f, 1.55f, 1.55f));

		 glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		 color = glm::vec3(0.475, 0.604, 0.69);
		 glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		 meshList[1]->RenderMeshGeometry();
		
		 //Oreja2
		 model = modelOREJAS;

		 model = glm::translate(model, glm::vec3(0.5f, 1.7f, -0.70f));

		 model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		 model = glm::scale(model, glm::vec3(1.55f, 1.55f, 1.55f));

		 glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		 color = glm::vec3(0.475, 0.604, 0.69);
		 glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		 meshList[1]->RenderMeshGeometry();
	
		


		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}


	
		