#include "AirTraffic.h"


//-------------------------------------------------------------------------------------
AirTraffic::AirTraffic(void)
{
	// Initialize bullet
	mCollisionConfig = new btDefaultCollisionConfiguration();
	mDispatcher = new btCollisionDispatcher(mCollisionConfig);
	mOverlappingPairCache = new btDbvtBroadphase();
	mSolver = new btSequentialImpulseConstraintSolver;
	mWorld = new btDiscreteDynamicsWorld
		(mDispatcher,mOverlappingPairCache,mSolver,mCollisionConfig);

	//mWorld->setGravity(btVector3(0,-9.8,0));
	mWorld->setGravity(btVector3(0,0,0));
	mEventQueue = EventManager::EventQueue::getEventQueue();
}
//-------------------------------------------------------------------------------------
AirTraffic::~AirTraffic(void)
{
	for (std::list<Arsenal::Entity*>::iterator iter = entities.begin();
			iter != entities.end(); iter++)
	{
		delete *iter;
	}
	entities.clear();
	delete mWorld;
	delete mSolver;
	delete mOverlappingPairCache;
	delete mDispatcher;
	delete mCollisionConfig;
	//delete &mSoundManager;
}

//-------------------------------------------------------------------------------------
void AirTraffic::createScene(void)
{
	mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0f,1.0f,1.0f));

	//plane entity
	mPlane = new Arsenal::Plane(mSceneMgr,mWorld,"plane",mCamera);
	entities.push_back(mPlane);
	for(float x = -100; x <= 100; x += 25) {
		for(float y = -100; y <= 100; y+= 25) {
			Arsenal::Box* mBox = new Arsenal::Box(mSceneMgr,mWorld,x,y);
			boxes.push_back(mBox);
			entities.push_back(mBox);
		}
	}
	
	mSceneMgr->setSkyBox(true,"Examples/EveningSkyBox");
}

bool AirTraffic::frameRenderingQueued(const Ogre::FrameEvent& evt) {
	bool b = BaseApplication::frameRenderingQueued(evt);
	if (!b) {
		return false;
	}

	float delta = evt.timeSinceLastFrame;
	
	mWorld->stepSimulation(delta,10);
	
	for(std::list<Arsenal::Entity*>::iterator iter = entities.begin();
		iter != entities.end(); ++iter) {
   		(*iter)->update(delta);
	}

	// Update camera position
	mCamera->setPosition(Ogre::Vector3(mPlane->getX(), mPlane->getY()+20, 95));
	mCamera->lookAt(Ogre::Vector3(mPlane->getX(), mPlane->getY(),-500));

	return true;
}

bool AirTraffic::keyPressed(const OIS::KeyEvent &arg) {
	BaseApplication::keyPressed(arg);
	if (arg.key == OIS::KC_W) {
		mPlane->move(Arsenal::UP);
	} 
	else if (arg.key == OIS::KC_S) {
		mPlane->move(Arsenal::DOWN);
	}
	else if (arg.key == OIS::KC_D) {
		mPlane->move(Arsenal::RIGHT);
	}
	else if (arg.key == OIS::KC_A) {
		mPlane->move(Arsenal::LEFT);
	}
	return true;
}

bool AirTraffic::keyReleased(const OIS::KeyEvent &arg) {
	BaseApplication::keyReleased(arg);
	switch (arg.key) {
		case OIS::KC_W :
			mPlane->stop(Arsenal::UP);
			break; 
		case OIS::KC_S :
			mPlane->stop(Arsenal::DOWN);
			break; 
		case OIS::KC_A :
			mPlane->stop(Arsenal::LEFT);
			break; 
		case OIS::KC_D :
			mPlane->stop(Arsenal::RIGHT);
			break;
	} 
	return true;
}
