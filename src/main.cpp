#include "SDLApp.h"
#include "TexturedRectangle.h"
#include "GameObject.h"
#include "Bird.h"
#include "Pipe.h"
#include "Ground.h"
#include "CheckCollision.h"
#include "Text.h"
#include "Sound.h"
#include "Button.h"
#include <vector>
#include <fstream>

//Screen constant
const int SCREEN_HEIGHT = 720;
const int SCREEN_WIDTH = 1280;
const int SPEED_SCROLLING_SCREEN = 3;

//Bird constant
const int DEFAULT_SPEED_FLY = -8;
const double DEFAULT_ANGLE = -30;
const int SLOW_DOWN_RENDER_BIRD = 4;
const int BIRD_HEIGHT = 610;
const int BIRD_WIDTH = 2868;
const int DEFAULT_BIRD_X = 256;

//Pipe constant
const int TOTAL_PIPE = 5;
const int MAX_PIPE_Y = 550;
const int MIN_PIPE_Y = 250;
const int PIPE_DISTANCE = 175;
const int SPEED_MOVING_VERTICAL = 1;

//Score constant
const int SCORE_X = 10;
const int SCORE_Y = 640;
const int SCORE_WIDTH_CHAR = 15;
const int SCORE_HEIGHT_CHAR = 28;

//Ground constant
const int GROUND_Y = 600;

enum CharacterY {
	RED_Y = 0,
	PINK_STRIPE_Y = 520,
	PINK_BALD_Y = 1061,
	BROWN_Y = 1647,
	BLUE_Y = 2250,
	YELLOW_Y = 2860
};

enum CharacterWidth {
	RED_W = 712,
	PINK_STRIPE_W = 713,
	PINK_BALD_W = 667,
	BROWN_W = 760,
	BLUE_W = 717,
	YELLOW_W = 716
};

//Store character Y and width
const vector<pair<int, int>> CharacterProperties = { {0, 712}, {520, 713}, {1061, 667}, {1647, 760}, {2250, 717}, {2860, 716}, {3492, 0} };

//Declare main game
SDLApp* app;

//Objects in gameplay
GameObject* background;
Bird* flappyBird;
Ground* ground;
vector<Pipe*> pipe(TOTAL_PIPE);
GameObject* flash;
Text* score;
Text* highScore;
SoundEffect* soundFly;
SoundEffect* soundGetPoint;
SoundEffect* soundDie;
SoundEffect* soundHit;

//Objects in main menu
TexturedRectangle* logo;
Button* startGame;
Button* nextLeft;
Button* nextRight;
Bird* character;
GameObject* blackTransition;
Music* bgMusic;

bool isMainMenu = true;

bool isStartingGame = false; //Signal to start game

bool isResetingGame = true; //Signal to reset game

//Check if a key is holding or not
//to avoid continuously holding a key so that the bird is flying high faster
bool isHoldingKey = false;

bool isPlayingMusic = false;

bool repeatFly = false; //Make sure the bird flaps wing only when it flys (space keydown)

//Global variables to handle screen flickering
bool isFlash = false; //Signal to flicker the screen (white flash) when the bird collides
int alphaFlash = 255; //Adjust the transparency of white screen when flashing

int point = 0; //Store point on gameplay

string hScore = "0"; //Temporarily store high score on gameplay
ifstream fin("bin/score.bin", ios::binary | ios::in); //Open binary file storing high score

int numCharacter = 0;
int numberOfFrame = 4;
static int frame = 0; //Store frame number to render bird flying



void HandleEvents() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {

		if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {

			//Store high score before closing game
			int sc; 
			fin.read((char*)&sc, sizeof(sc));
			if (sc < stoi(hScore)) {
				int tmphScore = stoi(hScore);
				ofstream fout("bin/score.bin", ios::binary | ios::out | ios::trunc);
				fout.write((char*)&tmphScore, sizeof(int));
			}

			app->EndAppLoop();
			
		}

		bool isFlying = false; //Check if the bird is flying or not

		//Game reseting process when pressing S key
		if (event.key.keysym.sym == SDLK_s) {

			if (!isResetingGame && !isStartingGame) {

				isResetingGame = true;

				flappyBird->SetDefaultStatus(DEFAULT_BIRD_X, GROUND_Y / 2, 0, 0, 0);

				for (int i = 0; i < TOTAL_PIPE; i++) {
					pipe[i]->SetStatus(SCREEN_WIDTH + i * SCREEN_WIDTH / (TOTAL_PIPE - 1), MAX_PIPE_Y, MIN_PIPE_Y, PIPE_DISTANCE);
					pipe[i]->SetMode();

				}

				alphaFlash = 255;
				isFlash = false;
				
				point = 0;
				score->SetSize(SCORE_X, SCORE_Y, SCORE_WIDTH_CHAR * 8, SCORE_HEIGHT_CHAR);
				score->ChangeText("Score: 0");
	
				highScore->SetSize(SCORE_X, SCORE_Y + SCORE_HEIGHT_CHAR + 2, SCORE_WIDTH_CHAR * (12 + hScore.length()), SCORE_HEIGHT_CHAR);
				highScore->ChangeText("High score: " + hScore);

			}
		}

		//Handle events when pressing Spacebar
		if (event.key.keysym.sym == SDLK_SPACE) {
			if (event.type == SDL_KEYDOWN) {

				//Handle starting game
				if (!isStartingGame && isResetingGame) {
					isStartingGame = true;
					isResetingGame = false;
				}

				//Handle bird flying
				if (!isHoldingKey && isStartingGame && !isMainMenu) {
					isFlying = true;
					flappyBird->Fly(DEFAULT_SPEED_FLY, DEFAULT_ANGLE);
					flappyBird->DrawFrame(0, SLOW_DOWN_RENDER_BIRD);
					flappyBird->ShiftColliders();
					soundFly->PlaySound(0);
					frame = 0;
					isHoldingKey = true;
					repeatFly = false;

				}
			}
			else {
				isHoldingKey = false; //Prevent holding Spacebar to help the bird continuously fly high
			}

		}

		//Reset high score when pressing R key
		if (event.key.keysym.sym == SDLK_r) {
			if (!isStartingGame) {
				ofstream fout("bin/score.bin", ios::trunc | ios::binary | ios::out);
				hScore = "0";
				fout.write((char*)&hScore, sizeof(int));
				highScore->ChangeText("High score: 0");
				highScore->SetSize(SCORE_X, SCORE_Y + SCORE_HEIGHT_CHAR + 2, SCORE_WIDTH_CHAR * 13, SCORE_HEIGHT_CHAR);
			}
		}
		if (event.type == SDL_MOUSEMOTION) {
			startGame->HandleMouseOver();
			//nextLeft->HandleMouseOver();
			//nextRight->HandleMouseOver();
		}
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			startGame->HandleMouseDown();
			nextLeft->HandleMouseDown();
			nextRight->HandleMouseDown();
		}
		if (event.type == SDLK_LEFT) {
			nextLeft->HandleMouseDown();
		}
		if (event.type == SDLK_RIGHT) {
			nextRight->HandleMouseDown();
		}
		if (nextLeft->IsClicking() || (event.key.keysym.sym == SDLK_LEFT && event.type == SDL_KEYDOWN)) {
			if (numCharacter == 0) numCharacter = 5;
			else --numCharacter;
		}
		if (nextRight->IsClicking() || (event.key.keysym.sym == SDLK_RIGHT && event.type == SDL_KEYDOWN)) {
			if (numCharacter == 5) numCharacter = 0;
			else ++numCharacter;
		}
		if (startGame->IsClicking()) {
			isMainMenu = false;
			isResetingGame = true;
		}

	}
}

void HandleRendering() {

	static int scrollingGround = 0;

	static int mode = 13;//Adjust the white flash appearing speed

	

	if (isMainMenu) {
		background->Render();

		if (!isPlayingMusic) {
			bgMusic->PlaySound(-1);
			isPlayingMusic = true;
		}
		character->SetDefaultFrame(0, CharacterProperties[numCharacter].first, CharacterProperties[numCharacter].second, CharacterProperties[numCharacter + 1].first - CharacterProperties[numCharacter].first);
		character->DrawFrame(0, 4);

		logo->Render(app->GetRenderer());
		nextLeft->Render();
		nextRight->Render();
		character->Render();
		startGame->Render();

		scrollingGround -= SPEED_SCROLLING_SCREEN;
		if (scrollingGround < -SCREEN_WIDTH) scrollingGround = 0;
		ground->GetTexturedRectangle().SetPosition(scrollingGround, GROUND_Y);
		ground->GetTexturedRectangle().SetDimension(SCREEN_WIDTH, SCREEN_HEIGHT / 6);
		ground->Render();

		ground->GetTexturedRectangle().SetPosition(scrollingGround + SCREEN_WIDTH, GROUND_Y);
		ground->GetTexturedRectangle().SetDimension(SCREEN_WIDTH, SCREEN_HEIGHT / 6);
		ground->Render();

	}
	else {
		if (isResetingGame) {
			background->Render();

			if (numCharacter == 0 || numCharacter == 1 || numCharacter == 5) {
				numberOfFrame = 2;
			}
			else {
				numberOfFrame = 4;
			}

			++frame;
			if (frame / SLOW_DOWN_RENDER_BIRD >= numberOfFrame) {
				frame = 0;
			}
			scrollingGround -= SPEED_SCROLLING_SCREEN;
			if (scrollingGround < -SCREEN_WIDTH) scrollingGround = 0;
			

		}
		else {
			
			if (isStartingGame) {

				background->Render();

				srand((unsigned int)time(NULL));

				if (flappyBird->GetY() < GROUND_Y - 52) {
					flappyBird->FreeFall(0.5, 0.1);
				}
				else {
					flappyBird->StopOnGround(GROUND_Y - 52);
					isStartingGame = false;
					soundHit->PlaySound(0);

					if (point > stoi(hScore)) {
						hScore = to_string(point);
					}
				}
				++frame;
				if (frame / SLOW_DOWN_RENDER_BIRD >= numberOfFrame && !repeatFly) {
					frame = SLOW_DOWN_RENDER_BIRD;
					repeatFly = true;
				}
				else if (repeatFly) {
					if (numCharacter == 0 || numCharacter == 1 || numCharacter == 5) {
						frame = 0;
					}
					else {
						frame = SLOW_DOWN_RENDER_BIRD;
					}
				}
				flappyBird->ShiftColliders();


				for (int i = 0; i < TOTAL_PIPE; i++) {
					pipe[i]->MoveHorizontal(SPEED_SCROLLING_SCREEN);
					pipe[i]->Render();

					//If pipe moves out of screen, reset normal status and pipe mode
					if (pipe[i]->GetPipeX() < -(pipe[i]->GetPipeWidth())) {
						pipe[i]->SetStatus(SCREEN_WIDTH + SCREEN_WIDTH / (TOTAL_PIPE - 1) - pipe[i]->GetPipeWidth(), MAX_PIPE_Y, MIN_PIPE_Y, PIPE_DISTANCE);
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

						isStartingGame = false;
						soundHit->PlaySound(0);

						if (point > stoi(hScore)) {
							hScore = to_string(point);
						}
					}

				}

				scrollingGround -= SPEED_SCROLLING_SCREEN;
				if (scrollingGround < -SCREEN_WIDTH) scrollingGround = 0;
			}
			else {




				background->Render();
				if (flappyBird->GetY() < GROUND_Y - 52) {
					flappyBird->FreeFall(0.5, 0.1);


				}
				else {
					flappyBird->StopOnGround(GROUND_Y - 52);

				}
				for (int i = 0; i < TOTAL_PIPE; i++) {
					pipe[i]->Render();
				}

			}
		}
		

		if (!isResetingGame && !isStartingGame && !isFlash) {

			flash->GetTexturedRectangle().SetAlpha(alphaFlash);

			if (alphaFlash >= 255) {
				mode = -13;
			}
			alphaFlash += mode;
			flash->Render();

			if (alphaFlash < 0) {
				isFlash = true;

			}
		}
		flappyBird->SetDefaultFrame(0, CharacterProperties[numCharacter].first, CharacterProperties[numCharacter].second, CharacterProperties[numCharacter + 1].first - CharacterProperties[numCharacter].first);
		//character->DrawFrame(numCharacter, 4);
		flappyBird->DrawFrame(frame, SLOW_DOWN_RENDER_BIRD);
		flappyBird->Render();

		ground->GetTexturedRectangle().SetPosition(scrollingGround, GROUND_Y);
		ground->GetTexturedRectangle().SetDimension(SCREEN_WIDTH, SCREEN_HEIGHT / 6);
		ground->Render();

		ground->GetTexturedRectangle().SetPosition(scrollingGround + SCREEN_WIDTH, GROUND_Y);
		ground->GetTexturedRectangle().SetDimension(SCREEN_WIDTH, SCREEN_HEIGHT / 6);
		ground->Render();

		score->Render();
		highScore->Render();
	}


	

	

	


	

	

	



}

int main(int argc, char* args[]) {

	//Initialize random number generator based on runtime value
	srand((unsigned int)time(NULL));

	//Setup the SDLApp
	const char* title = "Flappy Bird";
	app = new SDLApp(IMG_INIT_PNG, title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT);

	//Initialize background
	background = new GameObject(app->GetRenderer(), "asset/image/background.png");
	background->GetTexturedRectangle().SetPosition(0, 0);
	background->GetTexturedRectangle().SetDimension(SCREEN_WIDTH, SCREEN_HEIGHT);

	//Initialize bird
	flappyBird = new Bird(app->GetRenderer(), "asset/image/flappy-bird.png");
	flappyBird->SetDefaultStatus(DEFAULT_BIRD_X, GROUND_Y / 2, 0, 0, 0);
	flappyBird->GetTexturedRectangle().SetDimension(72, 61);
	flappyBird->SetDefaultFrame(0, 0, BIRD_WIDTH / numberOfFrame, BIRD_HEIGHT);

	//Create shape to check collision for bird
	vector<Point> birdCollisionBox;
	birdCollisionBox.push_back({ 714, 550 });
	birdCollisionBox.push_back({ 714, 382 });
	birdCollisionBox.push_back({ 655, 77 });
	birdCollisionBox.push_back({ 470, 0 });
	birdCollisionBox.push_back({ 319, 108 });
	birdCollisionBox.push_back({ 139, 312 });
	birdCollisionBox.push_back({ 139, 550 });
	flappyBird->CreateCollisionShape(birdCollisionBox);

	//Initialize ground (base)
	ground = new Ground(app->GetRenderer(), "asset/image/base.png");

	//Initialize pipes
	for (int i = 0; i < TOTAL_PIPE; i++) {
		pipe[i] = new Pipe(app->GetRenderer(), "asset/image/pipe-green.png", 104, 640);
		pipe[i]->SetStatus(SCREEN_WIDTH + i * SCREEN_WIDTH / (TOTAL_PIPE - 1), MAX_PIPE_Y, MIN_PIPE_Y, PIPE_DISTANCE);
		pipe[i]->SetMode();
	}

	//Initialize white texture for screen flickering
	flash = new GameObject(app->GetRenderer(), "asset/image/white.png");
	flash->GetTexturedRectangle().SetPosition(-50, -50);
	flash->GetTexturedRectangle().SetDimension(SCREEN_WIDTH + 300, SCREEN_HEIGHT + 300);

	//Initialize gameplay score text
	score = new Text(app->GetRenderer(), "asset/font/Flappy-Bird.ttf", "Score: 0", 112, { 255,0,0 });
	score->SetSize(SCORE_X, SCORE_Y, SCORE_WIDTH_CHAR * 8, SCORE_HEIGHT_CHAR);

	//Initialize high score text
	int tmphScore;
	fin.read((char*)&tmphScore, sizeof(int));
	hScore = to_string(tmphScore);
	highScore = new Text(app->GetRenderer(), "asset/font/Flappy-Bird.ttf", "High score: " + hScore, 112, { 255,0,0 });
	highScore->SetSize(SCORE_X, SCORE_Y + SCORE_HEIGHT_CHAR + 2, SCORE_WIDTH_CHAR * (12 + hScore.length()), SCORE_HEIGHT_CHAR);

	//Initialize sound
	soundFly = new SoundEffect("asset/sound/fly.wav");
	soundGetPoint = new SoundEffect("asset/sound/get-point.wav");
	soundDie = new SoundEffect("asset/sound/die.wav");
	soundHit = new SoundEffect("asset/sound/hit.wav");

	bgMusic = new Music("asset/sound/Flappy Bird.wav");
	
	

	logo = new TexturedRectangle(app->GetRenderer(), "asset/image/logo.png");
	logo->SetPosition(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 8);
	logo->SetDimension(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 5);

	startGame = new Button(app->GetRenderer(), "asset/image/start.png", SCREEN_WIDTH / 2 - 75, 2 * SCREEN_HEIGHT / 3 + 15, 150, 75);
	nextLeft = new Button(app->GetRenderer(), "asset/image/nextLeft.png", SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 10, 20, 25);
	nextRight = new Button(app->GetRenderer(), "asset/image/nextRight.png", SCREEN_WIDTH / 2 + 130, SCREEN_HEIGHT / 2 - 10, 20, 25);

	character = new Bird(app->GetRenderer(), "asset/image/flappy-bird.png");
	character->GetTexturedRectangle().SetDimension(108, 91);
	character->GetTexturedRectangle().SetPosition(SCREEN_WIDTH / 2 - 54, SCREEN_HEIGHT / 2 - 40);
	character->SetDefaultFrame(0, CharacterProperties[0].first, CharacterProperties[0].second, CharacterProperties[1].first);

	//Handle events and rendering
	app->SetEventCallback(HandleEvents);
	app->SetRenderCallback(HandleRendering);

	//Game loop
	app->RunAppLoop();

	//Clean up our game
	delete background;
	delete flappyBird;
	delete ground;
	for (int i = 0; i < TOTAL_PIPE; i++) {
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


