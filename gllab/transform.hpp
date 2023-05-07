#pragma once
#include <glm/glm.hpp>
#include "bullet/btBulletCollisionCommon.h"

class Transform
{
public:
	Transform();
	

	void translate(glm::dvec3 translate);
	void setTranslate(glm::dvec3 translate);
	void setTranslate(glm::dmat4 translationMatrix);
	glm::dvec3 getTranslate() const;
	glm::dmat4 getTranslateMat() const;

	void rotate(double angle, glm::dvec3 axis);
	void setRotation(double angle, glm::dvec3 axis);
	void setRotation(glm::dmat4 rotationMatrix);
	glm::dmat4 getRotationMat() const;

	void scale(glm::dvec3 scale);
	void setScale(glm::dvec3 scale);
	void setScale(glm::dmat4 scaleMatrix);
	glm::dvec3 getScale() const;
	glm::dmat4 getScaleMat() const;

	static btVector3 glmTobtVec3(glm::dvec3 vec);
	static glm::dvec3 btToglmVec3(btVector3 vec);

	btTransform& getbtTransform();

private:
	glm::dmat4 translation, rotation, scaling;
	btTransform btransform;
};




