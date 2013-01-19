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

		Ogre::String		pathCfg;

		//Earth tracked node
		Ogre::SceneNode*	mEarthTrackNode;

		//Earth base node
		Ogre::SceneNode*	mEarthBaseNode;

		//Earth node and entity
		Ogre::Entity*		mEarthEntity;             
		Ogre::SceneNode*	mEarthNode;               

		//First Cube node and entity
		Ogre::Entity*		mCubeEntity;                 
		Ogre::SceneNode*	mCubeNode;

		//Pongtab mesh and entity
		Ogre::SceneNode		*mPongTabNode;
		Ogre::Entity		*mPongTabEntity;

		Ogre::Real			mEarthRotationSpeed;
		Ogre::Real			mCubeRotationSpeed;
	
		Ogre::Real			mBaseRotationSpeed;

		//Second cube tracked node
		Ogre::SceneNode*	mCube2TrackNode;


};

#endif
