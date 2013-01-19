#ifndef _RAHPONG_H_
#define _RAHPONG_H_

#include "BaseApplication.h"

class Rahpong : public BaseApplication {

	public:
		Rahpong(void);

	protected:
		virtual void createScene(void);

		Ogre::String		pathCfg;

		//Pongtab mesh and entity
		Ogre::SceneNode		*mPongTabNode;
		Ogre::Entity		*mPongTabEntity;

};

#endif
