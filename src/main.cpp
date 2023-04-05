#include "SDLApp.h"
#include "TexturedRectangle.h"
#include "GameObject.h"
#include "Bird.h"
#include "Pipe.h"
#include "Ground.h"
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
Ground* ground;
vector<Pipe*> pipe(TOTAL_PIPE + 1);

bool startGame = false; //check if the game is started or not

bool resetGame = true;

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

		if (event.key.keysym.sym == SDLK_s) {
			if (!resetGame) {
				resetGame = true;
				flappyBird->SetDefaultStatus(SCREEN_WIDTH / 5, 5 * SCREEN_HEIGHT / 12, 0, 0, 0);
				for (int i = 0; i < TOTAL_PIPE + 1; i++) {
					pipe[i]->SetStatus(SCREEN_WIDTH + i * SCREEN_WIDTH / TOTAL_PIPE, MAX_PIPE_HEIGHT, MIN_PIPE_HEIGHT, PIPE_DISTANCE);
					pipe[i]->SetMode();
				}
			}
		}
		
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
			if (!startGame && resetGame) {
				startGame = true;
				resetGame = false;
				
			}
			if (!holdKey && startGame) {
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

	static int scrollingGround = 0;
	const int speed = 4;

	if (resetGame) {
		srand((unsigned int)time(NULL));
		++frame;
		if (frame / speed >= NUMBER_OF_FRAME) {
			frame = 0;
		}
		scrollingGround -= SPEED_SCREEN;
		if (scrollingGround < -SCREEN_WIDTH) scrollingGround = 0;
	}
	else {
		if (startGame) {
			if (flappyBird->GetY() < 5 * SCREEN_HEIGHT / 6 - 52) {
				flappyBird->FreeFall(0.5, 0.1);
			}
			else {
				flappyBird->StopOnGround(5 * SCREEN_HEIGHT / 6 - 52);
				startGame = false;
			}
			++frame;
			if (frame / speed >= NUMBER_OF_FRAME && !repeatFly) {
				frame = speed;
				repeatFly = true;
			}
			else if (repeatFly) {
				frame = speed;
			}

			//flappyBird->Render();

			for (int i = 0; i < TOTAL_PIPE + 1; i++) {
				pipe[i]->MoveHorizontal(SPEED_SCREEN);
				pipe[i]->Render();

				//If pipe moves out of screen, reset normal status and pipe mode
				if (pipe[i]->GetPipeX() < -(pipe[i]->GetPipeWidth())) {
					pipe[i]->SetStatus(SCREEN_WIDTH + SCREEN_WIDTH / TOTAL_PIPE - pipe[i]->GetPipeWidth(), MAX_PIPE_HEIGHT, MIN_PIPE_HEIGHT, PIPE_DISTANCE);
					pipe[i]->SetMode();
				}
				if (pipe[i]->IsMoving()) {
					if (pipe[i]->GetPipeY() < MIN_PIPE_HEIGHT) pipe[i]->SetMoveDown(true);
					else if (pipe[i]->GetPipeY() > MAX_PIPE_HEIGHT) pipe[i]->SetMoveDown(false);
					pipe[i]->MoveVertical(SPEED_SCREEN - 2, pipe[i]->IsMovingDown());
				}

				if (flappyBird->IsColliding(pipe[i]->GetPipeUp()) == SDL_TRUE || flappyBird->IsColliding(pipe[i]->GetPipeDown()) == SDL_TRUE) {
					startGame = false;
				}

			}

			scrollingGround -= SPEED_SCREEN;
			if (scrollingGround < -SCREEN_WIDTH) scrollingGround = 0;
		}
		else {
			if (flappyBird->GetY() < 5 * SCREEN_HEIGHT / 6 - 52) {
				flappyBird->FreeFall(0.5, 0.1);
			}
			else {
				flappyBird->StopOnGround(5 * SCREEN_HEIGHT / 6 - 52);
			}
			for (int i = 0; i < TOTAL_PIPE + 1; i++) {
				pipe[i]->Render();
			}

		}
	}
	
	flappyBird->DrawFrame(frame, speed);
	flappyBird->Render();

	ground->GetTexturedRectangle().SetPosition(scrollingGround, 5 * SCREEN_HEIGHT / 6);
	ground->GetTexturedRectangle().SetDimension(SCREEN_WIDTH, SCREEN_HEIGHT / 6);
	ground->Render();

	ground->GetTexturedRectangle().SetPosition(scrollingGround + SCREEN_WIDTH, 5 * SCREEN_HEIGHT / 6);
	ground->GetTexturedRectangle().SetDimension(SCREEN_WIDTH, SCREEN_HEIGHT / 6);
	ground->Render();




}

int main(int argc, char* args[]) {

	srand((unsigned int)time(NULL));

	//Setup the SDLApp
	const char* title = "Flappy Bird";
	app = new SDLApp(IMG_INIT_PNG, title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT);

	//Create objects and set up default properties
	background = new GameObject(app->GetRenderer(), "asset/image/background.png");
	flappyBird = new Bird(app->GetRenderer(), "asset/image/flappy-bird.png");
	ground = new Ground(app->GetRenderer(), "asset/image/base.png");
	

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

	

	

	//Handle events and rendering
	app->SetEventCallback(HandleEvents);
	app->SetRenderCallback(HandleRendering);

	//Game loop
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