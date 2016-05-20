#ifndef ApplicationManager_h__
#define ApplicationManager_h__
#include <Windows.h>
#include <glew.h>
#include <glfw3.h>
#include <memory>
#include "Renderer/Renderer.h"

class ApplicationManager
{

	int mOpenGLMajorVersion;
	int mOpenGLMinorVersion;
	double mTime;

	GLFWwindow* mWindow; //Why didn't we use smart pointers here? google it! ;)
	std::unique_ptr<Renderer> mRenderer;

	void InitializeComponents();
	void HandleKeyboardInput();
	void HandleMouseLocation();
	void Update();
	static void SpecialKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MouseMoved(GLFWwindow* window, double xpos, double ypos);
	static void WindowResized(GLFWwindow* window, int width, int height);

	static int KeyPressed; //Keep the code of the pressed key
	static double MouseXPos; //Keep the x-value of the mouse position
	static double MouseYPos; //Keep the y-value of the mouse position
	static int WindowSizeWidth; 
	static int WindowSizeHeight;

public:



	ApplicationManager(int pOpenGLMajorVersion, int pOpenGLMinorVersion);

	bool InitalizeApplication(int pWindowSizeWidth, int pWindowSizeHeight);
	void StartMainLoop();
	void CloseApplication();

	~ApplicationManager(void);
};
#endif // ApplicationManager_h__

