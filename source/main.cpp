#include "AIE.h"
#include <iostream>
#include <string>

using namespace std;
//Screen dimensions constants
const int screenWidth = 800;
const int screenHeight = 600;
//Game State enum
enum GAMESTATE {MENU, GAME, ENDGAME};
GAMESTATE GameState = MENU;
//Struct for Paddles
struct paddle {
	//visual variables
	unsigned int sprite;
	unsigned int width = 10;
	unsigned int height = 100;
	//set position
	float x = 30;
	float y = 300;
	//speed the paddle can move
	float speed = 300;
	//keys to control the paddle
	unsigned int upKey;
	unsigned int downKey;
	//set keys
	void setKeys(int s_up, int s_down) {
		upKey = s_up;
		downKey = s_down;
	}
	//controls the movement ingame
	void Move(float timeStep) {
		if (IsKeyDown(upKey) && y <= screenHeight-50) {
			y += timeStep*speed;
		}
		if (IsKeyDown(downKey) && y >= 50) {
			y -= timeStep*speed;
		}
		if (y >= screenHeight - 50) {
			y = screenHeight - 50;
		}
		if (y <= 50) {
			y = 50;
		}
	}
};
//initialise players and scores
paddle leftPlayer;
paddle rightPlayer;
int score1 = 0;
int score2 = 0;
//Ball struct
struct Ball {
	//vissuals
	unsigned int sprite;
	unsigned int width = 20;
	unsigned int height = 20;
	//starting position
	float x = 400;
	float y = 300;
	//two speed variables for horizontal and vertical
	float speedX = 150;
	float speedY = 0;
	//variable for wether the ball is supposed to move left or right
	int direction = 0;
	//moves the ball ingame
	void Move(float timeStep) {
		//if hits the top or bottom of screen
		if (y >= screenHeight - 10) {
			y = screenHeight - 10;
			speedY *= -1;
		}
		if (y <= 10) {
			y = 10;
			speedY *= -1;
		}
		//if goes off the left or right side
		/*if (x >= screenWidth+20) {
			x = 600;
			speedX *= -1;
		}
		if (x <= 0-20) {
			x = 200;
			speedX *= -1;
		}*/
		//moves for y speed
		y += timeStep*speedY;
		//made a variable to change the ball's direction left or right to keep the x speed variable positive at all times
		if (direction == 0) {//going right
			x += timeStep*speedX;
		}
		if (direction == 1) {//going left
			x -= timeStep*speedX;
		}
		//x speed slows whenever a player scores
		//keeps the speed from going below the starting speed
		if (speedX < 150) {
			speedX = 150;
		}
	}
};
//initialize the ball
Ball ball;
//function to put scores and positions and speeds back to the starting state whenever the game is started from the menu
void StartGame() {
	leftPlayer.y = 300;
	rightPlayer.y = 300;
	ball.x = 400;
	ball.y = 300;
	ball.speedX = 150;
	ball.speedY = 0;
	score1 = 0;
	score2 = 0;
}
//Main menu is two strings
void UpdateMenu() {
	DrawString("PongPongPongPongPongPong", 0, screenHeight - 23);
	DrawString("2 Player. Press Space to start!", 50, screenHeight - 230);
	if (IsKeyDown(32)) {
		GameState = GAME;
		StartGame();
	}
}
//most game mechanics are handled in the game function, including scoring and colision
void UpdateGame(float deltaTime, bool playing) {//I used the the same function for the game over screen only it acts differently whether the game is over or not
	if (playing == true) {//if the game is not over
		leftPlayer.Move(deltaTime);
		rightPlayer.Move(deltaTime);
		ball.Move(deltaTime);
		//if the ball hits the left paddle
		if ((ball.x <= leftPlayer.x + 15 && ball.x >= leftPlayer.x + 5) && abs(ball.y - leftPlayer.y) <= 60) {
			ball.x = leftPlayer.x + 15;
			ball.direction = 0;
			ball.speedX += 10;
			//the balls y speed increments based on where it hits the paddle. increments positive if it hits the top half, negetive if the bottom half
			if (ball.y > leftPlayer.y) {
				ball.speedY += 100 * (abs(ball.y - leftPlayer.y) / 60);
			}
			else if (ball.y < leftPlayer.y) {
				ball.speedY -= 100 * (abs(ball.y - leftPlayer.y) / 60);
			}
		}
		//if the ball hits the right paddle
		if ((ball.x >= rightPlayer.x - 15 && ball.x <= rightPlayer.x - 5) && abs(ball.y - rightPlayer.y) <= 60) {
			ball.x = rightPlayer.x - 15;
			ball.direction = 1;
			ball.speedX += 10;
			if (ball.y > rightPlayer.y) {
				ball.speedY += 100 * (abs(ball.y - rightPlayer.y) / 60);
			}
			else if (ball.y < rightPlayer.y) {
				ball.speedY -= 100 * (abs(ball.y - rightPlayer.y) / 60);
			}
		}
		//when the ball goes off the right side
		if (ball.x >= screenWidth + 20) {
			ball.x = 600;
			ball.speedX *= 0.7f;
			ball.direction = 1;
			ball.speedY /= 4;
			score1 += 1;
		}
		//when the ball goes off the left side
		if (ball.x <= 0 - 20) {
			ball.x = 200;
			ball.speedX *= 0.7f;
			ball.direction = 0;
			ball.speedY /= 4;
			score2 += 1;
		}
		//checks if one side has reached the score limit
		if (score1 == 10 || score2 == 10) {
			GameState = ENDGAME;
		}
	}
	//displays win text if the ghame is won
	if (score1 == 10) {
		DrawString("Player 1 Wins!", 50, screenHeight - 230);
	}
	if (score2 == 10) {
		DrawString("Player 2 Wins!", 500, screenHeight - 230);
	}
	//displays player 1's score
	char buffer1[3];
	itoa(score1, buffer1, 10);
	DrawString(buffer1, 50, screenHeight);
	//player 2's score
	char buffer2[3];
	itoa(score2, buffer2, 10);
	DrawString(buffer2, 730, screenHeight);

	MoveSprite(ball.sprite, ball.x, ball.y);
	DrawSprite(ball.sprite);
	MoveSprite(leftPlayer.sprite, leftPlayer.x, leftPlayer.y);
	DrawSprite(leftPlayer.sprite);
	MoveSprite(rightPlayer.sprite, rightPlayer.x, rightPlayer.y);
	DrawSprite(rightPlayer.sprite);
}

int main( int argc, char* argv[] )
{	
    Initialise(screenWidth, screenHeight, false, "PONGPONGPONGPONGPONGPONGPONGPONGPONGPONGPONGPONGPONGPONGPONGPONGPONGPONGPONGPONG");
    
	float fDeltaT = GetDeltaTime();
    SetBackgroundColour(SColour(0, 0, 0, 255));
	//create sprites
    leftPlayer.sprite = CreateSprite("./images/pong.png", leftPlayer.width, leftPlayer.height, true);
	rightPlayer.sprite = CreateSprite("./images/pong.png", rightPlayer.width, rightPlayer.height, true);
	ball.sprite = CreateSprite("./images/pong.png", ball.width, ball.height, true);
	rightPlayer.x = screenWidth - 30;//move player 2 to the right side of the screen
	//set keys
	leftPlayer.setKeys(87, 83);
	rightPlayer.setKeys(79, 76);

	//random For loop
	int forArray[11];
	for (int i = 0; i <= 10; i++) {
		forArray[i] = i;
		cout << i << "\n";
	}

    //Game Loop
    do
    {
		switch (GameState) {
		case MENU:
			UpdateMenu();
			break;
		case GAME:
			fDeltaT = GetDeltaTime();
			UpdateGame(fDeltaT,true);//true or false is if the game is won or not, true if still playing
			if (IsKeyDown(256)) {//press escape to quit to menu
				GameState = MENU;
			}
		case ENDGAME:
			fDeltaT = GetDeltaTime();
			UpdateGame(fDeltaT, false);
			if (IsKeyDown(256)) {//^^
				GameState = MENU;
			}
			break;
		}
		

        ClearScreen();

    } while(!FrameworkUpdate());

    Shutdown();

    return 0;
}
