#pragma once
#include "bullet/btBulletDynamicsCommon.h"
#include "include/glad/glad.h"
#include "include/GLFW/glfw3.h"
#include "shadermanager.hpp"
#include "globals.hpp"
#include "include/glm/glm.hpp"

class DebugDrawer : public btIDebugDraw {
public:
	DebugDrawer();

	void setMVP(glm::mat4 m, glm::mat4 v, glm::mat4 p);
	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	virtual inline void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) { (void)PointOnB; (void)normalOnB; (void)distance; (void)lifeTime; (void)color; }
	virtual inline void reportErrorWarning(const char* warningString) { (void)warningString; }
	virtual inline void draw3dText(const btVector3& location, const char* textString) { (void)location; (void)textString; }
	virtual inline void setDebugMode(int debugMode) { debugModes = (DebugDrawModes)debugMode; }
	virtual inline int getDebugMode() const { return debugModes; }
private:
	float vertices[6];
	GLuint vao, vbo;
	const Shader* shader;
	DebugDrawModes debugModes;
	glm::dmat4 m, v, p;

};