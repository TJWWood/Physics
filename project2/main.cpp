#pragma once
// Math constants
#define _USE_MATH_DEFINES
#include <cmath>  
#include <random>

// Std. Includes
#include <string>
#include <time.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include "glm/ext.hpp"

// Other Libs
#include "SOIL2/SOIL2.h"

// project includes
#include "Application.h"
#include "Shader.h"
#include "Mesh.h"


// time
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// main function
int main()
{
	// create application
	Application app = Application::Application();
	app.initRender();
	Application::camera.setCameraPosition(glm::vec3(0.0f, 5.0f, 20.0f));

	// create ground plane
	Mesh plane = Mesh::Mesh(Mesh::QUAD);
	// scale it up x5
	plane.scale(glm::vec3(5.0f, 5.0f, 5.0f));
	Shader lambert = Shader("resources/shaders/physics.vert", "resources/shaders/physics.frag");
	plane.setShader(lambert);


	// create particle
	Mesh particle1 = Mesh::Mesh(Mesh::QUAD);
	//scale it down (x.1), translate it up by 2.5 and rotate it by 90 degrees around the x axis
	particle1.translate(glm::vec3(0.0f, 2.0f, 0.0f));
	particle1.scale(glm::vec3(.1f, .1f, .1f));
	particle1.rotate((GLfloat)M_PI_2, glm::vec3(1.0f, 0.0f, 0.0f));
	particle1.setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));

	// create demo objects (a cube and a sphere)
	Mesh sphere = Mesh::Mesh("resources/models/sphere.obj");
	sphere.translate(glm::vec3(-1.0f, 1.0f, 0.0f));
	sphere.setShader(lambert);
	Mesh cube = Mesh::Mesh("resources/models/cube.obj");
	cube.translate(glm::vec3(1.0f, .5f, 0.0f));
	cube.setShader(lambert);

	// time
	GLfloat firstFrame = (GLfloat)glfwGetTime();
	glm::vec3 v = glm::vec3(1.5f, 2.5f, 2.0f);

	double t = 0.0;
	const double dt = 0.01;

	double currentTime = glfwGetTime();
	double accumulator = 0.0;

	// Game loop
	while (!glfwWindowShouldClose(app.getWindow()))
	{
		double newTime = glfwGetTime();
		double frameTime = newTime - currentTime;
		currentTime = newTime;

		accumulator += frameTime;

		while (accumulator >= dt)
		{
			/*
			**	INTERACTION
			*/
			// Manage interaction
			app.doMovement(dt);


			/*
			**	SIMULATION
			*/
			float mass = 1.0f;
			glm::vec3 g = glm::vec3(0.0f, -9.8f, 0.0f);
			glm::vec3 Fg = g * mass;
			glm::vec3 Fa = 0.5 * 1.225 * (length(v) * length(v)) * 1.05 * (particle1.getScale()[0][0] * particle1.getScale()[1][1]) * (-normalize(v));
			glm::vec3 F = Fg + Fa;
			glm::vec3 accel = F / mass;

			v = v + (dt * accel);

			//dimensions
			glm::vec3 dim = glm::vec3(5.0f, 5.0f, 5.0);
			//corner
			glm::vec3 corner = glm::vec3(0.0f, 0.0f, 0.0f);

			for (int i = 0; i < 3; i++)
			{
				if (particle1.getPos()[i] > (corner[i] + dim[i]))
				{
					v[i] *= -1.0f;

				}
				else if (particle1.getPos()[i] < (-corner[i] + -dim[i]))
				{
					v[i] *= -1.0f;
				}
				else
				{
					particle1.translate(v * dt);

				}
			}
			accumulator -= dt;
			t += dt;
		}

		/*
		**	RENDER
		*/
		// clear buffer
		app.clear();
		// draw groud plane
		app.draw(plane);
		// draw particles
		app.draw(particle1);

		// draw demo objects
		app.draw(cube);
		app.draw(sphere);

		app.display();
	}

	app.terminate();

	return EXIT_SUCCESS;
}