noinst_HEADERS = ../src/Arsenal.h ../src/BaseApplication.h ../src/AirTraffic.h ../src/Entity.h ../src/sound/SoundManager.h ../src/ScoreManager.h ../src/sound/Sound.h ../src/net/Endpoint.h ../src/net/Host.h ../src/net/Socket.h ../src/event/EventManager.h ../src/event/Event.h ../src/event/EventQueue.h ../src/Globals.h ../src/Plane.h ../src/Plasma.h ../src/Wall.h ../src/Box.h ../src/behaviour/MoveBehaviour.h ../src/behaviour/ForwardMoveBehaviour.h ../src/behaviour/SideToSideMoveBehaviour.h ../src/Enemy.h ../src/MainGUI.h ../src/InGUI.h ../src/Spawner.h ../src/Timer.h

bin_PROGRAMS = AirTraffic
AirTraffic_CPPFLAGS = -I$(top_srcdir)
AirTraffic_SOURCES = ../src/Main.cpp ../src/BaseApplication.cpp ../src/AirTraffic.cpp ../src/Entity.cpp ../src/sound/SoundManager.cpp ../src/ScoreManager.cpp ../src/sound/Sound.cpp ../src/net/Endpoint.cpp ../src/net/Host.cpp ../src/net/Socket.cpp ../src/event/EventManager.cpp ../src/event/EventQueue.cpp ../src/Globals.cpp ../src/Plane.cpp ../src/Plasma.cpp ../src/Wall.cpp ../src/Box.cpp ../src/behaviour/ForwardMoveBehaviour.cpp ../src/behaviour/SideToSideMoveBehaviour.cpp ../src/Enemy.cpp ../src/MainGUI.cpp ../src/InGUI.cpp ../src/Spawner.cpp ../src/Timer.cpp

boost_CFLAGS = -I/usr/include/boost
boost_LIBS = -lboost_system

AirTraffic_CXXFLAGS = $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS) $(SDL_CFLAGS) $(SDL_MIXER_CFLAGS) $(boost_CFLAGS)
AirTraffic_LDADD = $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS) $(SDL_LIBS) $(SDL_MIXER_LIBS) $(boost_LIBS) $(CEGUI_LIBS) $(CEGUI_OGRE_LIBS)

EXTRA_DIST = buildit makeit bootstrap
AUTOMAKE_OPTIONS = foreign
