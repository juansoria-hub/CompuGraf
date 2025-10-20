//Practica 9
//Soria Palos Juan Enrique
//Fecha de entrega: 19/10/2025
//422015639
//Fuentes de luz

#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera camera(glm::vec3(0.0f, 1.5f, 5.0f)); // Posición inicial de la cámara ajustada
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
bool spotlightOn = true; // Variable para controlar el foco

// Positions of the point lights
// pointLights[0] = Vela
// pointLights[1] = Farol
// pointLights[2] = Luz Decorativa 1 (Verde)
// pointLights[3] = Luz Decorativa 2 (Azul)
//glm::vec3 pointLightPositions[] = {
//	glm::vec3(-1.5f, 0.7f, 0.8f),   // Posición para la vela
//	glm::vec3(2.5f, 1.8f, -1.0f),    // Posición para el farol
//	glm::vec3(0.0f, 0.5f, 2.0f),     // Posición para la nueva luz verde
//	glm::vec3(1.5f, 0.5f, 1.5f),     // Posición para la nueva luz azul
//	glm::vec3(0.0f, 0.0f, 0.0f),     // No usada
//	glm::vec3(0.0f, 0.0f, 0.0f),     // No usada
//	glm::vec3(0.0f, 0.0f, 0.0f),     // No usada
//	glm::vec3(0.0f, 0.0f, 0.0f)      // No usada
//};

float vertices[] = {
	// Coordenadas de vértices (sin cambios)
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f; 	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 9 Juan Soria", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);

	// Build and compile our shader program
	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");
	
	// Carga de modelos (sin cambios)
	Model Fox((char*)"Models/Fox.obj");
	Model Floor((char*)"Models/Floor.obj");
	Model Corgi((char*)"Models/Corgi.obj");
	Model Lamp((char*)"Models/Lamp.obj");
	Model Pizza((char*)"Models/Pizza.obj");
	Model Cloth((char*)"Models/Cloth.obj");
	Model Table((char*)"Models/Table.obj");
	Model Candle((char*)"Models/Candle.obj");
	Model Farol((char*)"Models/Farol.obj");

	// 1. POSICIONES BASE DE LOS MODELOS
	// (Estas son las posiciones que ya tenías, representan la base de cada modelo)
	glm::vec3 posCandle = glm::vec3(0.0f, 0.0f, 0.0f); // Base de la vela (sobre la mesa)
	glm::vec3 posFarol = glm::vec3(0.0f, 0.0f, 0.0f);   // Base del farol (en el piso)
	glm::vec3 posLamp = glm::vec3(0.0f, 0.0f, 0.0f);     // Base de la lámpara (sobre la mesa)

	// 2. OFFSETS DE LUZ (¡AJUSTA ESTOS VALORES!)
	// Esta es la posición de la 'bombilla' o 'llama' RELATIVA a la base del modelo.
	// (x, y, z) -> (0, 0, 0) es la base del modelo.
	glm::vec3 offsetCandle = glm::vec3(-2.55f, 3.2f, 2.55f); // Sube 0.4 en Y para llegar a la llama
	glm::vec3 offsetFarol = glm::vec3(8.2f, 8.0f, -14.3f); // Sube 1.8 en Y para llegar a la bombilla
	glm::vec3 offsetLamp = glm::vec3(-5.3f, 3.0f, -1.5f); // Sube 0.6 en Y y un poco en Z (hacia donde alumbra)
	

	// 3. ARRAY DE POSICIONES DE LUZ
	// Este array se actualizará en cada frame dentro del loop.
	// Lo inicializamos aquí para que las luces decorativas tengan un valor.
	glm::vec3 pointLightPositions[] = {
		posCandle + offsetCandle, // Posición inicial vela
		posFarol + offsetFarol,   // Posición inicial farol
		posLamp + offsetLamp,     // Posición inicial lámpara
		glm::vec3(1.5f, 0.5f, 1.5f) // Luz decorativa azul
	};


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);


	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// VELA (Candle)
		glm::mat4 modelCandle = glm::mat4(1.0f);
		//modelCandle = glm::translate(modelCandle, posCandle);
		
		// La posición MUNDIAL de la luz es la matriz del modelo * el offset
		glm::vec3 lightPosCandle = glm::vec3(modelCandle * glm::vec4(offsetCandle, 1.0f));

		// FAROL
		glm::mat4 modelFarol = glm::mat4(1.0f);
		//modelFarol = glm::translate(modelFarol, posFarol);

		glm::vec3 lightPosFarol = glm::vec3(modelFarol * glm::vec4(offsetFarol, 1.0f));

		// LÁMPARA (Lamp)
		glm::mat4 modelLamp = glm::mat4(1.0f);
		//modelLamp = glm::translate(modelLamp, posLamp);

		glm::vec3 lightPosLamp = glm::vec3(modelLamp * glm::vec4(offsetLamp, 1.0f));

		// Actualizar el array de posiciones para el shader y las esferas de debug
		pointLightPositions[0] = lightPosCandle;
		pointLightPositions[1] = lightPosFarol;
		pointLightPositions[2] = lightPosLamp;

	
		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		// == CONFIGURACIÓN DE LUCES ==

		// Directional light (Luz de luna muy tenue)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.08f, 0.08f, 0.08f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.1f, 0.1f, 0.1f);

		// Point light 1: VELA
		// ¡MODIFICADO! Usa la nueva variable 'lightPosCandle'
		// Color: Naranja y cálido. Alcance: Medio.
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), lightPosCandle.x, lightPosCandle.y, lightPosCandle.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.1f, 0.05f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 1.0f, 0.5f, 0.1f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.5f, 0.5f, 0.5f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.14f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.07f);

		// Point light 2: FAROL
		// ¡MODIFICADO! Usa la nueva variable 'lightPosFarol'
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), lightPosFarol.x, lightPosFarol.y, lightPosFarol.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.2f, 0.2f, 0.1f);    // <-- MODIFICADO (Más luz ambiental)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 2.0f, 2.0f, 1.2f);   // <-- MODIFICADO (¡Mucho más intenso!)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 2.0f, 2.0f, 2.0f);  // <-- MODIFICADO (¡Brillo especular intenso!)
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.45f); // (Mismo gran alcance)
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.75f);// (Mismo gran alcance)

		// Point light 3: LÁMPARA (Era la luz verde, la ajusté a blanco cálido)
		// ¡MODIFICADO! Usa la nueva variable 'lightPosLamp'
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), lightPosLamp.x, lightPosLamp.y, lightPosLamp.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), 0.0f, -1.0f, 0.0f);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.18f); // <-- MODIFICADO (Más luz ambiental)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 1.8f, 1.8f, 1.5f);  // <-- MODIFICADO (¡Mucho más intenso!)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 2.0f, 2.0f, 2.0f); // <-- MODIFICADO (¡Brillo especular intenso!)

		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(20.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(35.0f)));

		// Point light 4: LUZ DECORATIVA AZUL
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.0f, 0.0f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 0.1f, 0.1f, 0.8f); // Color azul
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.2f, 0.2f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.14f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.07f);

		// SpotLight: FOCO (Linterna de la cámara)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), lightPosLamp.x, lightPosLamp.y, lightPosLamp.z); // <-- MODIFICADO (Usa la posición de la lámpara)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), 0.0f, -1.0f, 0.0f); // <-- MODIFICADO (Apunta hacia abajo)
		
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.1f, 0.1f, 0.08f); // <-- MODIFICADO (Blanco cálido)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.9f, 0.9f, 0.7f);  // <-- MODIFICADO (Blanco cálido)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);

		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(20.0f)));      // <-- MODIFICADO (Ángulo más abierto)
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(35.0f))); // <-- MODIFICADO (Borde más suave y abierto)

		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(17.5f)));
		
		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model(1);

		//Carga de modelo 
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Floor.Draw(lightingShader);

		model = glm::mat4(1);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		
		Fox.Draw(lightingShader);
		Corgi.Draw(lightingShader);
		Table.Draw(lightingShader);
		Cloth.Draw(lightingShader);
		Pizza.Draw(lightingShader);
		
		// Dibuja la VELA
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCandle));
		Candle.Draw(lightingShader);

		// Dibuja la LÁMPARA
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelLamp));
		Lamp.Draw(lightingShader);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelFarol));
		Farol.Draw(lightingShader);

		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		glDisable(GL_BLEND);
		glBindVertexArray(0);

	
		// Dibuja los objetos que representan las luces (lámparas)
		lampShader.Use();
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		for (GLuint i = 0; i < 4; i++)
		{
			model = glm::mat4(1);
			// ¡Esto usará las posiciones MUNDIALES actualizadas!
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model)); // Pasamos la matriz 'model'
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	// Camera controls (sin cambios)
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}
	// Control para encender/apagar el foco (linterna) con la tecla ESPACIO
	if (keys[GLFW_KEY_SPACE] && action == GLFW_PRESS)
	{
		spotlightOn = !spotlightOn;
	}
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}
	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left
	lastX = xPos;
	lastY = yPos;
	camera.ProcessMouseMovement(xOffset, yOffset);
}