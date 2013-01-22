#include "header.h"
#include "Rahpong.h"

Rahpong::Rahpong(void) :
	mBaseRotationSpeed(0.2f), mPalet1RotationSpeed(0.1f), mBallRotationSpeed(NULL),
	mBallNode(NULL), mBallEntity(NULL),
	mPaletEntity1(NULL), mPaletNode1(NULL), 
	mPaletEntity2(NULL), mPaletNode2(NULL),
	mPongPaletBaseNode1(NULL), mPongPaletBaseNode2(NULL),
	mBallBaseNode(NULL),
	mPongPaletTrackNode1(NULL), mPongPaletTrackNode2(NULL),
	ballSpeed(0.06f), positionBall(0.0f, 20.0f, 0.0f),
	idCurrentPad(0)
{
	//pathCfg = "E:/Codage/Cours/Rahpong/data";
	isBallVisible = false;
	i = 0;
	deltaX = 0.01f;
	nodeFound = (bool*)calloc(2, sizeof(bool));
	nodeFound[0] = false;
	nodeFound[1] = false;
	leftToRight = true;
}

void Rahpong::createScene(void) {

	mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0f, 1.0f, 1.0f));

	mBallEntity = mSceneMgr->createEntity("Ball", "palet.mesh");
	mBallNode = mSceneMgr->createSceneNode("BallNode");
	//mBallNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("BallBaseNode");
	mBallNode->setScale(0.02f, 0.02f, 0.02f);
    mBallNode->attachObject(mBallEntity);
	mBallNode->pitch(Ogre::Radian(-0.77f));

	// Create the pongtabs scenes nodes and entities
	mPaletEntity1 = mSceneMgr->createEntity("Palet1", "palet.mesh");
	mPaletNode1 = mSceneMgr->createSceneNode("PaletNode1");
	mPaletNode1->setScale(0.04f, 0.04f, 0.04f);
    mPaletNode1->attachObject(mPaletEntity1);
	mPaletNode1->pitch(Ogre::Radian(-0.57f));

	mPongPaletBaseNode1 = mSceneMgr->createSceneNode("PongPaletBaseNode1");
	mPongPaletBaseNode1->addChild(mPaletNode1);
	mPongPaletBaseNode1->addChild(mBallNode);
	mPaletNode1->setPosition(0.0f, 0.0f, 0.0f);
	mBallNode->setPosition(-60.0f, 0.0f, 0.0f);
	idCurrentPad = 0;

	mPongPaletTrackNode1 = mSceneMgr->getRootSceneNode()->createChildSceneNode("PaletTrackNode1");
	mPongPaletTrackNode1->addChild(mPongPaletBaseNode1);
	mPongPaletBaseNode1->setPosition(0.0f, 45.0f, 0.0f);

	mPaletEntity2 = mSceneMgr->createEntity("Palet2", "cube.mesh");
	mPaletNode2 = mSceneMgr->createSceneNode("PaletNode2");
	mPaletNode2->setScale(90.0f, 90.0f, 90.0f);
    mPaletNode2->attachObject(mPaletEntity2);
	//mPaletNode2->pitch(Ogre::Radian(-0.57f));

	mPongPaletBaseNode2 = mSceneMgr->createSceneNode("PongPaletBaseNode2");
	mPongPaletBaseNode2->addChild(mPaletNode2);
	mPaletNode2->setPosition(0.0f, 0.0f, 0.0f);

	mPongPaletTrackNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode("PaletTrackNode2");
	mPongPaletTrackNode2->addChild(mPongPaletBaseNode2);
	mPongPaletBaseNode2->setPosition(0.0f, 45.0f, 0.0f);

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
    /*switch (evt.key)
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
    }*/
    return true;
}

bool Rahpong::frameRenderingQueued(const Ogre::FrameEvent &evt) {

	//Call base class
	if(!BaseApplication::frameRenderingQueued(evt))
		return false;

	//Update your scene here

	//Manage tracking results	
	updateTrackedNode(trackResults[0] , mPongPaletTrackNode1, 0);
	updateTrackedNode(trackResults[1] , mPongPaletTrackNode2, 1);

	if(mPaletNode1)
		mPaletNode1->roll(Ogre::Radian(evt.timeSinceLastFrame * mPalet1RotationSpeed));
	if(mBallNode)
		mBallNode->roll(Ogre::Radian(evt.timeSinceLastFrame * mPalet1RotationSpeed)); 

	if( nodeFound[0] && !nodeFound[1] ) {
		mBallNode->setVisible(true);
		mBallNode->getParent()->removeChild(mBallNode);
		mPongPaletTrackNode1->addChild(mBallNode);
		mBallNode->setPosition(20.0f, 100.0f, 0.0f);

		//Ogre::LogManager::getSingletonPtr()->logMessage("Found[01] - "+Ogre::StringConverter::toString(nodeFound[0]));

	} else if ( nodeFound[1] && !nodeFound[0] ) {
		mBallNode->setVisible(true);
		mBallNode->getParent()->removeChild(mBallNode);
		mPongPaletTrackNode2->addChild(mBallNode);
		mBallNode->setPosition(20.0f, 100.0f, 0.0f);

		//Ogre::LogManager::getSingletonPtr()->logMessage("Found[1] - "+Ogre::StringConverter::toString(nodeFound[1]));

	// if both pads are visible
	} else if ( nodeFound[0] && nodeFound[1] ) {
		
		mBallNode->setVisible(true);
		positionBall = mBallNode->getPosition();

		// trajectoire of mBallNode
		Ogre::Vector3 delta = calculTrajectoire();

		mBallNode->setPosition(positionBall.x+delta.x, positionBall.y+delta.y, positionBall.z+delta.z);

		/*Ogre::LogManager::getSingletonPtr()->logMessage("looooog - (x: "+
			Ogre::StringConverter::toString(positionBall.x)+", y: "+
			Ogre::StringConverter::toString(positionBall.y)+", z: "+
			Ogre::StringConverter::toString(positionBall.z)+")");

		Ogre::LogManager::getSingletonPtr()->logMessage("Found[0] - "+Ogre::StringConverter::toString(nodeFound[0]));
		*/

		if( collisionOtherPad(mBallNode, idCurrentPad) ) {
			switchBaseNodeParent(idCurrentPad);
		}

		//Ogre::LogManager::getSingletonPtr()->logMessage("BaseNodeParent: "+Ogre::StringConverter::toString(idCurrentPad));
		
	} else if ( !nodeFound[0] && !nodeFound[1] ){
		/*Ogre::LogManager::getSingletonPtr()->logMessage("Found[0] - "+
			Ogre::StringConverter::toString(nodeFound[0])+ "Found[1] - "+
			Ogre::StringConverter::toString(nodeFound[1]));*/
		mBallNode->setVisible(false);
	}

	return true;
}


void Rahpong::updateTrackedNode(const TrackResult& trackRes, Ogre::SceneNode* node, int id )
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
		nodeFound[id] = false;
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
		nodeFound[id] = true;
	}
}

// check if Ball enter in colision with the pad which isn't his parent
// @param node: mBallNode
// @param idCurBaseNode: BaseNode parent of node
bool Rahpong::collisionOtherPad(Ogre::SceneNode *node, const int idCurBaseNode){
	Ogre::SceneNode *toColide = NULL;
	Ogre::Vector3 ballPos = node->getParent()->getPosition()+node->getPosition();

	if( idCurBaseNode == 0 ) {
		toColide = mPongPaletBaseNode2;
		if ( mPongPaletBaseNode1->getPosition().x+D_Pad_BaseNode > mPongPaletBaseNode2->getPosition().x+D_Pad_BaseNode ) {
			leftToRight = false;
		} else {
			leftToRight = true;
		}
	} else {
		toColide = mPongPaletBaseNode1;
		if ( mPongPaletBaseNode1->getPosition().x+D_Pad_BaseNode < mPongPaletBaseNode2->getPosition().x+D_Pad_BaseNode ) {
			leftToRight = true;
		} else {
			leftToRight = false;
		}
	}

	Ogre::LogManager::getSingletonPtr()->logMessage("Direction: "+Ogre::StringConverter::toString(leftToRight));

	//ALGO
	Ogre::Vector3 padPos = toColide->getPosition();
	/*Ogre::LogManager::getSingletonPtr()->logMessage("BallPos.x: "+Ogre::StringConverter::toString(ballPos.x)+
													", padPos.x: "+Ogre::StringConverter::toString(padPos.x));*/
	if ( leftToRight ) {
		if ( ballPos.x >= padPos.x+D_Pad_BaseNode /*&& ballPos.z <= padPos.z+toColide->getScale().z/2 && ballPos.z >= padPos.z-toColide->getScale().z/2*/) {
			return true;
		}

	} else {
		if ( ballPos.x <= padPos.x-D_Pad_BaseNode /*&& ballPos.z <= padPos.z+toColide->getScale().z/2 && ballPos.z >= padPos.z-toColide->getScale().z/2*/) {
			return true;
		}
	}
	//

	return false;
}

// switch parent of ballNode with other pad
void Rahpong::switchBaseNodeParent( const int idCurBaseNode ){
	mBallNode->getParent()->removeChild(mBallNode);
	
	if( idCurBaseNode == 0 ) {
		mPongPaletTrackNode2->addChild(mBallNode);
		idCurrentPad = 1;
	} else {
		mPongPaletTrackNode1->addChild(mBallNode);
		idCurrentPad = 0;
	}
	mBallNode->setPosition(0.0f, 0.0f, 0.0f);
	Ogre::LogManager::getSingletonPtr()->logMessage("Switch parent to "+Ogre::StringConverter::toString(idCurrentPad));
}

// return a vector which represent the next step to reach other pad
// TODO, Extremly hard to implement
Ogre::Vector3 Rahpong::calculTrajectoire(void){
	Ogre::Vector3 traj = Ogre::Vector3();

	Ogre::Real x = 0.0f;
	Ogre::Real z = 0.0f;
	
	//tmp
	x = ballSpeed;
	//z = 0.01f;

	if( !leftToRight ) x = -1*x;

	traj.x = x;
	traj.y = 0.0f;
	traj.z = z;

	return traj;
}
