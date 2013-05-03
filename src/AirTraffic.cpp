#include "AirTraffic.h"

//#include "stdlib.h"

bool insideGUI; //toggle gui menu

// Forward declarations
static void physicsTickCallback(btDynamicsWorld *world, btScalar timeStep);

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
	mWorld->setInternalTickCallback(&physicsTickCallback);

	//mWorld->setGravity(btVector3(0,-9.8,0));
	mWorld->setGravity(btVector3(0,0,0));
	mEventQueue = EventManager::EventQueue::getEventQueue();

	soundOn = true;
	insideGUI = false;
	bulletNumber = 1;
	gamePaused = true;
	insideIPMenu = false;
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

void AirTraffic::removeOutOfBoundsBullets() {
	list<Arsenal::Entity*>::iterator it = entities.begin();
	while (it != entities.end()) {
		if (((Arsenal::Plasma *)*it)->getZ() <= WORLD_END) {
			delete *it;
			entities.erase(it++);
		}
		++it;
	}
}

//-------------------------------------------------------------------------------------
void AirTraffic::createScene(void)
{
	mSceneMgr->setSkyBox(true,"Examples/EveningSkyBox");
	mSceneMgr->showBoundingBoxes(true);

	//plane entity
	mPlane = new Arsenal::Plane(mSceneMgr,mWorld,"plane",mCamera);
	entities.push_back(mPlane);

	//space sky plane
	entities.push_back(new Arsenal::Wall(mSceneMgr,mWorld,0,-100,0,"Examples/Ground","back wall"));

	// Spawn Boxes
	spawnBoxes();

	// Spawn Enemies
	Arsenal::Enemy* enemy = new Arsenal::Enemy(mSceneMgr, mWorld, new Arsenal::ForwardMoveBehaviour(300));
	entities.push_back(enemy);

	//load cegui stuff
	mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
	CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

	//main menu gui
	maingui = new Arsenal::MainGUI();
	maingui->launch();

	//create the in game gui
	ingui = new Arsenal::InGUI();
	ingui->create();
}

bool AirTraffic::frameRenderingQueued(const Ogre::FrameEvent& evt) {
	bool b = BaseApplication::frameRenderingQueued(evt);
	if (!b) {
		return false;
	}

	int before = entities.size();
	removeOutOfBoundsBullets();
	if (entities.size() < before) {
		cout << "reduced size" << endl;
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

static void physicsTickCallback(btDynamicsWorld *world, btScalar timeStep) {
    int numManifolds = world->getDispatcher()->getNumManifolds();
	for (int i=0;i<numManifolds;i++)
	{
		btPersistentManifold* contactManifold =  world->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA = static_cast<btCollisionObject*>(contactManifold->getBody0());
		btCollisionObject* obB = static_cast<btCollisionObject*>(contactManifold->getBody1());
	
		int numContacts = contactManifold->getNumContacts();
		for (int j=0;j<numContacts;j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			if (pt.getDistance()<0.f)
			{
				const btVector3& ptA = pt.getPositionWorldOnA();
				const btVector3& ptB = pt.getPositionWorldOnB();
				const btVector3& normalOnB = pt.m_normalWorldOnB;
			}
		}
	}
}

bool AirTraffic::keyPressed(const OIS::KeyEvent &arg) {
	BaseApplication::keyPressed(arg);

	//CEGUI input handling
	CEGUI::System &sys = CEGUI::System::getSingleton();
	sys.injectKeyDown(arg.key);
	sys.injectChar(arg.text);

	if (gamePaused && arg.key != OIS::KC_P)
		return true;
	if (insideIPMenu && arg.key != OIS::KC_RETURN)
		return true;
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
	else if (arg.key == OIS::KC_1) {
		mPlane->setShot(Arsenal::SINGLE);
	}
	else if (arg.key == OIS::KC_2) {
		mPlane->setShot(Arsenal::SPRAY3);
	}
	else if (arg.key == OIS::KC_3) {
		mPlane->setShot(Arsenal::SPRAY5);
	}
	else if (arg.key == OIS::KC_SPACE) {
		mPlane->shoot(bulletNumber, &entities);
	}
	else if (arg.key == OIS::KC_P) {
		if (!insideGUI) {
			ingui->launch();
			insideGUI = true;
			pauseGame();
		}
		else {
			ingui->hide();
			insideGUI = false;
			unpauseGame();
		}
	}
	return true;
}

bool AirTraffic::keyReleased(const OIS::KeyEvent &arg) {
	BaseApplication::keyReleased(arg);

	//CEGUI input handling
	CEGUI::System::getSingleton().injectKeyUp(arg.key);	

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

void AirTraffic::spawnBoxes() {
	// Spawn Boxes
	for(float x = -100; x <= 100; x += 50) {
		for(float y = -100; y <= 100; y+= 50) {
			Arsenal::Box* mBox = new Arsenal::Box(mSceneMgr,mWorld,x,y);
			boxes.push_back(mBox);
			entities.push_back(mBox);
		}
	}
}

void AirTraffic::startGame() {
	mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0f,1.0f,1.0f));
	gamePaused = false;
}

void AirTraffic::quitGame() {
	mShutDown = true;
}

void AirTraffic::soundToggle() {
	if (soundOn) {
		//mSoundManager.pauseMusic();
		soundOn = false;
	}
	else {
		//mSoundManager.resumeMusic();
		soundOn = true;
	}
}

void AirTraffic::hideIngame() {
	mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0f,1.0f,1.0f));
	insideGUI = false;
	gamePaused = false;
}

void AirTraffic::reset() {
	//reset plane
	mPlane->reset();
	//clean up all plasmas
	deletePlasmas();
	//respawn boxes
	spawnBoxes();

	maingui->show();
	insideGUI = false;
	pauseGame();
}

void AirTraffic::deletePlasmas() {
	/*list<Arsenal::Entity*>::iterator it = entities.begin();
	while (it != entities.end()) {
		if (((Arsenal::Plasma *)*it)->getID().compare("plasma")) {
			delete *it;
			entities.erase(it++);
		}
		++it;
	}*/
	printf("implement deleting plasmas \n");
}

void AirTraffic::pauseGame() {
	gamePaused = true;
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5f,0.5f,0.5f));
	//pause plasma 
	
}

void AirTraffic::unpauseGame() {
	gamePaused = false;
	mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0f,1.0f,1.0f));
	//unpause plasma 
}

void AirTraffic::inIP() {
	insideIPMenu = true;
}

void AirTraffic::outIP() {
	insideIPMenu = false;
}

CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID) {
	switch(buttonID) {
		case OIS::MB_Left:
       			return CEGUI::LeftButton;
 		case OIS::MB_Right:
        		return CEGUI::RightButton;
 		case OIS::MB_Middle:
        		return CEGUI::MiddleButton;
 		default:
        		return CEGUI::LeftButton;
	}
}

bool AirTraffic::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	CEGUI::System::getSingleton().injectMouseButtonDown(convertButton(id));
	return true;
}

bool AirTraffic::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	CEGUI::System::getSingleton().injectMouseButtonUp(convertButton(id));
	return true;
}

bool AirTraffic::mouseMoved( const OIS::MouseEvent &arg )
{
	CEGUI::System &sys = CEGUI::System::getSingleton();
	sys.injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
	// Scroll wheel.
	if (arg.state.Z.rel)
    		sys.injectMouseWheelChange(arg.state.Z.rel / 120.0f);
	return true;
}
