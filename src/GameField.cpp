#include "GameField.h"
#include<iostream>
#include "ResourceManager.h"
GameField::GameField(sf::RenderWindow* _window)
	: window(_window)
{
	PointDrawer = sf::RectangleShape(sf::Vector2f(CELL_SIZE, CELL_SIZE));
	PointDrawer.setTexture(ResourceManager::getTexture("assets/Textures/mino.png"));
	landed = std::vector<std::vector<int>>(ROWS, std::vector<int>(COLUMNS, 0));
}

const std::vector<std::vector<int>>& GameField::getLanded() const
{
	return this->landed;
}

bool GameField::canMoveDown(const Tetromino* tetromino)
{
	const std::vector<std::vector<int>>& points = tetromino->getPoints();
	for (int y = 0; y < tetromino->getSize(); y++)
	{
		for (int x = 0; x < tetromino->getSize(); x++)
		{
			if (points[y][x] == 1)
			{
				sf::Vector2i tet_pos = tetromino->getPos();
				sf::Vector2i next_pos = sf::Vector2i( tet_pos.x + x, tet_pos.y + y + 1 );

				if ( next_pos.y >= ROWS || landed[next_pos.y][next_pos.x] != 0)
				{
					return false;
				}
			}
		}
	}
	return true;
}

bool GameField::canMoveLeft(const Tetromino* tetromino)
{
	const std::vector<std::vector<int>>& points = tetromino->getPoints();
	for (int y = 0; y < tetromino->getSize(); y++)
	{
		for (int x = 0; x < tetromino->getSize(); x++)
		{
			if (points[y][x] == 1)
			{
				sf::Vector2i tet_pos = tetromino->getPos();
				sf::Vector2i next_pos = sf::Vector2i(tet_pos.x + x - 1, tet_pos.y + y);

				if (next_pos.x < 0 || landed[next_pos.y][next_pos.x] != 0)
				{
					return false;
				}
			}
		}
	}
	return true;
}

bool GameField::canMoveRight(const Tetromino* tetromino)
{
	const std::vector<std::vector<int>>& points = tetromino->getPoints();
	for (int y = 0; y < tetromino->getSize(); y++)
	{
		for (int x = 0; x < tetromino->getSize(); x++)
		{
			if (points[y][x] == 1)
			{
				sf::Vector2i tet_pos = tetromino->getPos();
				sf::Vector2i next_pos = sf::Vector2i(tet_pos.x + x + 1, tet_pos.y + y);

				if (next_pos.x >= COLUMNS || landed[next_pos.y][next_pos.x] != 0)
				{
					return false;
				}
			}
		}
	}
	return true;
}



sf::Vector2i GameField::getOutOfBounceOffset(const Shape& shape)
{
	sf::Vector2i offset(0, 0);

	for (int y = 0; y < shape.size; y++)
	{
		for (int x = 0; x < shape.size; x++)
		{
			if (shape.Points[y][x] == 1)
			{
				int new_x = shape.topLeft.x + x;
				int new_y = shape.topLeft.y + y;

				if (new_x < 0)
				{
					offset.x++;
				}
				else if (new_x > COLUMNS - 1)
				{
					offset.x--;
				}

				if (new_y < 0)
				{
					offset.y++;
				}
				else if (new_y > ROWS - 1)
				{
					offset.y--;
				}
			}
		}
	}

	return offset;
}

GameField::CanRotate GameField::canRotate(const Tetromino* tetromino)
{
	const Shape& current_shape = tetromino->getShape();
	Shape rotated_shape = current_shape.getRotated();

	sf::Vector2i offset = getOutOfBounceOffset(rotated_shape);
	rotated_shape.topLeft += offset;

	for (int y = 0; y < rotated_shape.size; y++)
	{
		for (int x = 0; x < rotated_shape.size; x++)
		{
			if (rotated_shape.Points[y][x] == 1)
			{
				if (landed[rotated_shape.topLeft.y + y][rotated_shape.topLeft.x + x] != 0)
				{
					return GameField::CanRotate{false};
				}
			}
		}
	}

	return GameField::CanRotate{ true, offset };
}

void GameField::LandTetromino(const Tetromino* tetromino)
{
	const std::vector<std::vector<int>>& points = tetromino->getPoints();
	
	for (int y = 0; y < tetromino->getSize(); y++)
	{
		for (int x = 0; x < tetromino->getSize(); x++)
		{
			if (points[y][x] == 1)
			{
				sf::Vector2i landed_pos = tetromino->getPos();
				landed_pos.x += x;
				landed_pos.y += y;

				landed[landed_pos.y][landed_pos.x] = tetromino->getShapeID();
			}
		}
	}
}

int GameField::findFilledRows()
{
	filled_rows.clear();
	for (int y = 0; y < ROWS; y++)
	{
		bool isFilled = true;
		for (int x = 0; x < COLUMNS; x++)
		{
			if (landed[y][x] == 0)
			{
				isFilled = false;
				break;
			}
		}
		if (isFilled)
			filled_rows.insert(y);
	}

	return filled_rows.size();
}

void GameField::ClearRows()
{
	for (auto it = filled_rows.begin(); it != filled_rows.end(); it++)
	{
		landed.erase(std::begin(landed) + *it);

		std::vector<int> ins(COLUMNS, 0);
		landed.insert(std::begin(landed), ins);
	}
}

bool GameField::checkCollision(const Shape& shape)
{
	for (int y = 0; y < shape.size; y++)
	{
		for (int x = 0; x < shape.size; x++)
		{
			if (shape.Points[y][x] == 1)
			{
				sf::Vector2i landed_pos = shape.topLeft;
				landed_pos.x += x;
				landed_pos.y += y;

				if (landed_pos.x < 0 || landed_pos.x >= COLUMNS || landed_pos.y < 0 || landed_pos.y >= ROWS)
					return true;
				if (landed[landed_pos.y][landed_pos.x] != 0) return true;
			}
		}
	}

	return false;
}

void GameField::ClearField()
{
	landed = std::vector<std::vector<int>>(ROWS, std::vector<int>(COLUMNS, 0));
}

void GameField::draw( DRAW_MODE draw_mode ) const
{
	switch (draw_mode)
	{
	case (DRAW_MODE::IDLE):
		
		for (int y = 0; y < ROWS; y++)
		{
			for (int x = 0; x < COLUMNS; x++)
			{
				switch (landed[y][x]) {
				case (0):
					PointDrawer.setTexture(ResourceManager::getTexture("assets/Textures/empty_board.png"));
					PointDrawer.setFillColor(sf::Color(100,100,100));
					break;
				default:
					PointDrawer.setTexture(ResourceManager::getTexture("assets/Textures/mino.png"));
					PointDrawer.setFillColor(Tetromino::getColorFromShapeID(landed[y][x]));
					break;
				}
				PointDrawer.setPosition(x * CELL_SIZE, y * CELL_SIZE);
				window->draw(PointDrawer);
			}
		}

		break;

	case (DRAW_MODE::WITH_FILLED_LINES):

		for (int y = 0; y < ROWS; y++)
		{
			for (int x = 0; x < COLUMNS; x++)
			{
				PointDrawer.setPosition(x * CELL_SIZE, y * CELL_SIZE);

				if (filled_rows.count(y))
				{
					PointDrawer.setTexture(0);
					PointDrawer.setFillColor(sf::Color::White);
				}
				else
				{
					switch (landed[y][x]) {
					case (0):
						PointDrawer.setTexture(ResourceManager::getTexture("assets/Textures/empty_board.png"));
						PointDrawer.setFillColor(sf::Color(100, 100, 100));
						break;
					default:
						PointDrawer.setTexture(ResourceManager::getTexture("assets/Textures/mino.png"));
						PointDrawer.setFillColor(Tetromino::getColorFromShapeID(landed[y][x]));
						break;
					}
				}
					//PointDrawer.setFillColor(Tetromino::getColorFromShapeID(landed[y][x]));

				window->draw(PointDrawer);
			}
		}

		break;
	}
}
