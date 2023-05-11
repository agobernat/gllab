#pragma once

#include "sprite.hpp"
#include <glm/glm.hpp>
#include "model.hpp"
#include "include/bullet/btBulletDynamicsCommon.h"
#include "transform.hpp"

class GameModel;

class GameObject
{

private:
	
	Transform transform;
	const GameModel* modelptr;
	std::unique_ptr<btRigidBody> rigidBody;
	std::unique_ptr<btDefaultMotionState> motionState;


	

public:

	GameObject();
	GameObject(const GameModel& model);

	
	std::string objecttype;

	Transform& getTransform();
	void setTransform(Transform transform);

	void updateTransformFromPhysics();
	void setCustomCollider(btVector3 origin, btScalar mass, btScalar friction);
	void setBoxColliderFromMesh();

	bool isKinematic();
	void setVelocity(glm::vec3 velocity);
	glm::vec3 getVelocity();
	void addVelocity(glm::vec3 velocity);
	
	void updatePhysicsTransformFromGraphics();
	void updateGraphicsTransformFromPhysics();
	void addColliderToDynamicsWorld(btDiscreteDynamicsWorld* dynamicsWorld);

	void draw(const Camera& camera) const;
	void normalizeSize();
	//glm::mat4 makeTransform(const Transform& transform, glm::mat4 model = glm::mat4(1.0));
	//void setSprite(Sprite sprite);

};


