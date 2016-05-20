#include "Application Manager/ApplicationManager.h"

int main(void)
{
	//Specify OpenGL version as 3.3
	int openGLMajorVersion = 3;
	int openGLMinorVersion = 3;

	//Unique object for the application manager
	std::unique_ptr<ApplicationManager> appManager (new ApplicationManager(openGLMajorVersion,openGLMinorVersion));
	
	// Initialize the window providing its width and height
	if(appManager->InitalizeApplication(1000,600) == true)
	{
		appManager->StartMainLoop();
		appManager->CloseApplication();
	}
}