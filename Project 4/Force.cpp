#include <iostream>
#include <cmath>
#include "Force.h"
#include "Body.h"
#include "glm/ext.hpp"

glm::vec3 Force::apply(float mass, const glm::vec3 &pos, const glm::vec3 &vel)
{
	return glm::vec3(0.0f);
}

glm::vec3 Gravity::apply(float mass, const glm::vec3 &pos, const glm::vec3 &vel)
{
	return getGravity() * mass;
}

//glm::vec3 Drag::apply(float mass, const glm::vec3 &pos, const glm::vec3 &vel)
//{

//}

glm::vec3 Hooke::apply(float mass, const glm::vec3 &pos, const glm::vec3 &vel)
{
	float ks = getKs();
	float kd = getKd();
	float rest = getRest();
	Body* b1 = getB1();
	Body* b2 = getB2();

	glm::vec3 l = normalize(b2->getPos() - b1->getPos());
	glm::vec3 e = (b2->getPos() - b1->getPos()) / l;
	glm::vec3 v1 = e * b1->getVel();
	glm::vec3 v2 = e * b2->getVel();

	glm::vec3 fs = -ks * (rest - l);
	glm::vec3 fd = -kd * (v1 - v2);
	glm::vec3 hooke = fs - fd;

	return hooke * e;
}