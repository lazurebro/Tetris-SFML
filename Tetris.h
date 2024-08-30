#pragma once

#include "SFML/Graphics.hpp"
#include "GameField.h"
#include "Shapes.h"
#include "ResourceManager.h"
#include "CodeKonamiHandler.h"

#include<iostream>
#include <list>
#include <string>

class Tetris
{
private:
	GameField* game_filed;

	std::list<Tetromino*> tetromino_queue;
	Tetromino* current_tetromino;
	Tetromino* current_shadow;
	
	sf::RenderWindow* window;

	sf::Font pixel_font;
	sf::Text scoreboard;
	int score = 0;
	int max_score = 0;

	float auto_donw_cd = 0.0f;
	float clear_rows_animation = 0.0f;
	float input_cd = 0.0;

	bool space_released = true;

	std::list<int> need_clear_rows;

	CodeKonami* code_konami;
	bool esster_egg_active = false;

public:
	enum STATES
	{
		STATE_IDLE,
		STATE_CLEAR_ROWS,
	};

	STATES state = STATES::STATE_IDLE;

	Tetris(sf::RenderWindow* _window);
	~Tetris();

	void start();
	void update(float deltaTime); // main tetris`s loop
	void handleEvent(sf::Event); // handle user input
	void gameOverScreen();

	void moveDown(); // alias
	void spawnRandomTetromino();

	void draw();
	void drawUI();
};