#include "header.h"
#include "Rahpong.h"

Rahpong::Rahpong(void) :
	mBallNode(NULL), mBallEntity(NULL),
	mPaletEntity1(NULL), mPaletNode1(NULL), 
	mPaletEntity2(NULL), mPaletNode2(NULL),
	mPongBaseNode(NULL),
	mPongPaletTrackNode1(NULL), mPongPaletTrackNode2(NULL),
	ballSpeedX(0.15f), ballSpeedZ(0.03f), positionBall(0.0f, 20.0f, 0.0f),
	padToColide(NULL)
{
	nodeFound = (bool*)calloc(2, sizeof(bool));
	nodeFound[0] = false;
	nodeFound[1] = false;
	upToDown = 0;
	direction = true;
	heightPad = 40.0f;
	firstAllFound = false;
}

void Rahpong::createScene(void) {

	mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0f, 1.0f, 1.0f));

	mBallEntity = mSceneMgr->createEntity("Ball", "sphere.mesh");
	mBallNode = mSceneMgr->createSceneNode("BallNode2");
	mBallNode->setScale(0.02f, 0.02f, 0.02f);
    mBallNode->attachObject(mBallEntity);
	mBallNode->pitch(Ogre::Radian(-0.77f));

	// Create the pongtabs scenes nodes and entities
	mPaletEntity1 = mSceneMgr->createEntity("Palet1", "cube.mesh");
	mPaletNode1 = mSceneMgr->createSceneNode("PaletNode1");
	//mPaletNode1->setScale(10.0f, 10.0f, 5.0f);
	mPaletNode1->setScale(10.0f, 10.0f, 90.0f);
    mPaletNode1->attachObject(mPaletEntity1);

	mPongPaletTrackNode1 = mSceneMgr->createSceneNode("PaletTrackNode1");
	mPongPaletTrackNode1->addChild(mPaletNode1);
	mPaletNode1->setPosition(0.0f, 45.0f, 0.0f);
	//mBallNode->setPosition(-50.0f, 45.0f, 0.0f);

	mPaletEntity2 = mSceneMgr->createEntity("Palet2", "cube.mesh");
	mPaletNode2 = mSceneMgr->createSceneNode("PaletNode2");
	mPaletNode2->setScale(10.0f, 10.0f, 90.0f);
    mPaletNode2->attachObject(mPaletEntity2);

	mPongPaletTrackNode2 = mSceneMgr->createSceneNode("PaletTrackNode2");
	mPongPaletTrackNode2->addChild(mPaletNode2);
	mPaletNode2->setPosition(0.0f, 45.0f, 0.0f);

	mPongBaseNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("PongBaseNode");
	mPongBaseNode->addChild(mPongPaletTrackNode1);
	mPongBaseNode->addChild(mPongPaletTrackNode2);
	mPongBaseNode->addChild(mBallNode);

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

	// Si un track sur les deux sont déteté, on affiche la balle mais elle ne bouge pas
	if( nodeFound[0] && !nodeFound[1] ) {
		firstAllFound = true;
		mBallNode->setVisible(true);
		mBallNode->setPosition(mPongPaletTrackNode1->getPosition());
		padToColide = mPongPaletTrackNode2;


	} else if ( nodeFound[1] && !nodeFound[0] ) {
		firstAllFound = true;
		mBallNode->setVisible(true);
		Ogre::Vector3 pos = mPongPaletTrackNode2->getPosition();
		pos.x += 30.0f;
		mBallNode->setPosition(pos);
		padToColide = mPongPaletTrackNode1;


	// if both pads are visible,
	// C'est ici qu'on calcule le step à chaque frame et si y'a colision
	} else if ( nodeFound[0] && nodeFound[1] ) {
		
		if(firstAllFound){
			direction = checkDirection();
		}
		firstAllFound = false;

		mBallNode->setVisible(true);
		positionBall = mBallNode->getPosition();

		//direction = checkDirection();

		// trajectoir of mBallNode
		Ogre::Vector3 delta = nextLocation(mBallNode);

		mBallNode->setPosition(positionBall.x+delta.x, positionBall.y+delta.y, positionBall.z+delta.z);

		if( collisionWithPad(mBallNode) ) {
			direction = !direction;
		}
		
	} else if ( !nodeFound[0] && !nodeFound[1] ){
		firstAllFound = true;
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

// Check la position des tags et determine la direction
bool Rahpong::checkDirection(){
	return mPongPaletTrackNode1->getPosition().x < mPongPaletTrackNode2->getPosition().x;
}

// Check colision with padToColide on axe x and z
bool Rahpong::collisionWithPad(Ogre::SceneNode *node){

	Ogre::Vector3 ballPos = node->getPosition();
	

	if( mPongPaletTrackNode1->getPosition().x < mPongPaletTrackNode2->getPosition().x ) {
		if( direction ) {
			padToColide = mPongPaletTrackNode2;
		} else {
			padToColide = mPongPaletTrackNode1;
		}
	} else {
		if( direction ) {
			padToColide = mPongPaletTrackNode1;
		} else {
			padToColide = mPongPaletTrackNode2;
		}
	}

	Ogre::Vector3 padPos = padToColide->getPosition();
	/*Ogre::LogManager::getSingletonPtr()->logMessage("\n-----------x: "+Ogre::StringConverter::toString(padPos.x)+"\n"+
														"-----------y: "+Ogre::StringConverter::toString(padPos.y)+"\n"+
														"-----------z: "+Ogre::StringConverter::toString(padPos.z)+"\n");*/
	/*Ogre::LogManager::getSingletonPtr()->logMessage("-----------\nBallPos: x: "+Ogre::StringConverter::toString(ballPos.x)+" - y: "+Ogre::StringConverter::toString(ballPos.y)+" - z: "+Ogre::StringConverter::toString(ballPos.z)+"\n"+
											"padPos:: x: "+Ogre::StringConverter::toString(padPos.x)+" - y:"+Ogre::StringConverter::toString(padPos.y)+" - z:"+Ogre::StringConverter::toString(padPos.z));*/
	if ( direction ) {
		if ( ballPos.x+20.0f >= padPos.x && ballPos.y >= padPos.y-heightPad && ballPos.y <= padPos.y+heightPad ) {
			if( ballPos.y > padPos.y+heightPad/2 && ballPos.y <= padPos.y+heightPad ){
				upToDown = 1;
			} else if( ballPos.y >= padPos.y-heightPad && ballPos.y < padPos.y-heightPad/2 ){
				upToDown = -1;
			} else {
				upToDown = 0;
			}
			Ogre::LogManager::getSingletonPtr()->logMessage("-----------\nBallPos: x: "+Ogre::StringConverter::toString(ballPos.x)+" - y: "+Ogre::StringConverter::toString(ballPos.y)+" - z: "+Ogre::StringConverter::toString(ballPos.z)+"\n"+
													"padPos:: x: "+Ogre::StringConverter::toString(padPos.x)+" - y:"+Ogre::StringConverter::toString(padPos.y)+" - z:"+Ogre::StringConverter::toString(padPos.z));
			return true;
		}

	} else {
		if ( ballPos.x-20.0f <= padPos.x && ballPos.y >= padPos.y-heightPad && ballPos.y <= padPos.y+heightPad )  {
			if( ballPos.y > padPos.y+heightPad/2 && ballPos.y <= padPos.y+heightPad ){
				upToDown = 1;
			} else if( ballPos.y >= padPos.y-heightPad && ballPos.y < padPos.y+heightPad/2 ){
				upToDown = -1;
			} else {
				upToDown = 0;
			}
			Ogre::LogManager::getSingletonPtr()->logMessage("-----------\nBallPos: x: "+Ogre::StringConverter::toString(ballPos.x)+" - y: "+Ogre::StringConverter::toString(ballPos.y)+" - z: "+Ogre::StringConverter::toString(ballPos.z)+"\n"+
													"padPos:: x: "+Ogre::StringConverter::toString(padPos.x)+" - y:"+Ogre::StringConverter::toString(padPos.y)+" - z:"+Ogre::StringConverter::toString(padPos.z));
			return true;
		}
	}

	return false;
}

// return a vector which represent the next step to reach other pad
Ogre::Vector3 Rahpong::nextLocation(Ogre::SceneNode *node){
	Ogre::Vector3 traj = Ogre::Vector3();

	Ogre::Real x = 0.0f;
	Ogre::Real y = 0.0f;

	//tmp
	x = ballSpeedX;

	if( !direction ) x = -1*x;
	if( upToDown == 0) {
		y = 0;
	} else if( upToDown == -1 ){
		y = -ballSpeedZ;
	} else if( upToDown == 1 ){
		y = ballSpeedZ;
	}
	

	traj.x = x;
	traj.y = y;
	traj.z = 0.0f;

	return traj;
}
