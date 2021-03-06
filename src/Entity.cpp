#include "Entity.h"
#include <iostream>

using namespace std;
using namespace Arsenal;

Entity::Entity() {
	initID();
	mPhysics = false;
	mHP = 0;
	mDamage = 0;
	mAttack = 0;
	enemy = false;
}

Entity::Entity(Ogre::SceneManager* scene, unsigned int hp, unsigned int attack) {
	initID();
	mPhysics = false;
	mScene = scene;
	mHP = hp;
	mDamage = 0;
	mAttack = attack;
	enemy = false;
}

Entity::Entity(Ogre::SceneManager* scene, btDiscreteDynamicsWorld* dynamics,
		btVector3 hitbox, int collide, int collidesWith, unsigned int hp,
		unsigned int attack, btScalar mass, float xPos, float yPos, float zPos) {
	initID();
	mPhysics = false;
	mScene = scene;
	mHP = hp;
	mDamage = 0;
	mAttack = attack;
	enemy = false;
	initPhysics(dynamics, hitbox, collide, collidesWith, mass, xPos, yPos, zPos);
}

Entity::~Entity() {
	destroyScene();
	destroyPhysics();
}

int Entity::getCurrentHP() {
	return mHP - mDamage;
}


void Entity::destroyScene() {
	if(mScene) {
		if(mNode) {
			mNode->detachObject(mRender);
			mScene->destroyEntity(mRender);
			mScene->destroySceneNode(mNode);
		}
	}
}

void Entity::destroyPhysics() {
	if(mPhysics) {
		if(mBody!=NULL) mDynamics->removeCollisionObject(mBody);
		if(mMotion!=NULL) delete mMotion;
		if(mShape!=NULL) delete mShape;
		if(mBody!=NULL) delete mBody;
		mPhysics = false;
	}
}

void Entity::update(float delta) {
	// Get the bullet transform
	btTransform trans;
	mBody->getMotionState()->getWorldTransform(trans);

	// Update the Ogre scene node position
	if(mNode) {
		mNode->setPosition(
			trans.getOrigin().getX(),
			trans.getOrigin().getY(),
			trans.getOrigin().getZ());
	}
}

void Entity::initPhysics(btDiscreteDynamicsWorld* dynamics, btVector3 hitbox,
		int collide, int collidesWith, btScalar mass, float xPos, float yPos,
		float zPos) {
	if(!mPhysics) {
		mPhysics = true;

		btVector3 localInertia = btVector3(btScalar(0),btScalar(0),btScalar(0));
		btTransform transform;
		transform.setIdentity();
		transform.setOrigin(btVector3(xPos,yPos,zPos));
		mMotion = new btDefaultMotionState(transform);
		mShape = new btBoxShape(hitbox);
		btRigidBody::btRigidBodyConstructionInfo rbInfo
			= btRigidBody::btRigidBodyConstructionInfo
				(mass,mMotion,mShape,localInertia);
		mBody = new btRigidBody(rbInfo);
		mBody->setUserPointer(this);

		mDynamics = dynamics;
		mDynamics->addRigidBody(mBody,collide,collidesWith);
	} else {
		printf("Warning: Attempted to re-initialize physics for entity.\n");
	}
}

void Entity::initID() {
	// Set the ID of the entity
	static int next_id = 0;
	mID = next_id;
	next_id++;
}

std::string Entity::getIDStr() const {
	// Convert the ID into a string
	std::ostringstream oss;
	oss << mID;
	return oss.str();
}

float Entity::getX() const {
	btTransform trans;
	mBody->getMotionState()->getWorldTransform(trans);
	return trans.getOrigin().getX();
}

float Entity::getY() const {
	btTransform trans;
	mBody->getMotionState()->getWorldTransform(trans);
	return trans.getOrigin().getY();
}

float Entity::getZ() const {
	btTransform trans;
	mBody->getMotionState()->getWorldTransform(trans);
	return trans.getOrigin().getZ();
}

float Entity::getXV() const {
	return mBody->getLinearVelocity().getX();
}

float Entity::getYV() const {
	return mBody->getLinearVelocity().getY();
}

float Entity::getZV() const {
	return mBody->getLinearVelocity().getZ();
}

void Entity::setPos(float x, float y, float z) {
	// Update the bullet position
	btTransform trans = mBody->getCenterOfMassTransform();
	trans.setOrigin(btVector3(x,y,z));
	mBody->setCenterOfMassTransform(trans);

	// Update the Ogre scene node position
	mNode->setPosition(
		trans.getOrigin().getX(),
		trans.getOrigin().getY(),
		trans.getOrigin().getZ());
}

void Entity::setVel(float xv, float yv, float zv) {
	mBody->setLinearVelocity(btVector3(xv,yv,zv));
}


