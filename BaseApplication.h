
#ifndef _INGESUP_BASE_APPLICATION_H_
#define _INGESUP_BASE_APPLICATION_H_

struct TrackResult {
	int id;
	std::string name;
	double pattWidth;

	bool found;
	double			 score;
	Ogre::Vector3	 pos;
	Ogre::Quaternion quat;

	TrackResult() : id(-1), found(false), pattWidth(110),score(0) {

	}
};

class BaseApplication : 
	public Ogre::FrameListener, 
	public Ogre::WindowEventListener, 
	public OIS::KeyListener, 
	public OIS::MouseListener, 
	OgreBites::SdkTrayListener
{
	public:
		BaseApplication(void);
		virtual ~BaseApplication(void);

		virtual void go(void);

	protected:
		virtual bool setup();
		virtual bool configure(void);
		virtual void chooseSceneManager(void);
		virtual void createCamera(void);
		virtual void createFrameListener(void);
		virtual void createScene(void) = 0; // Override me!
		virtual void destroyScene(void);
		virtual void createViewports(void);
		virtual void setupResources(void);
		virtual void createResourceListener(void);
		virtual void loadResources(void);

		//Initialize video capture
		void initVideoCap(int device , Ogre::Real width , Ogre::Real height , Ogre::Real fx , Ogre::Real fy , 
										bool verticalFlip =false  , bool horizontalFlip = false);
		//Initialize tracking
		void initArtoolkit();

		//Track patterns in current image
		void track();
		void updateTrackedNode(const TrackResult& trackRes , Ogre::SceneNode* node );

		void convertArtoolkitTransMat(double patt_trans[3][4] , Ogre::Vector3& pos , Ogre::Quaternion& quat);
		void normalizeRotationVector(const Ogre::Vector3& rotVect , double& rotNorm , Ogre::Vector3& normalizedVectRot);

		// Ogre::FrameListener
		virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

		// OIS::KeyListener
		virtual bool keyPressed( const OIS::KeyEvent &arg );
		virtual bool keyReleased( const OIS::KeyEvent &arg );
		// OIS::MouseListener
		virtual bool mouseMoved( const OIS::MouseEvent &arg );
		virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
		virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

		bool initBackgroundTexture();
		Ogre::PixelFormat openCVToOgrePixFormat(const cv::Mat& img);

		void treadCam();

		// Ogre::WindowEventListener
		//Adjust mouse clipping area
		virtual void windowResized(Ogre::RenderWindow* rw);
		//Unattach OIS before window shutdown (very important under Linux)
		virtual void windowClosed(Ogre::RenderWindow* rw);

		Ogre::Root*			mRoot;
		Ogre::Camera*		mCamera;
		Ogre::SceneManager* mSceneMgr;
		Ogre::RenderWindow* mWindow;
		Ogre::String		mResourcesCfg;
		Ogre::String		mPluginsCfg;
		Ogre::String		mCfg;
		Ogre::String		logCfg;
		Ogre::String		pathCfg;
		char*				PathHiro;
		char*				PathKanji;

		// OgreBites
		OgreBites::SdkTrayManager*	mTrayMgr;
		OgreBites::SdkCameraMan*	mCameraMan;       // basic camera controller
		OgreBites::ParamsPanel*		mDetailsPanel;     // sample details panel
    
		bool mCursorWasVisible;                    // was cursor visible before dialog appeared
		bool mShutDown;

		//OIS Input devices
		OIS::InputManager*	mInputManager;
		OIS::Mouse*			mMouse;
		OIS::Keyboard*		mKeyboard;

		//Video Camera
		Ogre::Real		mVideoCamWidth;		//camera width
		Ogre::Real		mVideoCamHeight;	//camera height
		Ogre::Real		mVideoCamFx;		//camera focal on x axis
		Ogre::Real		mVideoCamFy;		//camera focal on y axis
		bool			mVerticalFlip;
		bool			mHorizontalFlip;

		//Video capture
		cv::VideoCapture mVideoCap;
		cv::Mat 		 mCapImg;
		cv::Mat 		 mRGBAImg;

		//Background texture
		Ogre::TexturePtr	mBackgroundTexture;
	

		bool			mNewImage;
		boost::mutex	mMutex;
		boost::thread	mThread;
		bool			mRunThread;

		//Tracking results
		std::vector<TrackResult> trackResults;
};

#endif 
