#include "SDLApp.h"
#include "ResourceManager.h"
#include "TexturedRectangle.h"
#include "GameObject.h"
#include "Bird.h"

const int SCREEN_HEIGHT = 720;
const int SCREEN_WIDTH = 1280;
const int DEFAULT_SPEED_FLY = -8;
const double DEFAULT_ANGLE = -30;

SDLApp* app;

GameObject* background;
Bird* flappyBird;
GameObject* ground;
GameObject* pipe;



double posY = 5 * SCREEN_HEIGHT / 12;
double velFly = 0;
double angle = 0;
double velTurn = 0;

bool startGame = false; //check if the game is started or not

//Check if a key is holding or not
//to avoid continually holding a key so that the bird is flying high faster
bool holdKey = false;

int frame = 0;

//Make sure the bird flaps wing only when it flys (space keydown)
bool repeatFly = false;


void HandleEvents() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {

		if (event.type == SDL_QUIT) {
			app->EndAppLoop();
		}
		bool isFlying = false;

		
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
			if (!startGame) {

				startGame = true;
				posY = 5 * SCREEN_HEIGHT / 12;
				angle = 0;
				velFly = 0;
				velTurn = 0;

			}


			if (!holdKey) {
				isFlying = true;
				velFly = DEFAULT_SPEED_FLY;

				angle = DEFAULT_ANGLE;
				velTurn = 0;
				frame = 0;
				holdKey = true;
				repeatFly = false;

			}
		}
		if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE) {
			holdKey = false;
			
		}
		


		
	}
}

void HandleRendering() {
	background->GetTexturedRectangle().SetPosition(0, 0);
	background->GetTexturedRectangle().SetDimension(SCREEN_WIDTH, SCREEN_HEIGHT);
	background->Render();

	if (startGame) {
		if (posY < 5 * SCREEN_HEIGHT / 6 - 52) {
			posY += velFly + 0.5;
			velFly += 0.5;


			angle += velTurn + 0.1;
			velTurn += 0.1;

			if (angle >= 90) angle = 90;
		}
		else {
			posY = 5 * SCREEN_HEIGHT / 6 - 52;
			startGame = false;
		}
	}

	SDL_Rect clip;
	clip.x = 2868 / 4 * (int)(frame / 4);
	clip.y = 0;
	clip.w = 2868 / 4;
	clip.h = 610;

	++frame;
	if (frame / 4 > 3 && !repeatFly) {
		frame = 4;
		repeatFly = true;
	}
	else if (repeatFly) {
		frame = 4;
	}

	flappyBird->GetTexturedRectangle().SetPosition(SCREEN_WIDTH / 5, posY);
	flappyBird->GetTexturedRectangle().SetDimension(72, 61);
	flappyBird->GetTexturedRectangle().SetAngle(angle);
	flappyBird->GetTexturedRectangle().SetClip(&clip);
	flappyBird->Render();

	static int movingPipeX = SCREEN_WIDTH;
	static int movingPipeY = 5 * SCREEN_HEIGHT / 6 - 200;
	std::cout << movingPipeY << endl;
	pipe->GetTexturedRectangle().SetPosition(movingPipeX, movingPipeY);
	pipe->GetTexturedRectangle().SetDimension(104, 640);
	pipe->GetTexturedRectangle().SetFlip(SDL_FLIP_NONE);
	pipe->Render();

	pipe->GetTexturedRectangle().SetPosition(movingPipeX, movingPipeY - 800);
	pipe->GetTexturedRectangle().SetDimension(104, 640);
	pipe->GetTexturedRectangle().SetFlip(SDL_FLIP_VERTICAL);
	pipe->Render();


	movingPipeX -= 3;
	if (movingPipeX < -(pipe->GetTexturedRectangle().GetWidth())) {
		movingPipeX = SCREEN_WIDTH;
		movingPipeY = 5 * SCREEN_HEIGHT / 6 - 200;
	}

	static bool goDown = false;
	if (movingPipeY < 300) goDown = true;
	else if (movingPipeY > 5 * SCREEN_HEIGHT / 6 - 150) goDown = false;
	if (goDown) movingPipeY += 2;
	else movingPipeY -= 2;

	static int scrollingGround = 0;
	ground->GetTexturedRectangle().SetPosition(scrollingGround, 5 * SCREEN_HEIGHT / 6);
	ground->GetTexturedRectangle().SetDimension(SCREEN_WIDTH, SCREEN_HEIGHT / 6);
	ground->Render();

	ground->GetTexturedRectangle().SetPosition(scrollingGround + SCREEN_WIDTH, 5 * SCREEN_HEIGHT / 6);
	ground->GetTexturedRectangle().SetDimension(SCREEN_WIDTH, SCREEN_HEIGHT / 6);
	ground->Render();

	scrollingGround -= 3;
	if (scrollingGround < -SCREEN_WIDTH) scrollingGround = 0;


}

int main(int argc, char* args[]) {

	//Setup the SDLApp
	const char* title = "Flappy Bird";
	app = new SDLApp(IMG_INIT_PNG, title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT);

	//Create objects
	background = new GameObject(app->GetRenderer(), "asset/image/background.png");
	flappyBird = new Bird(app->GetRenderer(), "asset/image/flappy-bird.png");
	ground = new GameObject(app->GetRenderer(), "asset/image/base.png");
	pipe = new GameObject(app->GetRenderer(), "asset/image/pipe-green.png");

	app->SetEventCallback(HandleEvents);
	app->SetRenderCallback(HandleRendering);

	app->RunAppLoop();

	//Clean up our app
	delete app;
	delete background;
	delete flappyBird;
	delete ground;
	delete pipe;
	return 0;
}