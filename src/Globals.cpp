#include "Globals.h"

using namespace Arsenal;
using namespace EventManager;

EventQueue * Globals::gEventQueue = EventQueue::getEventQueue();
AirTraffic* Globals::airInst;
std::string Globals::mHostIP;
//SoundManager * Globals::gSoundManager = SoundManager::getSoundManager(10);
