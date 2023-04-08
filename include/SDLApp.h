#ifndef SDL_APP_H
#define SDL_APP_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>
#include <functional>

using namespace std;

class SDLApp {
private:

	//Pointer to window and renderer
	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer = nullptr;

	//Check if app is quited or not
	bool mQuit = false;

	//Callback function to handle events and rendering
	function<void(void)> mEventCallback;
	function<void(void)> mRenderCallback;

public:
	//Constructor
	SDLApp(int imgFlag, const char* title, int x, int y, int w, int h);

	//Destructor
	~SDLApp();
	
	//Get the renderer
	SDL_Renderer* GetRenderer() const;


	//Handle events and rendering
	void SetEventCallback(function<void(void)> func);
	void SetRenderCallback(function<void(void)> func);

	//Function for starting and ending app loop
	void RunAppLoop();
	void EndAppLoop();

};


#endif