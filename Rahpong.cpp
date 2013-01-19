#include "header.h"
#include "Rahpong.h"

Rahpong::Rahpong(void) :
	mPongTabNode(NULL),
	mPongTabEntity(NULL)
{
	pathCfg = "E:/Codage/Cours/Rahpong/data";
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
