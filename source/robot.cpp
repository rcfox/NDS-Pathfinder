#ifndef ROBOT_CPP__
#define ROBOT_CPP__

#include "robot.h"
#include "draw.h"

Robot::Robot()
{
	_position.px = SCREEN_WIDTH/2;
	_position.py = SCREEN_HEIGHT/2;
}

Robot::~Robot()
{

}

void Robot::move(int x, int y)
{
	if (x > _position.px) ++_position.px;
	if (x < _position.px) --_position.px;
	if (y > _position.py) ++_position.py;
	if (y < _position.py) --_position.py;
}

void Robot::draw()
{
	draw::plot(_position.px, _position.py, RGB15(0,0,31));
	draw::plot(_position.px+1, _position.py, RGB15(0,0,31));
	draw::plot(_position.px+2, _position.py, RGB15(0,0,31));
	draw::plot(_position.px-1, _position.py, RGB15(0,0,31));
	draw::plot(_position.px-2, _position.py, RGB15(0,0,31));
	draw::plot(_position.px, _position.py+1, RGB15(0,0,31));
	draw::plot(_position.px, _position.py+2, RGB15(0,0,31));
	draw::plot(_position.px, _position.py-1, RGB15(0,0,31));
	draw::plot(_position.px, _position.py-2, RGB15(0,0,31));
	draw::plot(_position.px+1, _position.py+1, RGB15(0,0,31));
	draw::plot(_position.px+1, _position.py-1, RGB15(0,0,31));
	draw::plot(_position.px-1, _position.py+1, RGB15(0,0,31));
	draw::plot(_position.px-1, _position.py-1, RGB15(0,0,31));
}

touchPosition& Robot::getPosition()
{
	return _position;
}

void Robot::setPosition(int x, int y)
{
	_position.px = x;
	_position.py = y;
}

void Robot::setPosition(touchPosition& newPosition)
{
	_position = newPosition;
}

#endif // #ifndef ROBOT_CPP__
