#include "header.h"
#include "Rahpong.h"

Rahpong::Rahpong(void) :
	mBaseRotationSpeed(0.2f), mPalet1RotationSpeed(0.1f), mBallRotationSpeed(NULL),
	mBallNode(NULL), mBallEntity(NULL),
	mPaletEntity1(NULL), mPaletNode1(NULL), 
	mPaletEntity2(NULL), mPaletNode2(NULL),
	mPongPaletBaseNode(NULL),
	mBallBaseNode(NULL),
	mPongPaletTrackNode1(NULL), mPongPaletTrackNode2(NULL),
	ballSpeed(0.001f), positionBall(0.0f, 0.0f, 0.0f)
{
	//pathCfg = "E:/Codage/Cours/Rahpong/data";
	isBallVisible = false;
}

void Rahpong::createScene(void) {

	mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0f, 1.0f, 1.0f));

	mBallEntity = mSceneMgr->createEntity("Ball", "palet.mesh");
	mBallNode = mSceneMgr->createSceneNode("BallNode");
	mBallNode->setScale(0.02f, 0.02f, 0.02f);
    mBallNode->attachObject(mBallEntity);
	mBallNode->pitch(Ogre::Radian(-0.77f));

	mBallBaseNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("BallBaseNode");
	mBallBaseNode->addChild(mBallNode);
	mBallNode->setPosition(0.0f, 0.0f, 0.0f);

	// Create the pongtabs scenes nodes and entities
	mPaletEntity1 = mSceneMgr->createEntity("Palet1", "palet.mesh");
	mPaletNode1 = mSceneMgr->createSceneNode("PaletNode1");
	mPaletNode1->setScale(0.04f, 0.04f, 0.04f);
    mPaletNode1->attachObject(mPaletEntity1);
	mPaletNode1->pitch(Ogre::Radian(-0.57f));

	mPongPaletBaseNode = mSceneMgr->createSceneNode("PongPaletBaseNode");
	mPongPaletBaseNode->addChild(mPaletNode1);
	//mPongPaletBaseNode->addChild(mBallNode);
	mPaletNode1->setPosition(0.0f, 0.0f, 0.0f);
	//mBallNode->setPosition(-60.0f, 0.0f, 0.0f);

	mPongPaletTrackNode1 = mSceneMgr->getRootSceneNode()->createChildSceneNode("PaletTrackNode1");
	mPongPaletTrackNode1->addChild(mPongPaletBaseNode);
	mPongPaletBaseNode->setPosition(0.0f, 45.0f, 0.0f);

	mPaletEntity2 = mSceneMgr->createEntity("Palet2", "cube.mesh");
	mPaletNode2 = mSceneMgr->createSceneNode("PaletNode2");
	mPaletNode2->setScale(90.0f, 90.0f, 90.0f);
    mPaletNode2->attachObject(mPaletEntity2);
	//mPaletNode2->pitch(Ogre::Radian(-0.57f));

	mPongPaletTrackNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode("PaletTrackNode2");
	mPongPaletTrackNode2->addChild(mPaletNode2);
	mPaletNode2->setPosition(0.0f, 45.0f, 0.0f);

    // Create a Light and set its position
    Ogre::Light* light = mSceneMgr->createLight("MainLight");
    light->setPosition(20.0f, 80.0f, 50.0f);

	/*Ogre::String pouet = pathCfg+"/connard";
		Ogre::LogManager::getSingletonPtr()->logMessage(pouet);*/
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
        mPalet1RotationSpeed -= 0.1f;
        break;
	case OIS::KC_W : 
        mPalet1RotationSpeed += 0.1f;
        break;

	case OIS::KC_A : 
        mBallRotationSpeed -= 0.1f;
        break;
	case OIS::KC_S : 
        mBallRotationSpeed += 0.1f;
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
	updateTrackedNode(trackResults[0] , mPongPaletTrackNode1);
	updateTrackedNode(trackResults[1] , mPongPaletTrackNode2);

	if(mPaletNode1)
		mPaletNode1->roll(Ogre::Radian(evt.timeSinceLastFrame * mPalet1RotationSpeed));
	if(mBallNode)
		mBallNode->roll(Ogre::Radian(evt.timeSinceLastFrame * mPalet1RotationSpeed)); 

	if( (mPaletNode1 || mPaletNode2) && !isBallVisible ) {
		if( positionBall.x == 0.0f && positionBall.y == 0.0f && positionBall.z == 0.0f ) {
			positionBall = mPaletNode1->getParent()->getParent()->getPosition();
		}
		mBallBaseNode->setPosition(positionBall.x+30.0f, positionBall.y, positionBall.z);
		mBallBaseNode->setVisible(true);
		mBallNode->setVisible(true);

		isBallVisible = true;

		Ogre::LogManager::getSingletonPtr()->logMessage("BallVisible");

	} else if( (mPaletNode1 || mPaletNode2) && isBallVisible ) {
		positionBall = mBallNode->getPosition();
		
		Ogre::Real deltaX = evt.timeSinceLastFrame *ballSpeed/1000;
		mBallNode->translate(deltaX, 0.0f, 0.0f);
		positionBall.x += deltaX;

		Ogre::LogManager::getSingletonPtr()->logMessage("looooog");

	} /*else if( !mPaletNode1 ) {
		mBallBaseNode->setVisible(false);
		mBallNode->setVisible(false);
		isBallVisible = false;
		Ogre::LogManager::getSingletonPtr()->logMessage("BallNonVisible");
	}*/

	return true;
}
