#pragma once
#include "SFML/Graphics.hpp"
#include"Shapes.h"
#include <vector>
#include <set>
// Класс только для хранения поля и манипуляции с ним, как бы интерфйс
// предостовляющий API? для класса Tetris

// Что нужно знать для движения фигуры тетриса?
// Состояние поля landed и структура Tetromino
// Фигура может быть перемещена, только если ей ничего не мешает
// При падении вниз если ей некуда упасть значит ее нужно записать в landed

// В теории можно создать функцию checker, котоаря будет возвращать, можно двигаться вниз или нет
// По результатам этих функций мы будет манипулирвать полем

// GameField - это API, которое использует класс Tetris, значит GameField просто должен предоставить функции для проверки
// А часть по передвижению будет находиться в классе Tetromino

class GameField
{
private:
	std::vector<std::vector<int>> landed;

	sf::RenderWindow* window;
	mutable sf::RectangleShape PointDrawer; // нужен для рисования landed квадратов
	// TODO: Придумать что испольовать
	// 1) Сетку / Grid
	// 2) Черные квадраты, будут чуть красивее и выразительнее

	std::set<int> filled_rows;

	sf::Vector2i getOutOfBounceOffset(const Shape&);

public:
	GameField(sf::RenderWindow*);

	const std::vector<std::vector<int>>& getLanded() const;
	//int getRows() const;
	//int getColumns() const;

	bool canMoveDown(const Tetromino*);
	bool canMoveLeft(const Tetromino*);
	bool canMoveRight(const Tetromino*);
	
	//sf::Vector2i getOutOfBounceOffset(const std::vector<std::vector<int>>& rotated_map, const sf::Vector2i tet_pos); 
	// будет возвращать минимальный оффсет для координат,
	// который будет гарантировать что фигуры не вышла за край карты
	
	//CanRotate canRotate(const std::vector<std::vector<int>>& rotated_map, const sf::Vector2i tet_pos); 
	// будет на основе предоствленных данных
	// возвращать возможен ли поворот

	struct CanRotate
	{
		bool canRotate;
		sf::Vector2i offset;
	};
	CanRotate canRotate(const Tetromino*);

	void LandTetromino(const Tetromino*);

	int findFilledRows(); // Если будет хотя бы одна линия верёться true
	void ClearRows();

	bool checkCollision(const Shape&);

	void ClearField();

	enum DRAW_MODE
	{
		IDLE,
		WITH_FILLED_LINES,
	};
	void draw( DRAW_MODE ) const;
};