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

		bool collisionOtherPad(Ogre::SceneNode *node, const int idCurBasNode);
		void switchBaseNodeParent(const int idCurBaseNode);
		Ogre::Vector3 calculTrajectoire(void);

		//Ogre::String		pathCfg;
		bool				isBallVisible;

		//Palet tracked node
		Ogre::SceneNode*	mPongPaletTrackNode1;
		Ogre::SceneNode*	mPongPaletTrackNode2;

		//Pong palet base node
		Ogre::SceneNode*	mPongPaletBaseNode1;
		Ogre::SceneNode*	mPongPaletBaseNode2;

		Ogre::SceneNode		*mBallBaseNode;

		//Pongtab mesh and entity
		Ogre::SceneNode		*mBallNode;
		Ogre::Entity		*mBallEntity;
		Ogre::SceneNode		*mPaletNode1;
		Ogre::Entity		*mPaletEntity1;
		Ogre::SceneNode		*mPaletNode2;
		Ogre::Entity		*mPaletEntity2;

		Ogre::Real			mPalet1RotationSpeed;
		Ogre::Real			mBallRotationSpeed;
		Ogre::Real			mBaseRotationSpeed;

		Ogre::Real			ballSpeed;

		Ogre::Vector3		positionBall;

		int i;
		Ogre::Real			deltaX;

		bool*				nodeFound;
		int					idCurrentPad;
};

#endif
