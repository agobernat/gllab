#include "transform.hpp"
#include <glm/ext/matrix_transform.hpp>


Transform::Transform() {
	
	translation = glm::dmat4(1.0);
	scaling = glm::dmat4(1.0);
	rotation = glm::dmat4(1.0);

	btransform.setIdentity();
}

void Transform::translate(glm::dvec3 translate)
{
	 translation = glm::translate(translation, translate);
	 btransform.setOrigin(glmTobtVec3(getTranslate()));

}

void Transform::setTranslate(glm::dvec3 translate)
{
	translation = glm::translate(glm::dmat4(1.0), translate);
	btransform.setOrigin(glmTobtVec3(getTranslate()));
}

void Transform::setTranslate(glm::dmat4 translationMatrix)
{
	translation = translationMatrix;
	btransform.setOrigin(glmTobtVec3(getTranslate()));
}

 glm::dvec3 Transform::getTranslate() const
{
	return glm::dvec3(translation[3][0], translation[3][1], translation[3][2]);
}

 glm::dmat4 Transform::getTranslateMat() const
{
	 return translation;
}

 void Transform::rotate(double angle, glm::dvec3 axis)
{
	 rotation =  glm::rotate(rotation, angle, axis);
}

 void Transform::setRotation(double angle, glm::dvec3 axis)
{
	 rotation = glm::rotate(glm::dmat4(1.0), angle, axis);
}

 void Transform::setRotation(glm::dmat4 rotationMatrix)
{
	 rotation = rotationMatrix;
}

 glm::dmat4 Transform::getRotationMat() const
{
	 return rotation;
}

 void Transform::scale(glm::dvec3 scale)
{
	 scaling = glm::scale(scaling, scale);
}

 void Transform::setScale(glm::dvec3 scale)
{
	 scaling = glm::scale(glm::dmat4(1.0), scale);
}

 void Transform::setScale(glm::dmat4 scaleMatrix)
{
	 scaling = scaleMatrix;
}

 glm::dvec3 Transform::getScale() const
{
	return glm::dvec3(translation[0][0], translation[1][1], translation[2][2]);
}

 glm::dmat4 Transform::getScaleMat() const
{
	 return scaling;
}

 btVector3 Transform::glmTobtVec3(glm::dvec3 vec)
 {
	 return btVector3(vec.x, vec.y, vec.z);
 }

 glm::dvec3 Transform::btToglmVec3(btVector3 vec)
 {
	 return glm::dvec3(vec.x(), vec.y(), vec.z());
 }

 btTransform& Transform::getbtTransform()
 {
	 return btransform;
 }
