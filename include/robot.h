#ifndef ROBOT_H__
#define ROBOT_H__

#include <nds.h>

class Robot
{
private:
	touchPosition _position;

public:
	Robot();
	~Robot();

	void move(int x, int y);
	void draw();
	
	touchPosition& getPosition();
	void setPosition(touchPosition& newPosition);
	void setPosition(int x, int y);

};

#endif // #ifndef ROBOT_H__