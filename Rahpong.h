#ifndef _RAHPONG_H_
#define _RAHPONG_H_

#include "BaseApplication.h"

class Rahpong : public BaseApplication {

	public:
		Rahpong(void);

	protected:
		virtual void createScene(void);
		virtual void createFrameListener(void);
		virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);
		virtual bool keyReleased( const OIS::KeyEvent& evt );
		virtual bool keyPressed( const OIS::KeyEvent &arg );
		void updateTrackedNode(const TrackResult& trackRes, Ogre::SceneNode* node, int id );

		bool checkDirection(void);
		bool collisionWithPad(Ogre::SceneNode *node);
		Ogre::Vector3 nextLocation(Ogre::SceneNode *node);


		//Palet tracked node
		Ogre::SceneNode*	mPongPaletTrackNode1;
		Ogre::SceneNode*	mPongPaletTrackNode2;

		//Pong palet base node
		Ogre::SceneNode		*mPongBaseNode;

		Ogre::SceneNode		*mBallBaseNode;

		//Pongtab mesh and entity
		Ogre::SceneNode		*mBallNode;
		Ogre::Entity		*mBallEntity;
		Ogre::SceneNode		*mPaletNode1;
		Ogre::Entity		*mPaletEntity1;
		Ogre::SceneNode		*mPaletNode2;
		Ogre::Entity		*mPaletEntity2;

		Ogre::SceneNode		*padToColide;

		Ogre::Real			ballSpeedX;
		Ogre::Real			ballSpeedZ;

		Ogre::Vector3		positionBall;

		bool*				nodeFound;
		int					upToDown; //0: horizontale, 1: downToUp, -1: upToDown
		bool				direction; // true: left to right
		bool				firstAllFound;

		float				heightPad;
};

#endif
