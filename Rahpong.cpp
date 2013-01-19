#include "header.h"
#include "Rahpong.h"

Rahpong::Rahpong(void) :
	mRoot(0),
    mCamera(0),
    mSceneMgr(0),
    mWindow(0),
	resourcesCfg(Ogre::StringUtil::BLANK),
    pluginsCfg(Ogre::StringUtil::BLANK),
	logCfg(Ogre::StringUtil::BLANK),
	mPongTabNode(NULL),
	mPongTabEntity(NULL)
{
	pathCfg = "E:/Codage/Cours/Rahpong/data";

	//Remove ourself as a Window listener
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);
    delete mRoot;
}

void Rahpong::go(void) {

#ifdef _DEBUG
	pluginsCfg = pathCfg+"/plugins_d.cfg";
#else
    pluginsCfg = pathCfg+"/plugins.cfg";
#endif
	resourcesCfg = pathCfg+"/resources.cfg";
	ogreCfg = pathCfg+"/ogre.cfg";
	logCfg = pathCfg+"/RahpongLog.txt";

    if (!setup())
        return;

    mRoot->startRendering();

    destroyScene();
}

bool Rahpong::setup(void) {
	mRoot = new Ogre::Root(pluginsCfg, ogreCfg, logCfg);

    setupResources();

	if( !configure() ) { return false; }

	// Get the SceneManager, in this case a generic one
    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
    createCamera();
    createViewports();

	//init video capture
	//initVideoCap(1,640,480,800,800,false,false);

	//inti Artoolkit
	//initArtoolkit();

    // Set default mipmap level (NB some APIs ignore this)
    //Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    // Create any resource listeners (for loading screens)
    //createResourceListener();
    // Load resources
    loadResources();

    // Create the scene
    createScene();

    //createFrameListener();

    return false;
}

bool Rahpong::configure(void) {

	//if( mRoot->restoreConfig() ) { return true; }

    if( mRoot->showConfigDialog() ) {
        mWindow = mRoot->initialise( true, "Application Render Window" );

        return true;
    } else {
        return false;
    }
}

void Rahpong::createScene(void) {

	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.0f, 1.0f, 1.0f));

	// Create the pongtab scene node and entity
	mPongTabEntity = mSceneMgr->createEntity("PongTab", "pongtab.mesh");

	mPongTabNode = mSceneMgr->createSceneNode("PongTabNode");
	mPongTabNode->setScale(0.06f,0.06f,0.06f);
    mPongTabNode->attachObject(mPongTabEntity);
	mPongTabNode->pitch(Ogre::Radian(-1.57f));

    // Create a Light and set its position
	Ogre::Light* light = mSceneMgr->createLight("MainLight");
    light->setPosition(20.0f, 80.0f, 50.0f);
}


void Rahpong::destroyScene(void) {

}

void Rahpong::createCamera(void) {
    // Create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");

    mCamera->setPosition(Ogre::Vector3(0,0,0));
    
    mCamera->lookAt(Ogre::Vector3(0,0,-300));
    mCamera->setNearClipDistance(0.1f);

    //mCameraMan = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
}

void Rahpong::createViewports(void) {
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(
        Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}

void Rahpong::setupResources(void) {
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load(resourcesCfg);

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

void Rahpong::loadResources(void) {
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}


//Adjust mouse clipping area
void Rahpong::windowResized(Ogre::RenderWindow* rw) {
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

//Unattach OIS before window shutdown (very important under Linux)
void Rahpong::windowClosed(Ogre::RenderWindow* rw) {
    //Only close for window that created OIS (the main window in these demos)
    if( rw == mWindow ) {
        if( mInputManager ) {
            mInputManager->destroyInputObject( mMouse );
            mInputManager->destroyInputObject( mKeyboard );

            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }
}
