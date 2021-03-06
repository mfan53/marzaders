#include "Enemy.h"
#include "Globals.h"
#include <iostream>
#include "OgreParticleSystem.h"

using namespace Arsenal;
using namespace std;

int Enemy::shootProbability = 50;
Ogre::ParticleSystem* smoke;

Enemy::Enemy(Ogre::SceneManager* scene, btDiscreteDynamicsWorld* dynamics,MoveBehaviour* behaviour,
		float xPos, float yPos, float zPos, unsigned int hp, unsigned int atk)
		: Entity(scene, dynamics, btVector3(15.3,15.3,15.3), COL_ENEMY,
			COL_BOX | COL_SHIP | COL_PLASMA, hp, atk, 10, xPos, yPos, zPos),
			mBehaviour(behaviour) {
	// Convert the ID into a string
	std::string idString = "ENEMY-"+getIDStr();

	mRender = scene->createEntity(idString, Ogre::SceneManager::PT_CUBE);
	mRender->setCastShadows(true);
	mNode = scene->getRootSceneNode()->createChildSceneNode();
	mNode->attachObject(mRender);
	mNode->setScale(0.3f,0.3f,0.3f);
	mStartX = xPos;
	// color = (rand() % (maxColors - minColors + 1)) + minColors;
	setColor();
	enemy = true;

	//std::string smokeID = "Smoke-" + getIDStr();
	//smoke = scene->createParticleSystem(smokeID,"Examples/Smoke");
	//mNode->attachObject(smoke);
	//smoke->setVisible(false);
}

Enemy::~Enemy() {
	if(mBehaviour) delete mBehaviour;
}

void Enemy::update(float delta) {
	mBehaviour->update(delta, this);
	Entity::update(delta);
	if(getZ() > Globals::WORLD_END) {
		damage(mHP);
	}
}

float Enemy::getWidth() {
	return mRender->getBoundingBox().getMaximum().x * 0.3 * 2;
}

void explode() {
	smoke->setVisible(true);
}

void Enemy::setColor() {
	switch (mBehaviour->type) {
		case SHIFTER :
			mRender->setMaterialName("Enemy/Water");
		break;

		case FORWARD :
			mRender->setMaterialName("Enemy/RustySteel");
		break;
	}
}

string intToString(int x) {
	stringstream ss;
	ss << x;
	return ss.str();
}

void Enemy::shoot(std::list<Arsenal::Entity*>* entities, float planeX, float planeY, float planeZ) {
	float z = planeZ - getZ();
	// Do not shoot if the plane is behind the enemy
	if(z < 0) { return; }
	float x = planeX - getX();
	float y = planeY - getY();

	float mag = sqrt(x*x + y*y + z*z);
	x = x/mag * 400.0f;
	y = y/mag * 400.0f;
	z = z/mag * 400.0f;

	if ((rand() % (shootProbability) + 1) == 2) {
		Arsenal::Plasma* p = new Arsenal::Plasma(mScene, mDynamics,
					Arsenal::coord3f(getX(),getY(),getZ()+getWidth() + 5.0f),
					Arsenal::coord3f(x, y, z), true);
		entities->push_back(p);
	}
}

void Enemy::increaseShootRate() {
	if (shootProbability > 4) {
		shootProbability -= 2;
	} else {
		shootProbability = 4;
	}
}
