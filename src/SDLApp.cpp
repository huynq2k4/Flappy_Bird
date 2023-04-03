#include "SDLApp.h"

//Constructor
SDLApp::SDLApp(int imgFlag, const char* title, int x, int y, int w, int h)
{
	//Initialize SDL, print error if having any error while initializing SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0 || ((IMG_Init(imgFlag) & imgFlag) != imgFlag)) {
		cout << "SDL could not be initialize! Error: " << SDL_GetError() << endl;
	}

	//Initialize window and renderer
	mWindow = SDL_CreateWindow(title, x, y, w, h, SDL_WINDOW_SHOWN);
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
	//Set initial mouse coordinate
	mMouseX = 0; mMouseY = 0;
}

//Destructor
SDLApp::~SDLApp()
{
	SDL_DestroyWindow(mWindow);
	mWindow = nullptr;

	IMG_Quit();
	SDL_Quit();
}

//Get the renderer
SDL_Renderer* SDLApp::GetRenderer() const
{
	return mRenderer;
}

//Get mouse X-coordinate
int SDLApp::GetMouseX()
{
	return mMouseX;
}

//Get mouse Y-coordinate
int SDLApp::GetMouseY()
{
	return mMouseY;
}

//Handle events
void SDLApp::SetEventCallback(function<void(void)> func)
{
	mEventCallback = func;
}

//Handle rendering
void SDLApp::SetRenderCallback(function<void(void)> func)
{
	mRenderCallback = func;
}

//Run app loop
void SDLApp::RunAppLoop()
{
	while (!mQuit) {
		Uint32 start = SDL_GetTicks();


		//Get the current coordinate of mouse 
		Uint32 button = SDL_GetMouseState(&mMouseX, &mMouseY);

		//Do event specified by user
		mEventCallback();

		//Clear the screen before drawing
		SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(mRenderer);

		//Set color to draw
		SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

		//Render to screen specified by user
		mRenderCallback();

		//Show what we have drawn on the screen
		SDL_RenderPresent(mRenderer);

		Uint32 finish = SDL_GetTicks();
		//Set a frame cap
		if (finish - start < 16) {
			SDL_Delay(16 - (finish - start));
		}
		//SDL_Delay(30);

	}
}

//End app loop
void SDLApp::EndAppLoop()
{
	mQuit = true;
}
