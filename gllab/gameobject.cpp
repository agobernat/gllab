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
	collobjtype = "rigidbody";
	collobj = std::make_unique<btRigidBody>(rbInfo);
	collobj->setActivationState(DISABLE_DEACTIVATION);
	collobj->setFriction(0);
	
}

void GameObject::setCustomTriggerCollider(btVector3 origin)
{
	btTransform groundTransform;
	groundTransform.setIdentity();
	auto translation = transform.getTranslate();
	groundTransform.setOrigin(origin);

	btVector3 localInertia(0, 0, 0);
	motionState = std::make_unique<btDefaultMotionState>(groundTransform);


	btRigidBody::btRigidBodyConstructionInfo rbInfo(.1, motionState.get(), modelptr->getCollisionShape(), localInertia);
	collobjtype = "trigger";
	collobj = std::make_unique<btRigidBody>(rbInfo);
	collobj->setActivationState(DISABLE_DEACTIVATION);
	collobj->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
	collobj->setFriction(0);
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
	collobjtype = "rigidbody";
	collobj = std::make_unique<btRigidBody>(rbInfo);
	collobj->setContactStiffnessAndDamping(10000, 10000);
	
	
}

bool GameObject::isKinematic()
{
	if (collobjtype == "rigidbody")
	{
		if (static_cast<btRigidBody*>(collobj.get())->getMass() == 0.0)
		{
			return true;
		}
	}
	
	return false;
}

void GameObject::setVelocity(glm::vec3 velocity)
{
	if (collobjtype == "rigidbody")
	{
		static_cast<btRigidBody*>(collobj.get())->setLinearVelocity(Transform::glmTobtVec3(velocity));
	}
}

glm::vec3 GameObject::getVelocity()
{
	return glm::vec3(Transform::btToglmVec3(static_cast<btRigidBody*>(collobj.get())->getLinearVelocity()));
}

void GameObject::addVelocity(glm::vec3 velocity)
{
	auto vel = getVelocity();
	vel += velocity;
	setVelocity(vel);
}

void GameObject::setUserIndex(int index)
{
	collobj->setUserIndex(index);
}

int GameObject::getUserIndex()
{
	return collobj->getUserIndex();
}




void GameObject::updatePhysicsTransformFromGraphics()
{
	btTransform groundTransform;
	groundTransform.setIdentity();
	auto translation = transform.getTranslate();
	groundTransform.setOrigin(btVector3(translation.x, translation.y, translation.z));
	
	collobj->setWorldTransform(groundTransform);
	if (collobjtype == "rigidbody")
	{
		static_cast<btRigidBody*>(collobj.get())->updateInertiaTensor();
	}
	
}

void GameObject::updateGraphicsTransformFromPhysics()
{
	const auto& physicstransform = collobj->getWorldTransform();
	transform.setTranslate(Transform::btToglmVec3(physicstransform.getOrigin()));
	//transform.setRotation(Transf)
}

void GameObject::addColliderToDynamicsWorld(btDynamicsWorld* dynamicsWorld)
{
	collobj->setUserPointer(static_cast<void*>(collobj.get()));
	if (collobjtype == "rigidbody")
	{
		dynamicsWorld->addRigidBody(static_cast<btRigidBody*>(collobj.get()));
		collobj->setUserIndex(4);
	}
	else if (collobjtype == "trigger") 
	{
		dynamicsWorld->addCollisionObject(collobj.get());
		collobj->setUserIndex(5);
	}
	
	
}

btCollisionObject* GameObject::getCollisionBody()
{
	return collobj.get();
}


void GameObject::draw(const Camera& camera) const
{
	modelptr->draw(camera, transform, triggercolor);
}

void GameObject::normalizeSize()
{
	//auto scalingfactor = modelptr->getScalingFactorFromAccessors();
	auto scalingfactor = 0.5;
	transform.scale(glm::dvec3(scalingfactor, scalingfactor, scalingfactor));
	
}

