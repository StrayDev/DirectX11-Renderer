#pragma once

enum Action
{
	None = 0, KeyPressed, KeyReleased, Char,
	MouseMove, MouseButtonPressed, MouseButtonReleased, MouseScroll
};

class Event
{
public:
	Event() = default;
	virtual ~Event() = default;

	void SetIsHandled(bool value) { is_handled_ = value; }

protected:
	bool is_handled_{ false };
};

class KeyEvent : public Event
{
public:
	KeyEvent(int action, int key) : Action(action), Key(key) {}
	~KeyEvent() override = default;

	int Action;
	unsigned char Key;
};

class MouseEvent : public Event
{
public:
	MouseEvent(int action) : Action(action) {}
	~MouseEvent() override = default;

	int Action;
};

class MouseMoveEvent : public MouseEvent
{
public:
	MouseMoveEvent(int action, int x_pos, int y_pos) : MouseEvent(action), _X(x_pos), _Y(y_pos) {}
	~MouseMoveEvent() override = default;

	double _X;
	double _Y;
};

class MouseButtonEvent : public MouseEvent
{
public:
	MouseButtonEvent(int action, int button) : MouseEvent(action), Button(button) {}
	~MouseButtonEvent() override = default;

	int Button;
};

