#include "Tetris.h"
#include "Windows.h"
#include <fstream>

Tetris::Tetris(sf::RenderWindow* _window)
	: window(_window)
{
	srand(time(nullptr));

	game_filed = new GameField(_window);

	pixel_font.loadFromFile("pixel_font.ttf");
	scoreboard = sf::Text("", pixel_font, 40);

	// read save file
	std::ifstream save("save", std::ios::binary);
	if (save) {
		//read max_score
		save.read((char*)&max_score, sizeof max_score);
		save.close();
	}
	else
	{
		max_score = 0;
	}

	/*
	a - 0
	b - 1
	73 - up
	74 - down
	71 - left
	72 - right*/
#define UP 73
#define DOWN 74
#define LEFT 71
#define RIGHT 72
	code_konami = new CodeKonami({ UP, UP, DOWN, DOWN, LEFT, RIGHT, LEFT, RIGHT, 0, 1 });
}

Tetris::~Tetris()
{
	delete game_filed;
	delete code_konami;
	if (current_tetromino != nullptr)
		delete current_tetromino;
	if (current_shadow != nullptr)
		delete current_shadow;
}

void Tetris::start()
{
	score = 0;
	game_filed->ClearField();

	if (current_tetromino != nullptr)
		delete current_tetromino;
	if (current_shadow != nullptr)
		delete current_shadow;

	tetromino_queue.clear();
	for (int i = 0; i < 3; i++)
	{
		tetromino_queue.push_back(getShape(window, (rand() % 7) + 1));
	}
	this->spawnRandomTetromino();
}

void Tetris::update(float deltaTime)
{
	switch (state)
	{
	case Tetris::STATE_IDLE:

		auto_donw_cd += deltaTime;
		if (auto_donw_cd >= .5)
		{
			auto_donw_cd = 0.0f;
			moveDown();
		}

		break;
	case Tetris::STATE_CLEAR_ROWS:

		clear_rows_animation -= deltaTime;
		if (clear_rows_animation <= 0.0f)
		{
			state = STATE_IDLE;
			game_filed->ClearRows();
			spawnRandomTetromino();

			if (score > max_score)
			{
				max_score = score;
			}
		}

		break;
	default:
		break;
	}
}

void Tetris::handleEvent(sf::Event event)
{
	switch (event.type)
	{
	case (sf::Event::KeyPressed):

		if (code_konami->addSymbol(event.key.code))
		{
			esster_egg_active = true;
		}

		if (current_tetromino)
		{
			if (event.key.code == sf::Keyboard::Key::Down)
			{
				moveDown();
			}
			else if (event.key.code == sf::Keyboard::Key::Left)
			{
				if (game_filed->canMoveLeft(current_tetromino))
				{
					current_tetromino->moveLeft();
				}
			}
			else if (event.key.code == sf::Keyboard::Key::Right)
			{
				if (game_filed->canMoveRight(current_tetromino))
				{
					current_tetromino->moveRight();
				}
			}
			else if (event.key.code == sf::Keyboard::Key::Up)
			{
				GameField::CanRotate result = game_filed->canRotate(current_tetromino);
				if (result.canRotate)
				{
					current_tetromino->rotate(result.offset);
					current_shadow->rotate(result.offset);
				}
			}
			else if (event.key.code == sf::Keyboard::Key::Space && space_released)
			{
				if (not game_filed->checkCollision(current_tetromino->getShape()))
				{
					space_released = false;
					current_tetromino->setPos(current_shadow->getPos());
					moveDown();
				}
			}
		}
		break;

	case (sf::Event::KeyReleased):

		if (event.key.code == sf::Keyboard::Key::Space)
		{
			space_released = true;
		}

		break;

	default:
		break;
	}
}

void Tetris::gameOverScreen()
{
	// write max_score
	std::ofstream save("save", std::ios::binary);
	save.write((char*)&max_score, sizeof max_score);
	save.close();

	int msgboxid = MessageBox(NULL,
		(LPCWSTR)L"Game Over! \nDo you want to try again?",
		(LPCWSTR)L"Tetris - Game Over :c",
		MB_ICONINFORMATION | MB_RETRYCANCEL);

	switch (msgboxid)
	{

	case (IDRETRY):
		this->start();
		break;

	case (IDCANCEL):
		window->close();
		break;

	default:
		break;
	}
}

void Tetris::moveDown()
{
	if (game_filed->canMoveDown(current_tetromino))
	{
		current_tetromino->moveDown();
	}
	else
	{
		game_filed->LandTetromino(current_tetromino);
		delete current_tetromino;
		current_tetromino = nullptr;
		delete current_shadow;
		current_shadow = nullptr;

		int rows = game_filed->findFilledRows();
		if (rows > 0)
		{
			score += rows * 100;
			state = STATE_CLEAR_ROWS;
			clear_rows_animation = .5f; // in sec
		}
		else {
			spawnRandomTetromino();
		}
	}
}

void Tetris::spawnRandomTetromino()
{
	Tetromino* new_tetromino = tetromino_queue.front();
	tetromino_queue.pop_front();
	
	tetromino_queue.push_back(getShape(window, (rand() % 7) + 1));

	current_tetromino = new_tetromino;
	current_tetromino->setPos(4,0);

	if (esster_egg_active)
		current_tetromino->setTexture(ResourceManager::getTexture("Textures/mino_shadow.jpg"));
	else
		current_tetromino->setTexture(ResourceManager::getTexture("Textures/mino.png"));

	current_shadow = getShape(window, current_tetromino->getShapeID());
	current_shadow->setPos(4, 0);
	current_shadow->setTexture(ResourceManager::getTexture("Textures/mino_shadow.png"));

	if (game_filed->checkCollision(current_tetromino->getShape()))
	{
		gameOverScreen();
	}
}

void Tetris::draw()
{
	switch (state)
	{
	case Tetris::STATE_IDLE:

		game_filed->draw(GameField::DRAW_MODE::IDLE);
		current_tetromino->draw();

		current_shadow->setPos(current_tetromino->getPos());
		while (not game_filed->checkCollision(current_shadow->getShape()))
		{
			current_shadow->moveDown();
		}
		current_shadow->setPos( current_shadow->getPos().x, current_shadow->getPos().y-1); // garbage fix, but this work
		current_shadow->draw();
		
		drawUI();

		break;
	case Tetris::STATE_CLEAR_ROWS:

		game_filed->draw(GameField::DRAW_MODE::WITH_FILLED_LINES);

		drawUI();

		break;
	default:
		break;
	}
}

void Tetris::drawUI()
{
	scoreboard.setString("SCORE:");
	scoreboard.setPosition(COLUMNS * CELL_SIZE + 70.0f, 30.0f);
	scoreboard.setFillColor(sf::Color::Red);
	window->draw(scoreboard);

	std::string score_str = std::to_string(score);
	while (score_str.size() < 9) score_str = "0" + score_str;
	scoreboard.setString(score_str);
	scoreboard.setPosition(COLUMNS * CELL_SIZE + 70.0f, 45.0f+30.0f);
	scoreboard.setFillColor(sf::Color::White);
	window->draw(scoreboard);

	scoreboard.setString("MAX SCORE:");
	scoreboard.setPosition(COLUMNS * CELL_SIZE + 70.0f, 45.0f + 30.0f + 100.0f);
	scoreboard.setFillColor(sf::Color::Red);
	window->draw(scoreboard);

	score_str = std::to_string(max_score);
	while (score_str.size() < 9) score_str = "0" + score_str;
	scoreboard.setString(score_str);
	scoreboard.setPosition(COLUMNS * CELL_SIZE + 70.0f, 45.0f + 30.0f + 100.0f + 45.0f);
	scoreboard.setFillColor(sf::Color::White);
	window->draw(scoreboard);

	// draw next figures
	int i = 0;
	for (auto it = tetromino_queue.rbegin(); it != tetromino_queue.rend(); it++)
	{
		Tetromino* tet = *it;
		sf::Vector2f ui_pos(COLUMNS * CELL_SIZE + 70.0f, 45.0f + 30.0f + 100.0f + 45.0f);

		tet->setTexture(ResourceManager::getTexture("Textures/mino.png"));
		tet->setPos(COLUMNS + 3, 9 + i*3);
		tet->draw();

		i++;
	}
}

