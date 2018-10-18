#pragma once
#include <glm/glm.hpp>
#include <iostream>

class Body; // forward declaration to avoid circular dependencies

class Force
{
public:
	Force() {}
	~Force() {}

	virtual glm::vec3 apply(float mass, const glm::vec3 & pos, const glm::vec3 & vel);
};


/*
** GRAVITY CLASS
*/
class Gravity : public Force {

public:
	// constructors
	Gravity() {}
	Gravity(const glm::vec3 & gravity) { m_gravity = gravity; }

	// get and set methods
	glm::vec3 getGravity() const { return m_gravity; }
	void setGravity(glm::vec3 gravity) { m_gravity = gravity; }

	// physics
	glm::vec3 apply(float mass, const glm::vec3 & pos, const glm::vec3 & vel);

private:
	glm::vec3 m_gravity = glm::vec3(0.0f, -9.8f, 0.0f);
};

/*
** DRAG CLASS
*/
class Drag : public Force {
public:
	Drag() {}
	// physics
	glm::vec3 apply(float mass, const glm::vec3 & pos, const glm::vec3 & vel);

private:

};

class Hooke : public Force {
public:
	Hooke() {}
	Hooke(Body * b1, Body* b2, float ks, float kd, float rest) {
		m_ks = ks; m_kd = kd; m_rest = rest; m_b1 = b1; m_b2 = b2;
	}

	//glm::vec3 getHooke() const { return m_ks; }
	//void setHooke(glm::vec3 ks) { m_ks = ks; }

	glm::vec3 apply(float mass, const glm::vec3 &pos, const glm::vec3 &vel);

private:
	float m_ks;
	float m_kd;
	float m_rest;

	Body* m_b1;
	Body* m_b2;
};
