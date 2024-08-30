#pragma once
#include "SFML/Graphics.hpp"
#include"Shapes.h"
#include <vector>
#include <set>
// ����� ������ ��� �������� ���� � ����������� � ���, ��� �� ��������
// ��������������� API? ��� ������ Tetris

// ��� ����� ����� ��� �������� ������ �������?
// ��������� ���� landed � ��������� Tetromino
// ������ ����� ���� ����������, ������ ���� �� ������ �� ������
// ��� ������� ���� ���� �� ������ ������ ������ �� ����� �������� � landed

// � ������ ����� ������� ������� checker, ������� ����� ����������, ����� ��������� ���� ��� ���
// �� ����������� ���� ������� �� ����� ������������� �����

// GameField - ��� API, ������� ���������� ����� Tetris, ������ GameField ������ ������ ������������ ������� ��� ��������
// � ����� �� ������������ ����� ���������� � ������ Tetromino

class GameField
{
private:
	std::vector<std::vector<int>> landed;

	sf::RenderWindow* window;
	mutable sf::RectangleShape PointDrawer; // ����� ��� ��������� landed ���������
	// TODO: ��������� ��� �����������
	// 1) ����� / Grid
	// 2) ������ ��������, ����� ���� �������� � �������������

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
	// ����� ���������� ����������� ������ ��� ���������,
	// ������� ����� ������������� ��� ������ �� ����� �� ���� �����
	
	//CanRotate canRotate(const std::vector<std::vector<int>>& rotated_map, const sf::Vector2i tet_pos); 
	// ����� �� ������ �������������� ������
	// ���������� �������� �� �������

	struct CanRotate
	{
		bool canRotate;
		sf::Vector2i offset;
	};
	CanRotate canRotate(const Tetromino*);

	void LandTetromino(const Tetromino*);

	int findFilledRows(); // ���� ����� ���� �� ���� ����� ������� true
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