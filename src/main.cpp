#include "SDLApp.h"
#include "TexturedRectangle.h"
#include "GameObject.h"
#include "Bird.h"
#include "Pipe.h"
#include "Ground.h"
#include "CheckCollision.h"
#include "Text.h"
#include "Sound.h"
#include <vector>
#include <fstream>

//Screen constant
const int SCREEN_HEIGHT = 720;
const int SCREEN_WIDTH = 1280;
const int SPEED_SCROLLING_SCREEN = 3;

//Bird constant
const int DEFAULT_SPEED_FLY = -8;
const double DEFAULT_ANGLE = -30;
const int NUMBER_OF_FRAME = 4;
const int BIRD_HEIGHT = 610;
const int BIRD_WIDTH = 2868;
const int SLOW_DOWN_RENDER_BIRD = 4;

//Pipe constant
const int TOTAL_PIPE = 4;
const int MAX_PIPE_Y = 550;
const int MIN_PIPE_Y = 250;
const int PIPE_DISTANCE = 175;
const int SPEED_MOVING_VERTICAL = 1;

//Score constant
const int SCORE_X = 10;
const int SCORE_Y = 640;
const int SCORE_WIDTH_CHAR = 15;
const int SCORE_HEIGHT_CHAR = 28;



SDLApp* app;

GameObject* background;
Bird* flappyBird;
Ground* ground;
vector<Pipe*> pipe(TOTAL_PIPE + 1);
GameObject* flash;
Text* score;
Text* highScore;
SoundEffect* soundFly;
SoundEffect* soundGetPoint;
SoundEffect* soundDie;
SoundEffect* soundHit;

bool startGame = false; //check if the game is started or not

bool resetGame = true;

//Check if a key is holding or not
//to avoid continually holding a key so that the bird is flying high faster
bool holdKey = false;


//Make sure the bird flaps wing only when it flys (space keydown)
bool repeatFly = false;

bool isFlash = false;
static int alpha = 255;
int point = 0;

static string hScore = "0";
ifstream fin("score.txt");

//ofstream fout("score.txt");

void HandleEvents() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {

		if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
			int sc; fin >> sc;
			if (sc < stoi(hScore)) {
				ofstream fout("score.txt", ios::trunc);
				fout << stoi(hScore);
			}
			app->EndAppLoop();
			
		}
		bool isFlying = false;

		if (event.key.keysym.sym == SDLK_s) {
			if (!resetGame && !startGame) {
				resetGame = true;
				cout << "Line 95: " << hScore << endl;
				flappyBird->SetDefaultStatus(SCREEN_WIDTH / 5, 5 * SCREEN_HEIGHT / 12, 0, 0, 0);
				cout << "Line 97: " << hScore << endl;
				for (int i = 0; i < TOTAL_PIPE + 1; i++) {
					pipe[i]->SetStatus(SCREEN_WIDTH + i * SCREEN_WIDTH / TOTAL_PIPE, MAX_PIPE_Y, MIN_PIPE_Y, PIPE_DISTANCE);
					pipe[i]->SetMode();
					cout << "Line 101: " << hScore << endl;
				}
				alpha = 255;
				isFlash = false;
				startGame = false;
				cout << "Line 106: " << hScore << endl;
				point = 0;
				score->SetSize(SCORE_X, SCORE_Y, SCORE_WIDTH_CHAR * 8, SCORE_HEIGHT_CHAR);
				score->ChangeText("Score: 0");
				cout << "Line 110: " << hScore << endl;
				highScore->SetSize(SCORE_X, SCORE_Y + SCORE_HEIGHT_CHAR + 2, SCORE_WIDTH_CHAR * (12 + hScore.length()), SCORE_HEIGHT_CHAR);
				highScore->ChangeText("High score: " + hScore);
				cout << "Line 113: " << hScore << endl;
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
				flappyBird->DrawFrame(0, SLOW_DOWN_RENDER_BIRD);
				flappyBird->ShiftColliders();
				soundFly->PlaySound(0);
				holdKey = true;
				repeatFly = false;

			}


		}
		if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE) {
			holdKey = false;
		}
		if (event.key.keysym.sym == SDLK_r) {
			if (!startGame) {
				ofstream fout("score.txt", ios::trunc);
				fout << "0";
				hScore = "0";
				cout << "Line 144: " << hScore << endl;
				highScore->ChangeText("High score: 0");
				highScore->SetSize(SCORE_X, SCORE_Y + SCORE_HEIGHT_CHAR + 2, SCORE_WIDTH_CHAR * 13, SCORE_HEIGHT_CHAR);
			}
		}

	}
}

void HandleRendering() {




	static int scrollingGround = 0;
	static int frame = 0;

	if (resetGame) {
		background->Render();

		srand((unsigned int)time(NULL));
		++frame;
		if (frame / SLOW_DOWN_RENDER_BIRD >= NUMBER_OF_FRAME) {
			frame = 0;
		}
		scrollingGround -= SPEED_SCROLLING_SCREEN;
		if (scrollingGround < -SCREEN_WIDTH) scrollingGround = 0;
	}
	else {
		if (startGame) {
			background->Render();
			if (flappyBird->GetY() < 5 * SCREEN_HEIGHT / 6 - 52) {
				flappyBird->FreeFall(0.5, 0.1);
			}
			else {
				flappyBird->StopOnGround(5 * SCREEN_HEIGHT / 6 - 52);
				startGame = false;
				soundHit->PlaySound(0);
				//ifstream fin("score.txt");
				//fin >> hScore;
				if (point > stoi(hScore)) {
					hScore = to_string(point);
					cout << hScore << endl;



					
				}
			}
			++frame;
			if (frame / SLOW_DOWN_RENDER_BIRD >= NUMBER_OF_FRAME && !repeatFly) {
				frame = SLOW_DOWN_RENDER_BIRD;
				repeatFly = true;
			}
			else if (repeatFly) {
				frame = SLOW_DOWN_RENDER_BIRD;
			}
			flappyBird->ShiftColliders();

			//flappyBird->Render();

			for (int i = 0; i < TOTAL_PIPE + 1; i++) {
				pipe[i]->MoveHorizontal(SPEED_SCROLLING_SCREEN);
				pipe[i]->Render();

				//If pipe moves out of screen, reset normal status and pipe mode
				if (pipe[i]->GetPipeX() < -(pipe[i]->GetPipeWidth())) {
					pipe[i]->SetStatus(SCREEN_WIDTH + SCREEN_WIDTH / TOTAL_PIPE - pipe[i]->GetPipeWidth(), MAX_PIPE_Y, MIN_PIPE_Y, PIPE_DISTANCE);
					pipe[i]->SetMode();
				}
				if (pipe[i]->IsMoving()) {
					if (pipe[i]->GetPipeY() < MIN_PIPE_Y) pipe[i]->SetMoveDown(true);
					else if (pipe[i]->GetPipeY() > MAX_PIPE_Y) pipe[i]->SetMoveDown(false);
					pipe[i]->MoveVertical(SPEED_MOVING_VERTICAL, pipe[i]->IsMovingDown());
				}
				if (pipe[i]->DetectScoring(flappyBird)) {
					++point;
					string s = "Score: " + to_string(point);
					score->SetSize(SCORE_X, SCORE_Y, SCORE_WIDTH_CHAR * s.length(), SCORE_HEIGHT_CHAR);
					score->ChangeText(s);
					soundGetPoint->PlaySound(0);

				}


				if (flappyBird->IsColliding(pipe[i]->GetPipeUp()) == SDL_TRUE || flappyBird->IsColliding(pipe[i]->GetPipeDown()) == SDL_TRUE) {

					startGame = false;
					soundHit->PlaySound(0);

					//fin >> hScore;
					if (point > stoi(hScore)) {
						hScore = to_string(point);
						cout << hScore << endl;
					}
					//soundDie->PlaySound(0);
				}

			}

			scrollingGround -= SPEED_SCROLLING_SCREEN;
			if (scrollingGround < -SCREEN_WIDTH) scrollingGround = 0;
		}
		else {




			background->Render();
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


	flappyBird->DrawFrame(frame, SLOW_DOWN_RENDER_BIRD);
	flappyBird->Render();

	ground->GetTexturedRectangle().SetPosition(scrollingGround, 5 * SCREEN_HEIGHT / 6);
	ground->GetTexturedRectangle().SetDimension(SCREEN_WIDTH, SCREEN_HEIGHT / 6);
	ground->Render();

	ground->GetTexturedRectangle().SetPosition(scrollingGround + SCREEN_WIDTH, 5 * SCREEN_HEIGHT / 6);
	ground->GetTexturedRectangle().SetDimension(SCREEN_WIDTH, SCREEN_HEIGHT / 6);
	ground->Render();


	static int mode = 13;
	if (!resetGame && !startGame && !isFlash) {
		flash->GetTexturedRectangle().SetAlpha(alpha);
		if (alpha >= 255) {
			mode = -13;

		}
		alpha += mode;
		flash->Render();

		if (alpha < 0) {
			isFlash = true;

		}
	}

	score->Render();
	highScore->Render();



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
		pipe[i]->SetStatus(SCREEN_WIDTH + i * SCREEN_WIDTH / TOTAL_PIPE, MAX_PIPE_Y, MIN_PIPE_Y, PIPE_DISTANCE);
		pipe[i]->SetMode();
	}

	background->GetTexturedRectangle().SetPosition(0, 0);
	background->GetTexturedRectangle().SetDimension(SCREEN_WIDTH, SCREEN_HEIGHT);

	flappyBird->SetDefaultStatus(SCREEN_WIDTH / 5, 5 * SCREEN_HEIGHT / 12, 0, 0, 0);
	flappyBird->GetTexturedRectangle().SetDimension(72, 61);
	flappyBird->SetDefaultFrame(0, 0, BIRD_WIDTH / NUMBER_OF_FRAME, BIRD_HEIGHT);

	vector<Point> birdCollisionBox;

	birdCollisionBox.push_back({ 714, 575 });
	birdCollisionBox.push_back({ 714, 382 });
	birdCollisionBox.push_back({ 655, 77 });
	birdCollisionBox.push_back({ 470, 0 });
	birdCollisionBox.push_back({ 319, 108 });
	birdCollisionBox.push_back({ 139, 312 });
	birdCollisionBox.push_back({ 139, 575 });

	flappyBird->CreateCollisionShape(birdCollisionBox);


	flash = new GameObject(app->GetRenderer(), "asset/image/white.png");
	flash->GetTexturedRectangle().SetPosition(-50, -50);
	flash->GetTexturedRectangle().SetDimension(SCREEN_WIDTH + 300, SCREEN_HEIGHT + 300);

	score = new Text(app->GetRenderer(), "asset/font/Flappy-Bird.ttf", "Score: 0", 112, { 255,0,0 });
	score->SetSize(SCORE_X, SCORE_Y, SCORE_WIDTH_CHAR * 8, SCORE_HEIGHT_CHAR);

	
	fin >> hScore;
	highScore = new Text(app->GetRenderer(), "asset/font/Flappy-Bird.ttf", "High score: " + hScore, 112, { 255,0,0 });
	highScore->SetSize(SCORE_X, SCORE_Y + SCORE_HEIGHT_CHAR + 2, SCORE_WIDTH_CHAR * (12 + hScore.length()), SCORE_HEIGHT_CHAR);

	soundFly = new SoundEffect("asset/sound/fly.wav");
	soundGetPoint = new SoundEffect("asset/sound/get-point.wav");
	soundDie = new SoundEffect("asset/sound/die.wav");
	soundHit = new SoundEffect("asset/sound/hit.wav");

	//Handle events and rendering
	app->SetEventCallback(HandleEvents);
	app->SetRenderCallback(HandleRendering);

	//Game loop
	app->RunAppLoop();

	//Clean up our app
	delete background;
	delete flappyBird;
	delete ground;
	for (int i = 0; i < TOTAL_PIPE + 1; i++) {
		delete pipe[i];
	}
	delete flash;
	delete score;
	delete soundDie;
	delete soundFly;
	delete soundGetPoint;
	delete soundHit;
	delete app;

	return 0;
}


