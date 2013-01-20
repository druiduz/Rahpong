#include "header.h"
#include "Rahpong.h"

Rahpong::Rahpong(void) :
	mEarthEntity(NULL), mEarthNode(NULL), mEarthRotationSpeed(-0.5f),
	mCubeEntity(NULL),	mCubeNode(NULL), mCubeRotationSpeed(0.75f),
	mBaseRotationSpeed(0.2f),
	mPaletEntity(NULL),	mPaletNode(NULL), 
	mEarthTrackNode(NULL), mCube2TrackNode(NULL)
{
	pathCfg = "E:/Codage/Cours/Rahpong/data";
}

void Rahpong::createScene(void) {

	mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0f, 1.0f, 1.0f));

	// Create the pongtab scene node and entity
	mPaletEntity = mSceneMgr->createEntity("Palet", "palet.mesh");

	mPaletNode = mSceneMgr->createSceneNode("PaletNode");
	mPaletNode->setScale(0.06f,0.06f,0.06f);
    mPaletNode->attachObject(mPaletEntity);
	mPaletNode->pitch(Ogre::Radian(-0.57f));

	//Create earth entity
    mEarthEntity = mSceneMgr->createEntity("Earth", "sphere.mesh");

	//Create cube entity
    mCubeEntity = mSceneMgr->createEntity("Cube", "cube.mesh");

    // Create the earth scene node 
    mEarthNode = mSceneMgr->createSceneNode("EarthNode");
	mEarthNode->setScale(0.06f,0.06f,0.06f);
    mEarthNode->attachObject(mEarthEntity);
	mEarthNode->pitch(Ogre::Radian(-0.57f));

    // Create the cube scene node
    mCubeNode = mSceneMgr->createSceneNode("CubeNode");
	mCubeNode->setScale(90.0f,90.0f,90.0f);
    mCubeNode->attachObject(mCubeEntity);
	mCubeNode->pitch(Ogre::Radian(0.3f));

	//Create the base node and add earth and cube nodes to it
	mEarthBaseNode = mSceneMgr->createSceneNode("EarthBaseNode");
	mEarthBaseNode->addChild(mEarthNode);
	mEarthBaseNode->addChild(mPaletNode);
	mEarthNode->setPosition(-120.0f, 0.0f, 0.0f);
	mPaletNode->setPosition(120.0f, 0.0f, 0.0f);

	//Create the tracked node for earth (top parent)
	mEarthTrackNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("EarthTrackNode");
	mEarthTrackNode->addChild(mEarthBaseNode);
	mEarthBaseNode->setPosition(0.0f, 45.0f, 0.0f);

	//Create another cube for the second pattern
	Ogre::Entity* cubeEntity2 = mSceneMgr->createEntity("Cube2", "cube.mesh");

    // Create the second cube scene node
	Ogre::SceneNode* cubeNode2 = mSceneMgr->createSceneNode("CubeNode2");
	cubeNode2->setScale(110.0f,110.0f,110.0f);
    cubeNode2->attachObject(cubeEntity2);

	//Create the tracked node for the second cube 
	mCube2TrackNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Cube2TrackNode");
	mCube2TrackNode->addChild(cubeNode2);
	cubeNode2->setPosition(0.0f, 55.0f, 0.0f);

    // Create a Light and set its position
    Ogre::Light* light = mSceneMgr->createLight("MainLight");
    light->setPosition(20.0f, 80.0f, 50.0f);

}


void Rahpong::createFrameListener(void)
{
	BaseApplication::createFrameListener();
}

bool Rahpong::keyPressed( const OIS::KeyEvent& evt ) {
    if (evt.key == OIS::KC_ESCAPE)
    {
        mShutDown = true;
    }

	return true;
}

bool Rahpong::keyReleased( const OIS::KeyEvent& evt ) {
    switch (evt.key)
    {
	case OIS::KC_Q : 
        mEarthRotationSpeed -= 0.1f;
        break;
	case OIS::KC_W : 
        mEarthRotationSpeed += 0.1f;
        break;

	case OIS::KC_A : 
        mCubeRotationSpeed -= 0.1f;
        break;
	case OIS::KC_S : 
        mCubeRotationSpeed += 0.1f;
        break;

	case OIS::KC_Z : 
        mBaseRotationSpeed -= 0.1f;
        break;
	case OIS::KC_X : 
        mBaseRotationSpeed += 0.1f;
        break;
    default:
        break;
    }
    return true;
}

bool Rahpong::frameRenderingQueued(const Ogre::FrameEvent &evt) {

	//Call base class
	if(!BaseApplication::frameRenderingQueued(evt))
		return false;

	//Update your scene here

	//Manage tracking results	
	updateTrackedNode(trackResults[0] , mEarthTrackNode);
	updateTrackedNode(trackResults[1] , mCube2TrackNode);

	//Rotate the earth
	if(mEarthNode)
		mEarthNode->roll(Ogre::Radian(evt.timeSinceLastFrame * mEarthRotationSpeed)); 

	//Rotate the first cube
	if(mCubeNode)
	{
		mCubeNode->roll(Ogre::Radian(evt.timeSinceLastFrame * mCubeRotationSpeed));
		mCubeNode->yaw(Ogre::Radian(evt.timeSinceLastFrame * mCubeRotationSpeed));
	}

	//Rotate the base
	if(mEarthBaseNode)
		mEarthBaseNode->yaw(Ogre::Radian(evt.timeSinceLastFrame * mBaseRotationSpeed));

	return true;
}
