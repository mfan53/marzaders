#include "Box.h"

using namespace Arsenal;

Box::Box(Ogre::SceneManager* scene, btDiscreteDynamicsWorld* dynamics,
			float xPos, float yPos)
		: Entity(scene, dynamics, btVector3(SIZE/2,SIZE/2,SIZE/4), COL_BOX,
			COL_BULLET | COL_ENEMY, HP, ATK, 10, xPos, yPos, Z_POS) {
	// Modify bullet behaviour
	mBody->setActivationState(DISABLE_DEACTIVATION);
	mBody->setLinearFactor(btVector3(0,0,0)); // Allow no movement
	mBody->setAngularFactor(btVector3(0,0,0)); // Allow no rotations

	// Convert the ID into a string
	std::string idString = "BOX-"+getIDStr();

	mRender = scene->createEntity(idString,Ogre::SceneManager::PT_CUBE);
	mNode = scene->getRootSceneNode()->createChildSceneNode();
	mNode->attachObject(mRender);
	mNode->setScale(SIZE/100.0f,SIZE/100.0f,0.1f);
	mRender->setCastShadows(true);
	mRender->setMaterialName("TransWhite");
}

Box::~Box() {

}

void Box::damage(unsigned int damage) {
	Entity::damage(damage);
	float xPos = getX();
	float yPos = getY();
	float ratio = (((float)mHP - (float)mDamage) / (float)mHP);

	destroyScene();
	std::string idString = "BOX-"+getIDStr();
	mRender = mScene->createEntity(idString,Ogre::SceneManager::PT_CUBE);
	mNode = mScene->getRootSceneNode()->createChildSceneNode();
	mNode->attachObject(mRender);
	mNode->setScale(SIZE*ratio/100.0f,SIZE*ratio/100.0f,0.1f);
	mRender->setCastShadows(true);
	mRender->setMaterialName("TransWhite");
	
	destroyPhysics();
	initPhysics(mDynamics, btVector3(SIZE*ratio/2,SIZE*ratio/2,SIZE*ratio/4),
			COL_BOX, COL_BULLET | COL_ENEMY, 10, xPos, yPos, Z_POS);
}
