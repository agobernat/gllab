#include "gameobject.hpp"



GameObject::GameObject() 
{
	
}

GameObject::GameObject(GameModel& model) {
	modelptr = &model;
	
}


Transform& GameObject::getTransform()
{
	return transform;
}

void GameObject::setTransform(Transform transform)
{
	this->transform = transform;
}

void GameObject::updateTransformFromPhysics()
{
	
	
}

void GameObject::setCustomCollider(btVector3 origin, btScalar mass, btScalar friction)
{
	
	btTransform groundTransform;
	groundTransform.setIdentity();
	auto translation = transform.getTranslate();
	groundTransform.setOrigin(origin);

	btVector3 localInertia(0, 0, 0);
	motionState = std::make_unique<btDefaultMotionState>(groundTransform);

	
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState.get(), modelptr->getCollisionShape(), localInertia);
	rigidBody = std::make_unique<btRigidBody>(rbInfo);
	rigidBody->setActivationState(DISABLE_DEACTIVATION);
	rigidBody->setFriction(friction);
	
	
	
	
	
}

void GameObject::setBoxColliderFromMesh()
{
	
	
	btTransform groundTransform;
	groundTransform.setIdentity();
	auto translation = transform.getTranslate();
	groundTransform.setOrigin(btVector3(translation.x, translation.y, translation.z));

	btScalar mass(0.);
	btVector3 localInertia(0, 0, 0);
	motionState = std::make_unique<btDefaultMotionState>(groundTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState.get(), modelptr->getCollisionShape(), localInertia);
	rigidBody = std::make_unique<btRigidBody>(rbInfo);
	rigidBody->setContactStiffnessAndDamping(10000, 10000);
	
	
}

bool GameObject::isKinematic()
{
	if (rigidBody->getMass() == 0.0)
	{
		return true;
	}
	return false;
}

void GameObject::setVelocity(glm::vec3 velocity)
{
	rigidBody->setLinearVelocity(Transform::glmTobtVec3(velocity));
}

glm::vec3 GameObject::getVelocity()
{
	return glm::vec3(Transform::btToglmVec3(rigidBody->getLinearVelocity()));
}

void GameObject::addVelocity(glm::vec3 velocity)
{
	auto vel = getVelocity();
	vel += velocity;
	setVelocity(vel);
}




void GameObject::updatePhysicsTransformFromGraphics()
{
	btTransform groundTransform;
	groundTransform.setIdentity();
	auto translation = transform.getTranslate();
	groundTransform.setOrigin(btVector3(translation.x, translation.y, translation.z));
	
	rigidBody->setWorldTransform(groundTransform);
	rigidBody->updateInertiaTensor();
}

void GameObject::updateGraphicsTransformFromPhysics()
{
	const auto& physicstransform = rigidBody->getWorldTransform();
	transform.setTranslate(Transform::btToglmVec3(physicstransform.getOrigin()));
	//transform.setRotation(Transf)
}

void GameObject::addColliderToDynamicsWorld(btDiscreteDynamicsWorld* dynamicsWorld)
{
	dynamicsWorld->addRigidBody(rigidBody.get());
}


void GameObject::draw(const Camera& camera) const
{
	modelptr->draw(camera, transform);
}

void GameObject::normalizeSize()
{
	auto scalingfactor = modelptr->getScalingFactorFromAccessors();
	transform.scale(glm::dvec3(scalingfactor, scalingfactor, scalingfactor));
	
}

