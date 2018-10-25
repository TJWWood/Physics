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
#include "Body.h"
#include "Particle.h"


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

	// time
	GLfloat firstFrame = (GLfloat)glfwGetTime();

	double t = 0.0;
	const double dt = 0.01;

	double currentTime = glfwGetTime();
	double accumulator = 0.0;

	Gravity g = Gravity(glm::vec3(0.0f, -9.8f, 0.0f));
	Particle particle[10][10];
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			particle[i][j].setMesh(Mesh::Mesh(Mesh::QUAD));
			//scale it down (x.1), translate it up by 2.5 and rotate it by 90 degrees around the x axis
			particle[i][j].translate(glm::vec3(5 - i, 10.0f, 0.0f));

			particle[i][j].scale(glm::vec3(.1f, .1f, .1f));
			particle[i][j].rotate((GLfloat)M_PI_2, glm::vec3(1.0f, 0.0f, 0.0f));
			particle[i][j].getMesh().setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));
			particle[i][j].setVel(glm::vec3(0.0f, 0.0f, 0.0f));
			particle[i][j].setMass(1.0f);
			particle[i][j].addForce(&g);
		}
	}

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			Hooke* hooke = new Hooke();
			hooke->setKs(15.0f);
			hooke->setKd(8.0f);
			hooke->setRest(0.5f);
			hooke->setB1(&particle[i][j]);
			hooke->setB2(&particle[i + 1][j + 1]);

			particle[i][j].addForce(hooke);
		}

	}

	for (int i = 1; i < 10; i++)
	{
		for (int j = 1; j < 9; j++)
		{
			Hooke* hooke = new Hooke();
			hooke->setKs(15.0f);
			hooke->setKd(8.0f);
			hooke->setRest(0.5f);
			hooke->setB1(&particle[i][j]);
			hooke->setB2(&particle[i - 1][j - 1]);

			particle[i][j].addForce(hooke);
		}
	}

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
			for (int i = 0; i < 9; i++) {
				for (int j = 0; j < 9; j++)
				{
					if (particle[i][j].getPos().y <= plane.getPos().y)
					{
						particle[i][j].setVel(glm::vec3(0.0f, 0.0f, 0.0f));
					}
					else
					{
						particle[i][j].setAcc(particle[i][j].applyForces(particle[i][j].getPos(), particle[i][j].getVel(), t, dt));

						particle[i][j].setVel(particle[i][j].getVel() + (dt * particle[i][j].getAcc()));

						particle[i][j].translate(particle[i][j].getVel() * dt);
					}
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
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				app.draw(particle[i][j].getMesh());
			}
		}
		//app.draw(particle1.getMesh());
		//app.draw(particle2.getMesh());

		// draw demo objects
		//app.draw(cube);
		//app.draw(sphere);

		app.display();
	}

	app.terminate();

	return EXIT_SUCCESS;
}