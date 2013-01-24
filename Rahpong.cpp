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
	ballSpeedX(0.10f), ballSpeedZ(0.03f), positionBall(0.0f, 20.0f, 0.0f), positionBall2(0.0f, 0.0f, 0.0f),
	idCurrentPad(0)
{
	//pathCfg = "E:/Codage/Cours/Rahpong/data";
	nodeFound = (bool*)calloc(2, sizeof(bool));
	nodeFound[0] = false;
	nodeFound[1] = false;
	leftToRight = true;
	upToDown = 0;
	direction = true;
}

void Rahpong::createScene(void) {

	mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0f, 1.0f, 1.0f));

	mBallEntity = mSceneMgr->createEntity("Ball", "palet.mesh");
	mBallNode = mSceneMgr->createSceneNode("BallNode");
	mBallNode->setScale(0.02f, 0.02f, 0.02f);
    mBallNode->attachObject(mBallEntity);
	mBallNode->pitch(Ogre::Radian(-0.77f));

	// Pas utilisé, prévu pour la suite si on met la balle indépendante des track
	mBallEntity2 = mSceneMgr->createEntity("Ball2", "palet.mesh");
	mBallNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode("BallNode2");
	mBallNode2->setScale(0.02f, 0.02f, 0.02f);
    mBallNode2->attachObject(mBallEntity2);
	mBallNode2->pitch(Ogre::Radian(-0.77f));

	// Create the pongtabs scenes nodes and entities
	mPaletEntity1 = mSceneMgr->createEntity("Palet1", "cube.mesh");
	mPaletNode1 = mSceneMgr->createSceneNode("PaletNode1");
	//mPaletNode1->setScale(10.0f, 10.0f, 5.0f);
	mPaletNode1->setScale(10.0f, 10.0f, 90.0f);
    mPaletNode1->attachObject(mPaletEntity1);

	mPongPaletTrackNode1 = mSceneMgr->getRootSceneNode()->createChildSceneNode("PaletTrackNode1");
	mPongPaletTrackNode1->addChild(mPaletNode1);
	mPongPaletTrackNode1->addChild(mBallNode);
	mPaletNode1->setPosition(0.0f, 45.0f, 0.0f);
	mBallNode->setPosition(-50.0f, 45.0f, 0.0f);
	idCurrentPad = 0;

	mPaletEntity2 = mSceneMgr->createEntity("Palet2", "cube.mesh");
	mPaletNode2 = mSceneMgr->createSceneNode("PaletNode2");
	mPaletNode2->setScale(10.0f, 10.0f, 90.0f);
    mPaletNode2->attachObject(mPaletEntity2);

	mPongPaletTrackNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode("PaletTrackNode2");
	mPongPaletTrackNode2->setDebugDisplayEnabled(true);
	mPongPaletTrackNode2->addChild(mPaletNode2);
	mPaletNode2->setPosition(0.0f, 45.0f, 0.0f);

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
        ballSpeedX -= 0.02f;
        break;
	case OIS::KC_W : 
        ballSpeedX += 0.02f;
        break;

	case OIS::KC_A : 
        ballSpeedZ -= 0.01f;
        break;
	case OIS::KC_S : 
        ballSpeedX += 0.01f;
        break;
/*
	case OIS::KC_Z : 
        mBaseRotationSpeed -= 0.1f;
        break;
	case OIS::KC_X : 
        mBaseRotationSpeed += 0.1f;
        break;*/
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
	updateTrackedNode(trackResults[0] , mPongPaletTrackNode1, 0);
	updateTrackedNode(trackResults[1] , mPongPaletTrackNode2, 1);

	//if(mPaletNode1)
	//	mPaletNode1->roll(Ogre::Radian(evt.timeSinceLastFrame * mPalet1RotationSpeed));
	//if(mBallNode)
	//	mBallNode->roll(Ogre::Radian(evt.timeSinceLastFrame * mPalet1RotationSpeed)); 

	// Si un track sur les deux sont déteté, on affiche la balle mais elle ne bouge pas
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

	// if both pads are visible,
	// C'est ici qu'on calcule le step à chaque frame et si y'a colision
	} else if ( nodeFound[0] && nodeFound[1] ) {
		//mBallNode2->setPosition(mPongPaletTrackNode1->getPosition());
		mBallNode->setVisible(true);
		positionBall = mBallNode->getPosition();
		//positionBall2 = mBallNode2->getPosition();

		// trajectoire of mBallNode
		Ogre::Vector3 delta = nextLocation(mBallNode);
		//Ogre::Vector3 delta2 = nextLocation(mBallNode2);
		//Ogre::LogManager::getSingletonPtr()->logMessage(" - (x: "+
		//	Ogre::StringConverter::toString(delta2.x));

		mBallNode->setPosition(positionBall.x+delta.x, positionBall.y+delta.y, positionBall.z+delta.z);
		//mBallNode2->setPosition(positionBall2.x+delta2.x, positionBall2.y+delta2.y, positionBall2.z+delta2.z);

		/*Ogre::LogManager::getSingletonPtr()->logMessage("looooog - (x: "+
			Ogre::StringConverter::toString(positionBall.x)+", y: "+
			Ogre::StringConverter::toString(positionBall.y)+", z: "+
			Ogre::StringConverter::toString(positionBall.z)+")");

		Ogre::LogManager::getSingletonPtr()->logMessage("Found[0] - "+Ogre::StringConverter::toString(nodeFound[0]));
		*/

		// Si y'a colision, on change de parent
		if( collisionOtherPad(mBallNode, idCurrentPad) ) {
			switchBaseNodeParent(idCurrentPad);
		}

		// Pas utilisé, prévu pour la suite si on met la balle indépendante des track
		/*if( collisionWithPad(mBallNode2) ) {
			direction = !direction;
		}*/

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

	// Si le parent de la balle est le track 1
	if( idCurBaseNode == 0 ) {
		toColide = mPongPaletTrackNode2;
		// Detecte la position des tag l'un par rapport à l'autre pour determiner le sens de déplacement
		if ( mPongPaletTrackNode1->getPosition().x > mPongPaletTrackNode2->getPosition().x) {
			leftToRight = false;
		} else {
			leftToRight = true;
		}
	} else {
		toColide = mPongPaletTrackNode1;
		if ( mPongPaletTrackNode1->getPosition().x < mPongPaletTrackNode2->getPosition().x ) {
			leftToRight = false;
		} else {
			leftToRight = true;
		}
	}

	//Ogre::LogManager::getSingletonPtr()->logMessage("Direction: "+Ogre::StringConverter::toString(leftToRight));

	//ALGO
	Ogre::Vector3 padPos = toColide->getPosition();
	Ogre::LogManager::getSingletonPtr()->logMessage("BallPos.x: "+Ogre::StringConverter::toString(ballPos.x)+
													", padPos.x: "+Ogre::StringConverter::toString(padPos.x)+" --> Direction: "+Ogre::StringConverter::toString(leftToRight));

	// Test la collision
	if ( leftToRight ) {
		if ( ballPos.x+30.0f >= padPos.x) {
			upToDown = 0;
			if( ballPos.z <= padPos.z+45.0f && ballPos.z > padPos.z+35.0f ){
				upToDown = 1;
			} else if( ballPos.z >= padPos.z+30.0f && ballPos.z < padPos.z+35.0f ){
				upToDown = -1;
			} else {
				upToDown = 0;
			}
			return true;
		}

	} else {
		if ( ballPos.x-30.0f <= padPos.x) {
			upToDown = 0;
			if( ballPos.z <= padPos.z+20.0f && ballPos.z > padPos.z+15.0f ){
				upToDown = 1;
			} else if( ballPos.z >= padPos.z-20.0f && ballPos.z < padPos.z+15.0f ){
				upToDown = -1;
			} else {
				upToDown = 0;
			}
			return true;
		}
	}
	//

	return false;
}

// Pas utilisé, prévu pour la suite si on met la balle indépendante des track
bool Rahpong::collisionWithPad(Ogre::SceneNode *node){
	Ogre::SceneNode *toColide = NULL;
	Ogre::Vector3 posTrack1 = mPongPaletTrackNode1->getPosition();
	Ogre::Vector3 posTrack2 = mPongPaletTrackNode2->getPosition();

	Ogre::Vector3 ballPos = node->getParent()->getPosition()+node->getPosition();
	

	if( posTrack1.x < posTrack2.x ) {
		if( direction ) {
			toColide = mPongPaletTrackNode2;
		} else {
			toColide = mPongPaletTrackNode1;
		}
	} else {
		if( direction ) {
			toColide = mPongPaletTrackNode1;
		} else {
			toColide = mPongPaletTrackNode2;
		}
	}

	Ogre::Vector3 padPos = toColide->getPosition();

	if ( leftToRight ) {
		if ( ballPos.x+20.0f >= padPos.x/*+D_Pad_BaseNode*/ /*&& ballPos.z <= padPos.z+toColide->getScale().z/2 && ballPos.z >= padPos.z-toColide->getScale().z/2*/) {
			if( ballPos.z <= padPos.z+20.0f && ballPos.z > padPos.z ){
				upToDown = 1;
			} else if( ballPos.z >= padPos.z-20.0f && ballPos.z < padPos.z ){
				upToDown = -1;
			} else {
				upToDown = 0;
			}
			return true;
		}

	} else {
		if ( ballPos.x-20.0f <= padPos.x/*-D_Pad_BaseNode*/ /*&& ballPos.z <= padPos.z+toColide->getScale().z/2 && ballPos.z >= padPos.z-toColide->getScale().z/2*/ ) {
			if( ballPos.z <= padPos.z+20.0f && ballPos.z > padPos.z ){
				upToDown = 1;
			} else if( ballPos.z >= padPos.z-20.0f && ballPos.z < padPos.z ){
				upToDown = -1;
			} else {
				upToDown = 0;
			}
			return true;
		}
	}

	return false;
}

// switch parent of ballNode with other pad
void Rahpong::switchBaseNodeParent( const int idCurBaseNode ){
	mBallNode->getParent()->removeChild(mBallNode);
	
	// On change le parent
	if( idCurBaseNode == 0 ) {
		mPongPaletTrackNode2->addChild(mBallNode);
		idCurrentPad = 1;
	} else {
		mPongPaletTrackNode1->addChild(mBallNode);
		idCurrentPad = 0;
	}
	// On place la balle un peu sur le coté (mais semble pas fonctionné )
	if( leftToRight ) {
		mBallNode->setPosition(-40.0f, 0.0f, 0.0f);
	} else {
		mBallNode->setPosition(40.0f, 0.0f, 0.0f);
	}
	
	Ogre::LogManager::getSingletonPtr()->logMessage("Switch parent to "+Ogre::StringConverter::toString(idCurrentPad));
}

// return a vector which represent the next step to reach other pad
Ogre::Vector3 Rahpong::nextLocation(Ogre::SceneNode *node){
	Ogre::Vector3 traj = Ogre::Vector3();

	Ogre::Real x = 0.0f;
	Ogre::Real z = 0.0f;

	//Ogre::Vector3 traj2 = node->getPosition()->getRotationTo(mPongPaletTrackNode1->getPosition());

	//tmp
	x = ballSpeedX;

	if( !leftToRight ) x = -1*x;
	if( upToDown == 0) {
		z = 0;
	} else if( upToDown == -1 ){
		z = -ballSpeedZ;
	} else if( upToDown == 1 ){
		z = ballSpeedZ;
	}
	

	traj.x = x;
	traj.y = 0.0f;
	traj.z = z;

	return traj;
}
