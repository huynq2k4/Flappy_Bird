#include "SDLApp.h"
#include "TexturedRectangle.h"
#include "GameObject.h"
#include "Bird.h"
#include "Pipe.h"
#include <vector>

const int SCREEN_HEIGHT = 720;
const int SCREEN_WIDTH = 1280;
const int DEFAULT_SPEED_FLY = -8;
const double DEFAULT_ANGLE = -30;
const int NUMBER_OF_FRAME = 4;
const int SPRITE_HEIGHT = 610;
const int SPRITE_WIDTH = 2868;
const int TOTAL_PIPE = 4;
const int SPEED_SCREEN = 3;
const int MAX_PIPE_HEIGHT = 550;
const int MIN_PIPE_HEIGHT = 250;
const int PIPE_DISTANCE = 175;


SDLApp* app;

GameObject* background;
Bird* flappyBird;
GameObject* ground;
vector<Pipe*> pipe(TOTAL_PIPE + 1);

bool startGame = false; //check if the game is started or not

//Check if a key is holding or not
//to avoid continually holding a key so that the bird is flying high faster
bool holdKey = false;


//Make sure the bird flaps wing only when it flys (space keydown)
bool repeatFly = false;

int frame = 0;


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
				flappyBird->SetDefaultStatus(SCREEN_WIDTH / 5, 5 * SCREEN_HEIGHT / 12, 0, 0, 0);
			}


			if (!holdKey) {
				isFlying = true;
				flappyBird->Fly(DEFAULT_SPEED_FLY, DEFAULT_ANGLE);
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

	background->Render();

	
	if (startGame) {
		if(flappyBird->GetY() < 5 * SCREEN_HEIGHT / 6 - 52){
			flappyBird->FreeFall(0.5, 0.1);
		}
		else {
			flappyBird->StopOnGround(5 * SCREEN_HEIGHT / 6 - 52);
			startGame = false;
		}
	}

	const int speed = 4;

	++frame;
	if (frame / speed >= NUMBER_OF_FRAME && !repeatFly) {
		frame = speed;
		repeatFly = true;
	}
	else if (repeatFly) {
		frame = speed;
	}

	flappyBird->DrawFrame(frame, speed);
	flappyBird->Render();

	for (int i = 0; i < TOTAL_PIPE + 1; i++) {
		pipe[i]->MoveHorizontal(SPEED_SCREEN);
		pipe[i]->Render();
		if (pipe[i]->GetPipeX() < -(pipe[i]->GetPipeWidth())) {
			pipe[i]->SetStatus(SCREEN_WIDTH + SCREEN_WIDTH / TOTAL_PIPE - pipe[i]->GetPipeWidth(), MAX_PIPE_HEIGHT, MIN_PIPE_HEIGHT, PIPE_DISTANCE);
			pipe[i]->SetMode();
		}
		if (pipe[i]->IsMoving()) {
			if (pipe[i]->GetPipeY() < MIN_PIPE_HEIGHT) pipe[i]->SetMoveDown(true);
			else if (pipe[i]->GetPipeY() > MAX_PIPE_HEIGHT) pipe[i]->SetMoveDown(false);
			pipe[i]->MoveVertical(SPEED_SCREEN - 2, pipe[i]->IsMovingDown());
		}
		
	}

	static int scrollingGround = 0;
	ground->GetTexturedRectangle().SetPosition(scrollingGround, 5 * SCREEN_HEIGHT / 6);
	ground->GetTexturedRectangle().SetDimension(SCREEN_WIDTH, SCREEN_HEIGHT / 6);
	ground->Render();

	ground->GetTexturedRectangle().SetPosition(scrollingGround + SCREEN_WIDTH, 5 * SCREEN_HEIGHT / 6);
	ground->GetTexturedRectangle().SetDimension(SCREEN_WIDTH, SCREEN_HEIGHT / 6);
	ground->Render();

	scrollingGround -= SPEED_SCREEN;
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
	

	for (int i = 0; i < TOTAL_PIPE + 1; i++) {
		pipe[i] = new Pipe(app->GetRenderer(), "asset/image/pipe-green.png", 104, 640);
		pipe[i]->SetStatus(SCREEN_WIDTH + i * SCREEN_WIDTH / TOTAL_PIPE, MAX_PIPE_HEIGHT, MIN_PIPE_HEIGHT, PIPE_DISTANCE);
		pipe[i]->SetMode();
	}

	background->GetTexturedRectangle().SetPosition(0, 0);
	background->GetTexturedRectangle().SetDimension(SCREEN_WIDTH, SCREEN_HEIGHT);

	flappyBird->SetDefaultStatus(SCREEN_WIDTH / 5, 5 * SCREEN_HEIGHT / 12, 0, 0, 0);
	flappyBird->GetTexturedRectangle().SetDimension(72, 61);
	flappyBird->SetDefaultFrame(0, 0, SPRITE_WIDTH / NUMBER_OF_FRAME, SPRITE_HEIGHT);


	app->SetEventCallback(HandleEvents);
	app->SetRenderCallback(HandleRendering);

	app->RunAppLoop();

	//Clean up our app
	delete app;
	delete background;
	delete flappyBird;
	delete ground;
	for (int i = 0; i < TOTAL_PIPE + 1; i++) {
		delete pipe[i];
	}
	return 0;
}