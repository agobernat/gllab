#pragma once

#include "sprite.hpp"
#include <glm/glm.hpp>
#include "model.hpp"
#include "include/bullet/btBulletDynamicsCommon.h"
#include "transform.hpp"
#include "include/bullet/btBulletCollisionCommon.h"
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

class GameModel;

class GameObject
{

private:
	
	Transform transform;
	GameModel* modelptr;

	std::unique_ptr<btCollisionObject> collobj;
	std::string collobjtype;
	
	
	std::unique_ptr<btDefaultMotionState> motionState;


	

public:

	GameObject();
	GameObject(GameModel& model);

	double blocktimer = 0;
	std::string objecttype;
	glm::vec4 triggercolor;
	Transform& getTransform();
	void setTransform(Transform transform);

	void updateTransformFromPhysics();
	void setCustomCollider(btVector3 origin, btScalar mass, btScalar friction);
	void setCustomTriggerCollider(btVector3 origin);
	void setBoxColliderFromMesh();

	bool isKinematic();
	void setVelocity(glm::vec3 velocity);
	glm::vec3 getVelocity();
	void addVelocity(glm::vec3 velocity);
	
	void setUserIndex(int index);
	int  getUserIndex();
	void updatePhysicsTransformFromGraphics();
	void updateGraphicsTransformFromPhysics();
	void addColliderToDynamicsWorld(btDynamicsWorld* dynamicsWorld);

	btCollisionObject* getCollisionBody();

	void draw(const Camera& camera) const;
	void normalizeSize();
	//glm::mat4 makeTransform(const Transform& transform, glm::mat4 model = glm::mat4(1.0));
	//void setSprite(Sprite sprite);

};


