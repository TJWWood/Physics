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


	// create particle
	Particle particle1;
	particle1.setMesh(Mesh::Mesh(Mesh::QUAD));
	//scale it down (x.1), translate it up by 2.5 and rotate it by 90 degrees around the x axis
	particle1.translate(glm::vec3(-1.0f, 2.0f, 0.2f));
	particle1.scale(glm::vec3(.1f, .1f, .1f));
	particle1.rotate((GLfloat)M_PI_2, glm::vec3(1.0f, 0.0f, 0.0f));
	particle1.getMesh().setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));

	Particle particle2;
	particle2.setMesh(Mesh::Mesh(Mesh::QUAD));
	//scale it down (x.1), translate it up by 2.5 and rotate it by 90 degrees around the x axis
	particle2.translate(glm::vec3(1.0f, 2.0f, 0.0f));
	particle2.scale(glm::vec3(.1f, .1f, .1f));
	particle2.rotate((GLfloat)M_PI_2, glm::vec3(1.0f, 0.0f, 0.0f));
	particle2.getMesh().setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));

	Particle particle3;
	particle3.setMesh(Mesh::Mesh(Mesh::QUAD));
	//scale it down (x.1), translate it up by 2.5 and rotate it by 90 degrees around the x axis
	particle3.translate(glm::vec3(-3.0, 2.0f, 0.0f));
	particle3.scale(glm::vec3(.1f, .1f, .1f));
	particle3.rotate((GLfloat)M_PI_2, glm::vec3(1.0f, 0.0f, 0.0f));
	particle3.getMesh().setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));

	// create demo objects (a cube and a sphere)
	Mesh sphere = Mesh::Mesh("resources/models/sphere.obj");
	sphere.translate(glm::vec3(-1.0f, 1.0f, 0.0f));
	sphere.setShader(lambert);
	Mesh cube = Mesh::Mesh("resources/models/cube.obj");
	cube.translate(glm::vec3(1.0f, .5f, 0.0f));
	cube.setShader(lambert);

	// time
	GLfloat firstFrame = (GLfloat)glfwGetTime();
	particle1.setVel(glm::vec3(0.0f, 0.0f, 0.0f));
	glm::vec3 v2 = glm::vec3(0.0f, 0.0f, 0.0f);

	double t = 0.0;
	const double dt = 0.01;

	double currentTime = glfwGetTime();
	double accumulator = 0.0;
	//dimensions
	glm::vec3 dim = glm::vec3(7.0f, 7.0f, 7.0f);
	//corner
	glm::vec3 corner = glm::vec3(0.0f, 0.0f, 0.0f);
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
			glm::vec3 g = glm::vec3(0.0f, -9.8f, 0.0f);
			particle1.setMass(1.0);
			glm::vec3 Fg = g * particle1.getMass();
			particle1.setAcc(Fg / particle1.getMass());


			glm::vec3 g2 = glm::vec3(0.0f, -9.8f, 0.0f);
			particle2.setMass(1.0);
			glm::vec3 Fg2 = g2 * particle2.getMass();
			particle2.setAcc(Fg2 / particle2.getMass());

			//v1 forward euler
			particle1.setVel(particle1.getVel() + (dt * particle1.getAcc()));
			//v2 simplicit euler
			particle2.setVel(particle2.getVel() + (dt * particle2.getAcc()));



			//glm::vec3 leftUpWind = glm::vec3(1.6f, -1.1f, 0.0f);

			//glm::vec3 rightUpWind = glm::vec3(1.6f, -1.1f, 0.0f);

			for (int j = 0; j < 3; j++)
			{
				if (particle1.getPos()[j] > (corner[j] + dim[j]))
				{
					particle1.getVel()[j] *= -0.999f;
				}
				else if (particle1.getPos()[j] < (-corner[j] + -dim[j]))
				{
					particle1.getVel()[j] *= -0.999f;
				}
				else
				{
					particle1.translate(particle1.getVel() * dt);
				}

				if (particle2.getPos()[j] > (corner[j] + dim[j]))
				{
					particle2.getVel()[j] *= -0.999f;
				}
				else if (particle2.getPos()[j] < (-corner[j] + -dim[j]))
				{
					particle2.getVel()[j] *= -0.999f;
				}
				else
				{
					particle2.translate(particle2.getVel() * dt);
				}
			}

			if (particle1.getPos()[0] > -1.6f && particle1.getPos()[0] <= 1.6f && (particle1.getPos()[1] < -1.0f && particle1.getPos()[1] >= -5.0f) && (particle1.getPos()[2] >= -3.0f && particle1.getPos()[2] <= 3.0f))
			{
				glm::vec3 wind = glm::vec3(particle1.getPos()[0], 6.0f, sin(particle1.getPos()[2]) + 2.0f);
				particle1.getVel() = wind;
			}
			else
			{
				particle1.translate(particle1.getVel() * dt);
			}

			if (particle2.getPos()[0] > -1.6f && particle2.getPos()[0] <= 1.6f && (particle2.getPos()[1] < -1.0f && particle2.getPos()[1] >= -4.0f) && particle2.getPos()[2] >= -3.0f && particle2.getPos()[2] <= 3.0f)
			{
				glm::vec3 wind = glm::vec3(particle2.getPos()[0], 6.0f, sin(particle2.getPos()[2]) + 2.0f);
				particle2.getVel() = wind;
			}
			else
			{
				particle2.translate(particle2.getVel() * dt);
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
		app.draw(particle1.getMesh());
		app.draw(particle2.getMesh());
		app.draw(particle3.getMesh());

		// draw demo objects
		//app.draw(cube);
		//app.draw(sphere);

		app.display();
	}

	app.terminate();

	return EXIT_SUCCESS;
}