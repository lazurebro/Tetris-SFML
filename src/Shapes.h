#pragma once
#include <vector>
#include <map>
#include "SFML/Graphics.hpp"
#include "Globals.h"

struct Shape
{
	int size;
	std::vector<std::vector<int>> Points;
	int shape_id;

	sf::Vector2i topLeft;

	Shape getRotated() const;
};

class Tetromino
{
/// <summary>
/// Ответсвенность этого класса состоит в хранении точек, цвета, предоставления поврота
/// В целом просто структура
/// </summary>
private:
	//int size;
	//std::vector<std::vector<int>> Points;
	//sf::Vector2i topLeft; // позиция, которая подразумевает, что это позиция не глобальная в окне, а в сетке (20х10)
	//int shape_id;

	Shape shape;
	sf::Texture main_texture;

	sf::RenderWindow* window;
	mutable sf::RectangleShape PointDrawer; // будет в итерации просто перерисовывать самого себя, но в разных позиуиях
	
public:
	Tetromino( sf::RenderWindow*, std::vector<std::vector<int>>, int size, int _shape_id);
	Tetromino(sf::RenderWindow*, Shape&&);

	void setTexture(sf::Texture*);
	const std::vector<std::vector<int>>& getPoints() const;
	const int getSize() const;

	void setPos(int x, int y);
	void setPos(const sf::Vector2i&);
	sf::Vector2i getPos() const;

	void moveDown();
	void moveRight();
	void moveLeft();
	//void rotate(std::vector<std::vector<int>>&& rotated_map); // rotated_map будет перезаписывать this->Points
	//void rotate(Shape&& rotated_shape);
	void rotate(sf::Vector2i offset);

	void setShape(Shape&& new_shape);
	const Shape& getShape() const;

	int getShapeID() const;

	static std::map<int, sf::Color> color_map;
	static sf::Color getColorFromShapeID(int);

	void draw() const;
};

#define SHAPE_O 1
#define SHAPE_I 2
#define SHAPE_T 3
#define SHAPE_J 4
#define SHAPE_L 5
#define SHAPE_S 6
#define SHAPE_Z 7

Tetromino* getShape(sf::RenderWindow* _window, const int id);