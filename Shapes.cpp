#include "Shapes.h"
#include "ResourceManager.h"

Tetromino::Tetromino(sf::RenderWindow* _window, std::vector<std::vector<int>> _points, int _size, int _shape_id)
: window(_window)
{
	shape = Shape{ _size, _points, _shape_id };
	PointDrawer = sf::RectangleShape(sf::Vector2f(CELL_SIZE, CELL_SIZE));
	PointDrawer.setFillColor(Tetromino::getColorFromShapeID(_shape_id));
}

Tetromino::Tetromino(sf::RenderWindow* _window, Shape&& _shape)
	: window(_window), shape(_shape)
{
	PointDrawer = sf::RectangleShape(sf::Vector2f(CELL_SIZE, CELL_SIZE));
	PointDrawer.setFillColor(Tetromino::getColorFromShapeID(shape.shape_id));
}

void Tetromino::setTexture(sf::Texture* texture)
{
	PointDrawer.setTexture(texture);
}

const std::vector<std::vector<int>>& Tetromino::getPoints() const
{
	return this->shape.Points;
}

const int Tetromino::getSize() const
{
	return this->shape.size;
}

void Tetromino::setPos(int x, int y)
{
	this->shape.topLeft.x = x;
	this->shape.topLeft.y = y;
}

void Tetromino::setPos(const sf::Vector2i& new_pos)
{
	this->shape.topLeft = new_pos;
}

sf::Vector2i Tetromino::getPos() const
{
	return this->shape.topLeft;
}

void Tetromino::moveDown()
{
	this->shape.topLeft.y++;
}

void Tetromino::moveRight()
{
	this->shape.topLeft.x++;
}

void Tetromino::moveLeft()
{
	this->shape.topLeft.x--;
}

void Tetromino::rotate(sf::Vector2i offset)
{
	this->shape = this->shape.getRotated();
	this->shape.topLeft += offset;
}

void Tetromino::setShape(Shape&& new_shape)
{
	this->shape = new_shape;
}

const Shape& Tetromino::getShape() const
{
	return this->shape;
}

int Tetromino::getShapeID() const
{
	return this->shape.shape_id;
}

std::map<int, sf::Color> Tetromino::color_map = {
	{SHAPE_O, sf::Color::Yellow},
	{SHAPE_I, sf::Color::Cyan},
	{SHAPE_T, sf::Color::Magenta},
	{SHAPE_J, sf::Color::Magenta},
	{SHAPE_L, sf::Color::Yellow},
	{SHAPE_S, sf::Color::Green},
	{SHAPE_Z, sf::Color::Red },
};

sf::Color Tetromino::getColorFromShapeID(int shape_id)
{
	return Tetromino::color_map[shape_id];
}

void Tetromino::draw() const
{
	// main tetromino draw
	for (int y = 0; y < this->getSize(); y++)
	{
		for (int x = 0; x < this->getSize(); x++)
		{
			if (shape.Points[y][x] == 1)
			{
				PointDrawer.setPosition((shape.topLeft.x + x)*CELL_SIZE, (shape.topLeft.y + y) * CELL_SIZE);
				window->draw(PointDrawer);
			}
		}
	}
}

Tetromino* getShape(sf::RenderWindow* _window, const int id)
{
	switch (id)
	{
	case (SHAPE_O):
		return new Tetromino(_window, {
			{1,1},
			{1,1}
			}, 2, SHAPE_O);

	case (SHAPE_I):
		return new Tetromino(_window, {
			{0,0,0,0},
			{1,1,1,1},
			{0,0,0,0},
			{0,0,0,0},
			}, 4, SHAPE_I);
	
	case (SHAPE_J):
		return new Tetromino(_window, {
			{1,0,0},
			{1,1,1},
			{0,0,0},
			}, 3, SHAPE_J);

	case (SHAPE_L):
		return new Tetromino(_window, {
			{0,0,1},
			{1,1,1},
			{0,0,0},
			}, 3, SHAPE_L);

	case (SHAPE_T):
		return new Tetromino(_window, {
			{0,1,0},
			{1,1,1},
			{0,0,0},
			}, 3, SHAPE_T);

	case (SHAPE_S):
		return new Tetromino(_window, {
			{0,1,1},
			{1,1,0},
			{0,0,0},
			}, 3, SHAPE_S);

	case (SHAPE_Z):
		return new Tetromino(_window, {
			{1,1,0},
			{0,1,1},
			{0,0,0},
			}, 3, SHAPE_Z);
	}
}

Shape Shape::getRotated() const
{
	Shape rotated_shape{};
	rotated_shape.Points = std::vector<std::vector<int>>(4, std::vector<int>(4, 0));
	rotated_shape.size = size;
	rotated_shape.topLeft = topLeft;
	rotated_shape.shape_id = shape_id;

	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{
			rotated_shape.Points[x][size - y - 1] = Points[y][x];
		}
	}

	return rotated_shape;
}