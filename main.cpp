#include "header.h"
#include "Rahpong.h"

#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

extern "C" 
{

	INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
	#else

	int main(int argc, char **argv)
	#endif

	{
		Rahpong app;

		try 
		{
			app.go();
		} 
		catch( Ogre::Exception &e ) 
		{
			MessageBoxA( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		}
	
		return 0;
	}
}