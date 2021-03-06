#ifndef INC_ARSENAL_ENTITY_H
#define INC_ARSENAL_ENTITY_H

#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <OgreMesh.h>
#include <OgreSceneNode.h>
#include "btBulletDynamicsCommon.h"
#include "sound/Sound.h"

namespace Arsenal {
	struct coord3f {
		float x;
		float y;
		float z;
		coord3f() {};
		coord3f(float xpos, float ypos, float zpos) : x(xpos), y(ypos), z(zpos) {}
	};

	class Entity {
	public:
		Entity();
		Entity(Ogre::SceneManager* scene, unsigned int hp, unsigned int attack);
		Entity(Ogre::SceneManager* scene, btDiscreteDynamicsWorld* dynamics,
				btVector3 hitbox, int collide, int collidesWith, unsigned int hp,
				unsigned int attack, btScalar mass=10, float xPos=0,
				float yPos=0, float zPos=0);
		virtual ~Entity();

		void destroyScene();
		void destroyPhysics();

		virtual void update(float delta);

		virtual void damage(unsigned int damage) { mDamage += damage; };

		int getId() const { return mID; }
		std::string getIDStr() const;
		float getX() const;
		float getY() const;
		float getZ() const;
		float getXV() const;
		float getYV() const;
		float getZV() const;
		float getBoundingRadius() const { return mBoundingRadius; }
		Ogre::SceneNode* getNode() const { return mNode; };
		Ogre::Entity* getRender() const { return mRender; };
		unsigned int getHP() const { return mHP; };
		unsigned int getDamage() const { return mDamage; };
		unsigned int getAttack() const { return mAttack; };
		float getStartX() {return mStartX;}
		bool isDead() const { return mHP == 0 ? false : mDamage >= mHP; };
		int getCurrentHP();
		bool isEnemy() {return enemy;}

		void setPos(float x, float y, float z);
		void setVel(float vx, float vy, float vz);

		enum collisiontypes {
			COL_NOTHING = 0,   // Collide with nothing
			COL_BOX = 1,       // Collide with ship
			COL_SHIP = 1<<1,   // Collide with boxes
			COL_ENEMY = 1<<2,  // Collide with enemies
			COL_PLASMA = 1<<3, // Collide with player bullets
			COL_BULLET = 1<<4, // Collide with enemy bullets
		};

	protected:
		bool enemy;

		// The starting position of the Entity
		float mStartX;

		// The id of the Entity
		int mID;
		// The bounding radius of the Entity
		float mBoundingRadius;

		// The maximum hit points of the entity or 0 if invulnerable
		unsigned int mHP;
		// The amount of damage this entity has taken
		unsigned int mDamage;
		// The amount of damage this entity can deal
		unsigned int mAttack;

		// A reference to the OGRE scene that this Entity belongs to
		Ogre::SceneManager* mScene;
		// A reference to the dynamics world that this Entity belongs to
		btDiscreteDynamicsWorld* mDynamics;

		// A reference to the scene node that this Entity belongs to
		Ogre::SceneNode* mNode;

		// The Ogre render component
		Ogre::Entity* mRender;
		// The bullet physics components
		btRigidBody* mBody;
		btCollisionShape* mShape;
		btDefaultMotionState* mMotion;

		void initPhysics(btDiscreteDynamicsWorld* world, btVector3 hitbox,
				int collide, int collidesWith, btScalar mass=10, float xPos=0,
				float yPos=0, float zPos=0);

	private:
		// True if physics has been initialized
		bool mPhysics;

		void initID();
	};
}

#endif // INC_ARSENAL_ENTITY_H
