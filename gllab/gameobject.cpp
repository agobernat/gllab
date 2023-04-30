#include "gameobject.hpp"



GameObject::GameObject() 
{
	
}

GameObject::GameObject(const GameModel& model) {
	modelptr = &model;
	
}




void GameObject::updateTransformFromPhysics()
{
	
	
}

void GameObject::setCustomCollider(btVector3 origin, btScalar mass)
{
	
	btTransform groundTransform;
	groundTransform.setIdentity();
	const auto& translation = getTranslate();
	groundTransform.setOrigin(origin);
	std::cout << translation.x << " " << translation.y << " " << translation.z << std::endl;

	
	btVector3 localInertia(0, 0, 0);
	motionState = std::make_unique<btDefaultMotionState>(groundTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState.get(), modelptr->getCollisionShape(), localInertia);
	rigidBody = std::make_unique<btRigidBody>(rbInfo);
	rigidBody->setActivationState(DISABLE_DEACTIVATION);
	
}

void GameObject::setBoxColliderFromMesh()
{
	
	
	btTransform groundTransform;
	groundTransform.setIdentity();
	const auto& translation = getTranslate();
	groundTransform.setOrigin(btVector3(translation.x, translation.y, translation.z));
	std::cout << translation.x <<  " "  << translation.y << " " << translation.z << std::endl;

	btScalar mass(0.);
	btVector3 localInertia(0, 0, 0);
	motionState = std::make_unique<btDefaultMotionState>(groundTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState.get(), modelptr->getCollisionShape(), localInertia);
	rigidBody = std::make_unique<btRigidBody>(rbInfo);
	
	
}

void GameObject::scale(glm::dvec3 scale)
{
	transform.scale = glm::scale(transform.scale, scale);
}
void GameObject::translate(glm::dvec3 translate)
{
	transform.translate = glm::translate(transform.translate, translate);
	updateColliderTransform();
	
}

void GameObject::setTranslate(glm::dvec3 translate) {
	transform.translate = glm::translate(glm::dmat4(1.0), translate);
	updateColliderTransform();
	
}

void GameObject::updateColliderTransform()
{
	btTransform groundTransform;
	groundTransform.setIdentity();
	const auto& translation = getTranslate();
	groundTransform.setOrigin(btVector3(translation.x, translation.y, translation.z));

	rigidBody->setWorldTransform(groundTransform);
	rigidBody->updateInertiaTensor();
}

void GameObject::addColliderToDynamicsWorld(btDiscreteDynamicsWorld* dynamicsWorld)
{
	dynamicsWorld->addRigidBody(rigidBody.get());
}

glm::dvec3 GameObject::getTranslate()
{
	return glm::dvec3(transform.translate[3][0], transform.translate[3][1], transform.translate[3][2]);
	
}

void GameObject::setTransformMat(Transform transform)
{
	this->transform = transform;
	updateColliderTransform();
	
}

glm::dvec3 GameObject::getTransformVec() {
	return glm::dvec3(transform.translate[3][0], transform.translate[3][1], transform.translate[3][2]);
}

void GameObject::setRotation(double angle, glm::dvec3 rot) {
	transform.rotation = glm::rotate(glm::dmat4(1.0), angle, rot);
}

void GameObject::rotate(double angle, glm::dvec3 axis) {
	transform.rotation = glm::rotate(transform.rotation, angle, axis);
}

/*void GameObject::setSprite(Sprite sprite) {
	this->sprite = sprite;
}*/

void GameObject::draw(const Camera& camera) const
{
	modelptr->draw(camera, transform);
}

void GameObject::normalizeSize()
{
	auto scalingfactor = modelptr->getScalingFactorFromAccessors();
	transform.scale = glm::scale(transform.scale, glm::dvec3(scalingfactor, scalingfactor, scalingfactor));
}

