#ifndef _RAHPONG_H_
#define _RAHPONG_H_

class Rahpong {

	public:
		Rahpong(void);
		void go(void);

	protected:
		void destroyScene(void);
		bool setup(void);
		bool configure(void);
		void createScene(void);
		void createCamera(void);
		void createViewports(void);
		void setupResources(void);
		void loadResources(void);


		// Ogre::WindowEventListener
		//Adjust mouse clipping area
		void windowResized(Ogre::RenderWindow* rw);
		//Unattach OIS before window shutdown (very important under Linux)
		void windowClosed(Ogre::RenderWindow* rw);


		Ogre::Root		   *mRoot;
		Ogre::Camera	   *mCamera;
		Ogre::SceneManager *mSceneMgr;
		Ogre::RenderWindow *mWindow;

		// OgreBites
		OgreBites::SdkTrayManager*	mTrayMgr;
		OgreBites::SdkCameraMan*	mCameraMan;       // basic camera controller
		OgreBites::ParamsPanel*		mDetailsPanel;     // sample details panel

		// Config & ressources
		Ogre::String		pathCfg;
		Ogre::String		resourcesCfg;
		Ogre::String		pluginsCfg;
		Ogre::String		ogreCfg;
		Ogre::String		logCfg;

		//Pongtab mesh and entity
		Ogre::SceneNode		*mPongTabNode;
		Ogre::Entity		*mPongTabEntity;

		//OIS Input devices
		OIS::InputManager*	mInputManager;
		OIS::Mouse*			mMouse;
		OIS::Keyboard*		mKeyboard;

};

#endif
