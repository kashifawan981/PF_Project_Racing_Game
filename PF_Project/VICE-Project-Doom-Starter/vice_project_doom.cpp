#include <iostream>
#include <fstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

using namespace sf;
using namespace std;

int screen_x = 1136;
int screen_y = 896;

void display_level(RenderWindow& window, char**lvl, Texture& bgTex,Sprite& bgSprite,Texture& blockTexture,Sprite& blockSprite, const int height, const int width, const int cell_size)
{
	window.draw(bgSprite);

	for (int i = 0; i < height; i += 1)
	{
		for (int j = 0; j < width; j += 1)
		{

			if (lvl[i][j] == '#')
			{
				blockSprite.setPosition(j * cell_size, i * cell_size);
				window.draw(blockSprite);
			}
		}
	}

}

void player_gravity(char** lvl, float& offset_y, float& velocityY, bool& onGround, const float& gravity, float& terminal_Velocity, float& player_x, float& player_y, const int cell_size, int& Pheight, int& Pwidth)
{
	offset_y = player_y;

	offset_y += velocityY;

	char bottom_left_down = lvl[(int)(offset_y + Pheight) / cell_size][(int)(player_x ) / cell_size];
	char bottom_right_down = lvl[(int)(offset_y  + Pheight) / cell_size][(int)(player_x + Pwidth) / cell_size];
	char bottom_mid_down = lvl[(int)(offset_y + Pheight) / cell_size][(int)(player_x + Pwidth / 2) / cell_size];

	if (bottom_left_down == '#' || bottom_mid_down == '#' || bottom_right_down == '#')
	{
		onGround = true;
	}
	else
	{
		player_y = offset_y;
		onGround = false;
	}

	if (!onGround)
	{
		velocityY += gravity;
		if (velocityY >= terminal_Velocity) velocityY = terminal_Velocity;
	}

	else
	{
		velocityY = 0;
	}
}





int main()
{	
	// cout << "here";

	RenderWindow window(VideoMode(screen_x, screen_y), "VICE: Project Doom Game", Style::Resize);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);

	//level specifics
	const int cell_size = 64;
	const int height = 14;
	const int width = 18;
	char** lvl;

	//level and background textures and sprites
	Texture bgTex;
	Sprite bgSprite;
	Texture blockTexture;
	Sprite blockSprite;

	bgTex.loadFromFile("Data/bg.png");
	bgSprite.setTexture(bgTex);
	bgSprite.setPosition(0,0);

	blockTexture.loadFromFile("Data/block1.png");
	blockSprite.setTexture(blockTexture);

	//Music initialisation
	Music lvlMusic;

	lvlMusic.openFromFile("Data/mus.ogg");
	lvlMusic.setVolume(20);
	lvlMusic.play();
	lvlMusic.setLoop(true);

	//player data
	float player_x = 60;
	float player_y = 60;

	const float gravity = 1;

	Texture PlayerTexture;
	Sprite PlayerSprite1;
	Sprite PlayerSprite2; 
	Sprite PlayerSprite3;

	

	bool onGround = false;

	float offset_y = 0;
	float velocityY = 0;

	float terminal_Velocity = 20;

	int PlayerHeight = 104;
	int PlayerWidth = 66;

	PlayerTexture.loadFromFile("Assets/Sprites/3_lightblue.png");
	PlayerSprite1.setTexture(PlayerTexture);
	PlayerSprite1.setTextureRect(IntRect(0, 0, 24, 32));
	PlayerSprite1.setScale(3,3);
	PlayerSprite1.setPosition(player_x, player_y);
    
	PlayerSprite2.setTexture(PlayerTexture);
    PlayerSprite2.setTextureRect(IntRect(24,0, 19, 32));
	PlayerSprite2.setScale(3,3);
    PlayerSprite1.setPosition(player_x, player_y);
	
	PlayerSprite2.setTexture(PlayerTexture);
    PlayerSprite2.setTextureRect(IntRect(38,0, 17, 32));
	PlayerSprite2.setScale(3,3);
    PlayerSprite1.setPosition(100, 700);
	


	//creating level array
	lvl = new char* [height];
	for (int i = 0; i < height; i += 1)
	{
		lvl[i] = new char[width];
	}

	for (int j = 0; j < width; j += 1)
	{
		lvl[13][j] = '#';
	}

	int jump = 0;

	Event ev;

	//main loop
	while (window.isOpen())
	{

		while (window.pollEvent(ev))
		{
			if (ev.type == Event::Closed)
			{
				window.close();
			}
		}

		//presing escape to close
		if (Keyboard::isKeyPressed(Keyboard::Delete))
		{
			window.close();
		}

		window.clear();
		
		display_level(window, lvl, bgTex, bgSprite, blockTexture, blockSprite, height, width, cell_size);
		 player_gravity(lvl,offset_y,velocityY,onGround,gravity,terminal_Velocity, player_x, player_y, cell_size, PlayerHeight, PlayerWidth);
		if ( player_x <= 700){
		 player_x += 1; 
		}
		
		
		PlayerSprite1.setPosition(player_x, player_y);
		window.draw(PlayerSprite1);

		window.display();
	}

	//stopping music and deleting level array
	lvlMusic.stop();
	for (int i = 0; i < height; i++)
	{
		delete[] lvl[i];
	}
	delete[] lvl;

	return 0;
}