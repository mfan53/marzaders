#ifndef INC_AIRTRAFFIC_h
#define INC_AIRTRAFFIC_h

#include <vector>
#include <iostream>
#include <OgreManualObject.h>
#include <stdlib.h> 
#include <string.h>
#include "BaseApplication.h"
#include "btBulletDynamicsCommon.h"
#include "Entity.h"
#include "sound/SoundManager.h"
#include "ScoreManager.h"
#include "event/Event.h"
#include "event/EventManager.h"
#include "net/Endpoint.h"
#include "net/Host.h"
#include "net/Socket.h"
#include "Wall.h"
#include "Plane.h"
#include "Plasma.h"
#include "Box.h"
#include "Enemy.h"
#include "behaviour/ForwardMoveBehaviour.h"
#include <math.h>
#include "MainGUI.h"
#include "InGUI.h"

#define WORLD_END -2000

class AirTraffic : public BaseApplication
{
public:
   	AirTraffic(void);
   	virtual ~AirTraffic(void);

	bool keyPressed(const OIS::KeyEvent &arg);
   	bool keyReleased(const OIS::KeyEvent &arg);
	void quitGame();
	void soundToggle();
	void hideIngame();
	bool mouseMoved( const OIS::MouseEvent &arg);
	bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

protected:
   	virtual void createScene(void);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	void removeOutOfBoundsBullets();

	std::list<Arsenal::Entity*> entities;
	std::list<Arsenal::Box*> boxes;
	Arsenal::Plane *mPlane;
	int bulletNumber;
	
	btDefaultCollisionConfiguration* mCollisionConfig;
	btCollisionDispatcher* mDispatcher;
	btBroadphaseInterface* mOverlappingPairCache;
	btSequentialImpulseConstraintSolver* mSolver;
	btDiscreteDynamicsWorld* mWorld;
	EventManager::EventQueue * mEventQueue;

	bool soundOn;
	Arsenal::InGUI* ingui;
};

#endif // #ifndef _AIRTRAFFIC_h
