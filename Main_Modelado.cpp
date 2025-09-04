//Practica 4
//Juan Enrique Soria palos
//Fecha de entrega: 04/09/2025
//422015639


#include<iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Shaders
#include "Shader.h"

void Inputs(GLFWwindow* window);

const GLint WIDTH = 800, HEIGHT = 600;
float movX = 0.0f;
float movY = 0.0f;
float movZ = -5.0f;
float rot = 0.0f;

int main() {
	glfwInit();

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 4 Juan Soria", nullptr, nullptr);

	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, screenWidth, screenHeight);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader ourShader("Shader/core.vs", "Shader/core.frag");

	// Vertices para un cubo genérico
	float vertices[] = {
		// Front
		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		// Back
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		// Right
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,

		 // Left
		 -0.5f,  0.5f,  0.5f,
		 -0.5f,  0.5f, -0.5f,
		 -0.5f, -0.5f, -0.5f,
		 -0.5f, -0.5f, -0.5f,
		 -0.5f, -0.5f,  0.5f,
		 -0.5f,  0.5f,  0.5f,

		 // Bottom
		 -0.5f, -0.5f, -0.5f,
		  0.5f, -0.5f, -0.5f,
		  0.5f, -0.5f,  0.5f,
		  0.5f, -0.5f,  0.5f,
		 -0.5f, -0.5f,  0.5f,
		 -0.5f, -0.5f, -0.5f,

		 // Top
		 -0.5f,  0.5f, -0.5f,
		  0.5f,  0.5f, -0.5f,
		  0.5f,  0.5f,  0.5f,
		  0.5f,  0.5f,  0.5f,
		 -0.5f,  0.5f,  0.5f,
		 -0.5f,  0.5f, -0.5f,
	};

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);

	// Variables de color para cada parte de Woody (en RGB)
	glm::vec3 blue(0.25f, 0.41f, 0.88f);          // Pantalones
	glm::vec3 yellow(1.0f, 0.85f, 0.0f);        // Camisa
	glm::vec3 red(0.85f, 0.1f, 0.1f);           // Pañuelo
	glm::vec3 skin(0.8f, 0.6f, 0.4f);          // Piel
	glm::vec3 brown(0.4f, 0.2f, 0.0f);         // Botas y sombrero
	glm::vec3 vestColor(1.0f, 1.0f, 1.0f);     // Chaleco (blanco)
	glm::vec3 black(0.0f, 0.0f, 0.0f);         // Ojos, nariz
	glm::vec3 belt(0.3f, 0.15f, 0.0f);         // Cinturón (marrón oscuro)
	glm::vec3 hair(0.4f, 0.2f, 0.0f);          // Cabello (marrón)
	glm::vec3 darkSkinColor = glm::vec3(0.6f, 0.4f, 0.2f); // café más oscuro


	while (!glfwWindowShouldClose(window))
	{
		Inputs(window);
		glfwPollEvents();

		glClearColor(0.6f, 0.8f, 0.6f, 1.0f); // Color de fondo verde
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.Use();
		glm::mat4 model = glm::mat4(1);
		glm::mat4 view = glm::mat4(1);

		view = glm::translate(view, glm::vec3(movX, movY, movZ));
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");
		GLint colorLoc = glGetUniformLocation(ourShader.Program, "inColor");

		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		glBindVertexArray(VAO);

		// Cuerpo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.3f, 0.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.8f, 0.3f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(yellow)); // Color amarillo para la camisa
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Parte trasera del chaleco
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.3f, -0.16f)); // atrás
		model = glm::scale(model, glm::vec3(0.6f, 0.8f, 0.05f));      // ancho casi total
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(vestColor));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Hombro izquierdo (encima de la manga amarilla)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.32f, 0.65f, 0.0f)); // se ajusta en X hacia el brazo
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.35f));     // pequeño bloque sobre hombro
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(vestColor));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Hombro derecho (encima de la otra manga)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.32f, 0.65f, 0.0f));  // simétrico al izquierdo
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.35f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(vestColor));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Frontal izquierdo (solo una franja, dejando espacio al centro)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.2f, 0.3f, 0.16f)); // lado izquierdo
		model = glm::scale(model, glm::vec3(0.2f, 0.8f, 0.05f));      // más angosto
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(vestColor));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Frontal derecho (otra franja al lado opuesto)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.2f, 0.3f, 0.16f));  // lado derecho
		model = glm::scale(model, glm::vec3(0.2f, 0.8f, 0.05f));      // igual de angosto
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(vestColor));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Manchas del chaleco (negras)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.1f, 0.4f, 0.19f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.01f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(black));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.2f, 0.2f, 0.19f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.01f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(black));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// ===================== MANCHAS NEGRAS DEL CHALECO =====================

	// ===================== MANCHAS NEGRAS DEL CHALECO =====================

// Mancha superior izquierda (frente)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.22f, 0.55f, 0.17f)); // más arriba y hacia el lado
		model = glm::scale(model, glm::vec3(0.1f, 0.15f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(black));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Mancha inferior izquierda (frente)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.18f, 0.15f, 0.17f)); // más abajo
		model = glm::scale(model, glm::vec3(0.12f, 0.1f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(black));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Mancha superior derecha (frente)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.22f, 0.45f, 0.17f)); // lado derecho más arriba
		model = glm::scale(model, glm::vec3(0.1f, 0.14f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(black));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Mancha inferior derecha (frente)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.18f, 0.2f, 0.17f)); // más abajo y separada
		model = glm::scale(model, glm::vec3(0.13f, 0.1f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(black));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// ===================== MANCHAS NEGRAS DEL CHALECO - TRASERA =====================

// Mancha superior izquierda (espalda)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.22f, 0.55f, -0.17f)); // misma que la frontal pero en Z negativa
		model = glm::scale(model, glm::vec3(0.1f, 0.15f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(black));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Mancha inferior izquierda (espalda)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.18f, 0.15f, -0.17f));
		model = glm::scale(model, glm::vec3(0.12f, 0.1f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(black));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Mancha superior derecha (espalda)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.22f, 0.45f, -0.17f));
		model = glm::scale(model, glm::vec3(0.1f, 0.14f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(black));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Mancha inferior derecha (espalda)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.18f, 0.2f, -0.17f));
		model = glm::scale(model, glm::vec3(0.13f, 0.1f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(black));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	

		// Mancha en la espalda (grande, centrada)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.45f, -0.17f)); // centro espalda
		model = glm::scale(model, glm::vec3(0.2f, 0.18f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(black));
		glDrawArrays(GL_TRIANGLES, 0, 36);



		// Cabeza
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(skin)); // Color de piel
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Ojos
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.08f, 1.05f, 0.21f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(black));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.08f, 1.05f, 0.21f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(black));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Nariz
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.98f, 0.21f));
		model = glm::scale(model, glm::vec3(0.08f, 0.05f, 0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(darkSkinColor));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Orejas
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.21f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.1f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(darkSkinColor));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.21f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.1f, 0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(darkSkinColor));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Cabello sobresaliente
		// Parte trasera
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.15f, -0.18f));  // detrás de la cabeza
		model = glm::scale(model, glm::vec3(0.42f, 0.12f, 0.05f));     // delgado y cubriendo la parte trasera
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(black));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Lateral izquierdo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.21f, 1.15f, 0.0f));  // al lado izquierdo
		model = glm::scale(model, glm::vec3(0.05f, 0.12f, 0.42f));     // delgado en X, cubriendo lateral
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(black));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Lateral derecho
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.21f, 1.15f, 0.0f));   // al lado derecho
		model = glm::scale(model, glm::vec3(0.05f, 0.12f, 0.42f));     // delgado en X, cubriendo lateral
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(black));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Pantalones
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.4f, 0.3f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(blue)); // Color azul para los pantalones
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Cinturón
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.20f, 0.0f));
		model = glm::scale(model, glm::vec3(0.55f, 0.05f, 0.35f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(belt)); // Color de cinturón
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Brazos
		// Brazo Izquierdo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.4f, 0.3f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.6f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(yellow));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Puño izquierdo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.4f, -0.05f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(skin));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Brazo Derecho
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.4f, 0.3f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.6f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(yellow));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Puño derecho
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.4f, -0.05f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(skin));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Piernas
		// Pierna Izquierda
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.1f, -0.8f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.8f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(blue));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Pierna Derecha
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.1f, -0.8f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.8f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(blue));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Botas
		// Bota Izquierda
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.1f, -1.3f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(brown));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Bota Derecha
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.1f, -1.3f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(brown));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Sombrero
		// Sombrero base
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.3f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.1f, 0.8f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(brown));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Sombrero copa
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.4f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.2f, 0.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(brown));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Cuello
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.7f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.1f, 0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(skin));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Pañuelo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.77f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.1f, 0.3f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(red));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glBindVertexArray(0);

		glfwSwapBuffers(window);

	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return EXIT_SUCCESS;
}

void Inputs(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movX += 0.0008f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX -= 0.0008f;
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		movY += 0.0008f;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		movY -= 0.0006f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movZ -= 0.0006f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movZ += 0.008f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rot += 0.04f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rot -= 0.04f;
}


