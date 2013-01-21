#include "header.h"
#include "BaseApplication.h"

//-------------------------------------------------------------------------------------
BaseApplication::BaseApplication(void)
    : mRoot(0),
    mCamera(0),
    mSceneMgr(0),
    mWindow(0),
    mResourcesCfg(Ogre::StringUtil::BLANK),
    mPluginsCfg(Ogre::StringUtil::BLANK),
    mTrayMgr(0),
    mCameraMan(0),
    mDetailsPanel(0),
    mCursorWasVisible(false),
    mShutDown(false),
    mInputManager(0),
    mMouse(0),
    mKeyboard(0),
	mVideoCamWidth(640),
	mVideoCamHeight(480),
	mVideoCamFx(800),
	mVideoCamFy(800),
	mVerticalFlip(false),
	mHorizontalFlip(false),
	mRunThread(true),
	mNewImage(false)
{
	pathCfg = "E:/Codage/Cours/Rahpong/data";
	PathHiro = (char *)"E:/Codage/Cours/Rahpong/data/patt.hiro";
	PathKanji = (char *)"E:/Codage/Cours/Rahpong/data/patt.kanji";
	mThread = boost::thread(&BaseApplication::treadCam, this);
	foundSomething = false;
}

//-------------------------------------------------------------------------------------
BaseApplication::~BaseApplication(void)
{
	mRunThread = false;
	mThread.join();

    if (mTrayMgr) delete mTrayMgr;
    if (mCameraMan) delete mCameraMan;

    //Remove ourself as a Window listener
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);
    delete mRoot;
}

//-------------------------------------------------------------------------------------
void BaseApplication::treadCam()
{
	cv::Mat tmp;
	
	while(mRunThread)
	{

		mMutex.lock();
		if(mVideoCap.isOpened())
		{
			mVideoCap.read(tmp);
			if(mVerticalFlip && mHorizontalFlip)
				cv::flip(tmp,tmp,-1);
			else if(mVerticalFlip && !mHorizontalFlip)
				cv::flip(tmp,tmp,0);
			else if(!mVerticalFlip && mHorizontalFlip)
				cv::flip(tmp,tmp,1);
			mNewImage = true;

			if(tmp.cols != this->mVideoCamWidth || tmp.rows != this->mVideoCamHeight )
			{
				cv::resize(tmp,mCapImg,cv::Size(mVideoCamWidth,mVideoCamHeight));
			}
			else
			{
				mCapImg =tmp.clone();
			}
		}
		mMutex.unlock();
		Sleep(20);	
	}
}




//-------------------------------------------------------------------------------------
bool BaseApplication::initBackgroundTexture()
{
	if(!mSceneMgr)
		return false;

	//clear
	if(!mBackgroundTexture.isNull())
	{
		//pSceneMgr_->getRootSceneNode()->detachObject("BackgroundVideoNode");
		mSceneMgr->destroySceneNode("BackgroundVideoNode");
		Ogre::TextureManager::getSingleton().remove("BackgroundVideoTexture");
		Ogre::MaterialManager::getSingleton().remove("BackgroundVideoMaterial");
		mBackgroundTexture.setNull();
	}

	//IplImage pImg = mCapImg;
	const size_t width      = mCapImg.rows*mCapImg.cols ? mCapImg.cols  : 32;
	const size_t height     = mCapImg.rows*mCapImg.cols ? mCapImg.rows  : 32;
	const Ogre::PixelFormat format = mCapImg.rows*mCapImg.cols ? openCVToOgrePixFormat(mCapImg) : Ogre::PF_A8R8G8B8;

	mBackgroundTexture = Ogre::TextureManager::getSingleton().createManual(
						"BackgroundVideoTexture",
						Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
						Ogre::TEX_TYPE_2D,
						width, height, 0, format/*PF_A8R8G8B8*//*Ogre::PF_BYTE_BGR*/,
						/*TU_DYNAMIC_WRITE_ONLY*/Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE );
	
	
	
	Ogre::MaterialPtr backgroundMaterial = Ogre::MaterialManager::getSingleton().create(
					"BackgroundVideoMaterial",
					Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	backgroundMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("BackgroundVideoTexture");
	backgroundMaterial->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
	backgroundMaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
	backgroundMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	backgroundMaterial->getTechnique(0)->getPass(0)->setCullingMode(Ogre::CULL_NONE);
	backgroundMaterial->getTechnique(0)->getPass(0)->setManualCullingMode(Ogre::MANUAL_CULL_NONE);
	//Image m_backgroundImage;
	//m_backgroundImage.load("img.jpg",ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	//backgroundTexture_->loadImage(m_backgroundImage);

	Ogre::Rectangle2D* rectangle = new Ogre::Rectangle2D(true);

	rectangle->setCorners(-1.0f , 1.0f , 1.0f , -1.0f);
	rectangle->setBoundingBox(Ogre::AxisAlignedBox( -100.0 * Ogre::Vector3::UNIT_SCALE, 100.0 * Ogre::Vector3::UNIT_SCALE));
	rectangle->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);
	rectangle->setMaterial("BackgroundVideoMaterial");

	// Attach background to the scene
	Ogre::SceneNode* headNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("BackgroundVideoNode");
	headNode->attachObject(rectangle);

	return true;
}

	
//-------------------------------------------------------------------------------------
bool BaseApplication::configure(void)
{
    // Show the configuration dialog and initialise the system
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg

	//if(mRoot->restoreConfig()) return true;
    if(mRoot->showConfigDialog())
    {
        // If returned true, user clicked OK so initialise
        // Here we choose to let the system create a default rendering window by passing 'true'
        mWindow = mRoot->initialise(true, "Application Render Window");

        return true;
    }
    else
    {
        return false;
    }
}
//-------------------------------------------------------------------------------------
void BaseApplication::chooseSceneManager(void)
{
    // Get the SceneManager, in this case a generic one
    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
}

//-------------------------------------------------------------------------------------
void BaseApplication::initVideoCap(int device , Ogre::Real width , Ogre::Real height , Ogre::Real fx , Ogre::Real fy 
										, bool verticalFlip , bool horizontalFlip)
{

	mVideoCamWidth = width;
	mVideoCamHeight = height;
	mVideoCamFx = fx;
	mVideoCamFy = fy;
	mVerticalFlip = verticalFlip;
	mHorizontalFlip = horizontalFlip;

	//Configure projection perspective to match video camera
	if(mVideoCamFy != 0)
		if(mCamera)
			mCamera->setFOVy(Ogre::Radian(2.0 * (  atan( 0.5*mVideoCamHeight/mVideoCamFy ) )));
	
	if(mVideoCamFx!=0.0 && mVideoCamHeight !=0 )
		if(mCamera)
			mCamera->setAspectRatio((mVideoCamFy*mVideoCamWidth)/(mVideoCamFx*mVideoCamHeight));
			
	mMutex.lock();
	mVideoCap.open(device);
	mVideoCap.set(CV_CAP_PROP_FRAME_WIDTH,mVideoCamWidth);
	mVideoCap.set(CV_CAP_PROP_FRAME_HEIGHT,mVideoCamHeight);
	mVideoCap.set(CV_CAP_PROP_FPS,25);
	mMutex.unlock();

	
}


//-------------------------------------------------------------------------------------
void BaseApplication::createCamera(void)
{
    // Create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");

    
    mCamera->setPosition(Ogre::Vector3(0,0,0));
    
    mCamera->lookAt(Ogre::Vector3(0,0,-300));
    mCamera->setNearClipDistance(0.1f);

    mCameraMan = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
}

//-------------------------------------------------------------------------------------
void BaseApplication::initArtoolkit()
{
	ARParam cparam;
	//arParamLoad("../../../data/camera_para.dat", 1, &cparam);
	cparam.xsize = mVideoCamWidth;
	cparam.ysize = mVideoCamHeight;
	cparam.mat[0][0] = mVideoCamFx;	cparam.mat[0][1] = 0;			cparam.mat[0][2] = mVideoCamWidth/2;	cparam.mat[0][3] = 0;
	cparam.mat[1][0] = 0;			cparam.mat[1][1] = mVideoCamFy;	cparam.mat[1][2] = mVideoCamHeight/2;	cparam.mat[1][3] = 0;
	cparam.mat[2][0] = 0;			cparam.mat[2][1] = 0;			cparam.mat[2][2] = 1;					cparam.mat[2][3] = 0;
	cparam.dist_factor[0] = mVideoCamWidth/2;
	cparam.dist_factor[1] = mVideoCamHeight/2;
	cparam.dist_factor[2] = 1;
	cparam.dist_factor[3] = 1;
	arInitCparam(&cparam);

	TrackResult res1;
	if( (res1.id=arLoadPatt(PathHiro)) >= 0)
	{
		res1.name = "patt.hiro";
		res1.pattWidth = 113;
		trackResults.push_back(res1);
	}

	TrackResult res2;
	if( (res2.id=arLoadPatt(PathKanji)) >= 0 )
	{
		res2.name = "patt.kanji";
		res2.pattWidth = 113;
		trackResults.push_back(res2);
	}
}


//-------------------------------------------------------------------------------------
void BaseApplication::createFrameListener(void)
{
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    mInputManager = OIS::InputManager::createInputSystem( pl );

    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);

    //Set initial mouse clipping size
    windowResized(mWindow);

    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

    mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, mMouse, this);
    mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    mTrayMgr->hideCursor();

    // create a params panel for displaying sample details
    Ogre::StringVector items;
    items.push_back("cam.pX");
    items.push_back("cam.pY");
    items.push_back("cam.pZ");
    items.push_back("");
    items.push_back("cam.oW");
    items.push_back("cam.oX");
    items.push_back("cam.oY");
    items.push_back("cam.oZ");
    items.push_back("");
    items.push_back("Filtering");
    items.push_back("Poly Mode");

    mDetailsPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 200, items);
    mDetailsPanel->setParamValue(9, "Bilinear");
    mDetailsPanel->setParamValue(10, "Solid");
    mDetailsPanel->hide();

    mRoot->addFrameListener(this);
}
//-------------------------------------------------------------------------------------
void BaseApplication::destroyScene(void)
{
}
//-------------------------------------------------------------------------------------
void BaseApplication::createViewports(void)
{
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(
        Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}
//-------------------------------------------------------------------------------------
void BaseApplication::setupResources(void)
{
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load(mResourcesCfg);

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }
}
//-------------------------------------------------------------------------------------
void BaseApplication::createResourceListener(void)
{

}
//-------------------------------------------------------------------------------------
void BaseApplication::loadResources(void)
{
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
//-------------------------------------------------------------------------------------
void BaseApplication::go(void)
{
#ifdef _DEBUG
    mPluginsCfg = pathCfg+"/plugins_d.cfg";
#else
    mPluginsCfg = pathCfg+"/plugins.cfg";
#endif
	mResourcesCfg = pathCfg+"/resources.cfg";
	mCfg = pathCfg+"/ogre.cfg";
	logCfg = pathCfg+"/RahpongLog.txt";

    if (!setup())
        return;

    mRoot->startRendering();

    // clean up
    destroyScene();
}
//-------------------------------------------------------------------------------------
bool BaseApplication::setup(void)
{
    mRoot = new Ogre::Root(mPluginsCfg , mCfg, logCfg);

    setupResources();

    bool carryOn = configure();
    if (!carryOn) return false;

    chooseSceneManager();
    createCamera();
    createViewports();

	//init video capture
	initVideoCap(1,640,480,800,800,false,false);

	//inti Artoolkit
	initArtoolkit();

    // Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    // Create any resource listeners (for loading screens)
    createResourceListener();
    // Load resources
    loadResources();

    // Create the scene
    createScene();

    createFrameListener();

    return true;
};


//-------------------------------------------------------------------------------------
Ogre::PixelFormat BaseApplication::openCVToOgrePixFormat(const cv::Mat& img)
{

	switch(img.channels())
	{
	case 1:
		return Ogre::PF_L8;
	case 3:
		return Ogre::PF_R8G8B8;
		//return Ogre::PF_B8G8R8;
	case 4:
		return Ogre::PF_A8R8G8B8;
		//return Ogre::PF_A8B8G8R8;
		//return Ogre::PF_R8G8B8A8;
		//return Ogre::PF_B8G8R8A8;
	default:
		return Ogre::PF_UNKNOWN;
	}
}
	
	
//-------------------------------------------------------------------------------------
bool BaseApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(mWindow->isClosed())
        return false;

    if(mShutDown)
        return false;


    //Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();

    mTrayMgr->frameRenderingQueued(evt);

    if (!mTrayMgr->isDialogVisible())
    {
        mCameraMan->frameRenderingQueued(evt);   // if dialog isn't up, then update the camera
        if (mDetailsPanel->isVisible())   // if details panel is visible, then update its contents
        {
            mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(mCamera->getDerivedPosition().x));
            mDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(mCamera->getDerivedPosition().y));
            mDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(mCamera->getDerivedPosition().z));
            mDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().w));
            mDetailsPanel->setParamValue(5, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().x));
            mDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().y));
            mDetailsPanel->setParamValue(7, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().z));
        }
    }
	

	//Video capture
	boost::unique_lock<boost::mutex> lock(mMutex , boost::try_to_lock );
	if( lock.owns_lock() && mNewImage)
	{
		mNewImage = false;

		if(mBackgroundTexture.isNull())
		{
			initBackgroundTexture();
			return true;
		}
		if(mCapImg.cols != mBackgroundTexture->getWidth()|| mCapImg.rows != mBackgroundTexture->getHeight())
		{
			initBackgroundTexture();
			return true;
		}
		
		//Update background texture		
		Ogre::Image backgroundImage;
		Ogre::PixelFormat backgroundImgFormat = openCVToOgrePixFormat(mCapImg);
		backgroundImage.loadDynamicImage(mCapImg.data , mCapImg.cols , mCapImg.rows , 1,backgroundImgFormat);
		mBackgroundTexture->getBuffer()->blitFromMemory( backgroundImage.getPixelBox());	

		//Tracking
		track();
	}

    return true;
}

void BaseApplication::normalizeRotationVector(const Ogre::Vector3& rotVect , double& rotNorm , Ogre::Vector3& normalizedVectRot)
{
	rotNorm = sqrt( rotVect.x*rotVect.x + rotVect.y*rotVect.y + rotVect.z*rotVect.z);
	if(rotNorm !=0.0f){
		normalizedVectRot.x = rotVect.x/rotNorm;
		normalizedVectRot.y = rotVect.y/rotNorm;
		normalizedVectRot.z = rotVect.z/rotNorm;
	}
	else{
		normalizedVectRot.x = 0.0f;
		normalizedVectRot.y = 0.0f;
		normalizedVectRot.z = 0.0f;
	}
}

void BaseApplication::convertArtoolkitTransMat(double patt_trans[3][4] , Ogre::Vector3& pos , Ogre::Quaternion& quat)
{
	double rot_mat[3][3];
	double rot_x[3][3];
	double final_rot_mat[3][3];
	
	double rot_vect[3];
	CvMat cvrot_mat, cvrotx_mat, cvfinalrot_mat, cvrot_vect;

	cvInitMatHeader(&cvrot_mat, 3, 3, CV_64FC1, rot_mat, 3*sizeof(double));
	cvInitMatHeader(&cvrotx_mat, 3, 3, CV_64FC1, rot_x, 3*sizeof(double));
	cvInitMatHeader(&cvfinalrot_mat, 3, 3, CV_64FC1, final_rot_mat, 3*sizeof(double));
	cvInitMatHeader(&cvrot_vect, 1, 3, CV_64FC1, rot_vect,3*sizeof(double));

	//copy original matrix
	for (int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			rot_mat[i][j] = patt_trans[i][j];
		}
	}
	
	////// Pi rotation around x axis
	////rot_x[0][0] = 1.0;		rot_x[0][1] = 0.0;		rot_x[0][2] = 0.0;
	////rot_x[1][0] = 0.0;		rot_x[1][1] = -1.0;		rot_x[1][2] = 0.0;
	////rot_x[2][0] = 0.0;		rot_x[2][1] = 0.0;		rot_x[2][2] = -1.0;
	rot_x[0][0] = 1.0;		rot_x[0][1] = 0.0;		rot_x[0][2] = 0.0;
	rot_x[1][0] = 0.0;		rot_x[1][1] = 0.0;		rot_x[1][2] = 1.0;
	rot_x[2][0] = 0.0;		rot_x[2][1] = -1.0;		rot_x[2][2] = 0.0;
	
	cvMatMul( &cvrot_mat, &cvrotx_mat, &cvfinalrot_mat );
	
	
	cvRodrigues2(&cvfinalrot_mat, &cvrot_vect);
	pos.x = patt_trans[0][3];
	pos.y = -patt_trans[1][3];
	pos.z = -patt_trans[2][3];

	Ogre::Vector3 rotVect;
	rotVect.x = rot_vect[0];
	rotVect.y = -rot_vect[1];
	rotVect.z = -rot_vect[2];

	Ogre::Vector3 notmalizedRotVect;
	double normRot;

	normalizeRotationVector(rotVect,normRot,notmalizedRotVect);

	Ogre::Vector3 rot((float)notmalizedRotVect.x , (float)notmalizedRotVect.y , (float)notmalizedRotVect.z );	
	quat = Ogre::Quaternion(Ogre::Radian((float)normRot),rot);
}


//-------------------------------------------------------------------------------------
void BaseApplication::track()
{
	ARMarkerInfo*   marker_info=NULL;
	int             marker_num=0;
	double          patt_center[2] = {0.0, 0.0};
	double          patt_trans[3][4];

	cv::cvtColor(mCapImg,mRGBAImg, CV_RGB2RGBA/*CV_BGR2BGRA*/, 4);

	//Reset results
	for(size_t k=0 ; k<trackResults.size() ; k++)
		trackResults[k].found = false;

	//Call artoolkit tracking
	if(arDetectMarker((ARUint8 *)mRGBAImg.data, 50, &marker_info, &marker_num)<0)
		return;
	
	for(size_t k=0 ; k<trackResults.size() ; k++)
	{
		for(int j = 0; j < marker_num; j++ )
		{
			if(marker_info[j].cf < 0.7)
				continue;

			if( trackResults[k].id == marker_info[j].id )
			{
				if(trackResults[k].found && trackResults[k].score>marker_info[j].cf)
					continue;
				
				trackResults[k].found = true;
				trackResults[k].score = marker_info[j].cf;

				arGetTransMat(&marker_info[j], patt_center, trackResults[k].pattWidth, patt_trans);
				
				convertArtoolkitTransMat(patt_trans , trackResults[k].pos , trackResults[k].quat);

			}
		}
	}
}


//-------------------------------------------------------------------------------------
void BaseApplication::updateTrackedNode(const TrackResult& trackRes, Ogre::SceneNode* node )
{
	if(!node)
		return;

	if(!trackRes.found)
	{
		if(node->isInSceneGraph())
		{
			if(node->getParent())
				node->getParent()->removeChild(node);
		}
	}
	else
	{
		if( !node->isInSceneGraph() )
		{
			if(node->getParent())
				node->getParent()->removeChild(node);
			
			mSceneMgr->getRootSceneNode()->addChild(node);
		}
		node->setVisible(true,false);
		node->setPosition(trackRes.pos);
		node->setOrientation(trackRes.quat);
		node->needUpdate(true);
	}
}

//-------------------------------------------------------------------------------------
bool BaseApplication::keyPressed( const OIS::KeyEvent &arg )
{
    if (mTrayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up

    if (arg.key == OIS::KC_F)   // toggle visibility of advanced frame stats
    {
        mTrayMgr->toggleAdvancedFrameStats();
    }
    else if (arg.key == OIS::KC_G)   // toggle visibility of even rarer debugging details
    {
        if (mDetailsPanel->getTrayLocation() == OgreBites::TL_NONE)
        {
            mTrayMgr->moveWidgetToTray(mDetailsPanel, OgreBites::TL_TOPRIGHT, 0);
            mDetailsPanel->show();
        }
        else
        {
            mTrayMgr->removeWidgetFromTray(mDetailsPanel);
            mDetailsPanel->hide();
        }
    }
    else if (arg.key == OIS::KC_T)   // cycle polygon rendering mode
    {
        Ogre::String newVal;
        Ogre::TextureFilterOptions tfo;
        unsigned int aniso;

        switch (mDetailsPanel->getParamValue(9).asUTF8()[0])
        {
        case 'B':
            newVal = "Trilinear";
            tfo = Ogre::TFO_TRILINEAR;
            aniso = 1;
            break;
        case 'T':
            newVal = "Anisotropic";
            tfo = Ogre::TFO_ANISOTROPIC;
            aniso = 8;
            break;
        case 'A':
            newVal = "None";
            tfo = Ogre::TFO_NONE;
            aniso = 1;
            break;
        default:
            newVal = "Bilinear";
            tfo = Ogre::TFO_BILINEAR;
            aniso = 1;
        }

        Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(tfo);
        Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(aniso);
        mDetailsPanel->setParamValue(9, newVal);
    }
    else if (arg.key == OIS::KC_R)   // cycle polygon rendering mode
    {
        Ogre::String newVal;
        Ogre::PolygonMode pm;

        switch (mCamera->getPolygonMode())
        {
        case Ogre::PM_SOLID:
            newVal = "Wireframe";
            pm = Ogre::PM_WIREFRAME;
            break;
        case Ogre::PM_WIREFRAME:
            newVal = "Points";
            pm = Ogre::PM_POINTS;
            break;
        default:
            newVal = "Solid";
            pm = Ogre::PM_SOLID;
        }

        mCamera->setPolygonMode(pm);
        mDetailsPanel->setParamValue(10, newVal);
    }
    else if(arg.key == OIS::KC_F5)   // refresh all textures
    {
        Ogre::TextureManager::getSingleton().reloadAll();
    }
    else if (arg.key == OIS::KC_SYSRQ)   // take a screenshot
    {
        mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
    }
    else if (arg.key == OIS::KC_ESCAPE)
    {
        mShutDown = true;
    }

    //mCameraMan->injectKeyDown(arg);
    return true;
}

bool BaseApplication::keyReleased( const OIS::KeyEvent &arg )
{
    //mCameraMan->injectKeyUp(arg);
    return true;
}

bool BaseApplication::mouseMoved( const OIS::MouseEvent &arg )
{
    //if (mTrayMgr->injectMouseMove(arg)) return true;
    //mCameraMan->injectMouseMove(arg);
    return true;
}

bool BaseApplication::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
   // if (mTrayMgr->injectMouseDown(arg, id)) return true;
    //mCameraMan->injectMouseDown(arg, id);
    return true;
}

bool BaseApplication::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
   // if (mTrayMgr->injectMouseUp(arg, id)) return true;
    //mCameraMan->injectMouseUp(arg, id);
    return true;
}

//Adjust mouse clipping area
void BaseApplication::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

//Unattach OIS before window shutdown (very important under Linux)
void BaseApplication::windowClosed(Ogre::RenderWindow* rw)
{
    //Only close for window that created OIS (the main window in these demos)
    if( rw == mWindow )
    {
        if( mInputManager )
        {
            mInputManager->destroyInputObject( mMouse );
            mInputManager->destroyInputObject( mKeyboard );

            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }
}
