#ifndef INC_ARSENAL_PLASMA_H
#define INC_ARSENAL_PLASMA_H

#include "Entity.h"

namespace Arsenal {
	class Plasma : public Entity {
		public:
			Plasma() {}
			Plasma(Ogre::SceneManager* scene, btDiscreteDynamicsWorld* dynamics,
					const coord3f pos, const coord3f vel, bool isEnemyShot = false);
			virtual ~Plasma();

			void update(float delta);

			static void toggleFire() {fireOn = !fireOn;}

			static const unsigned int HP = 1;
			static const unsigned int ATK = 1;

		private:
			bool m_isEnemyShot;
			static bool fireOn;
	};
}

#endif // INC_ARSENAL_PLASMA_H
