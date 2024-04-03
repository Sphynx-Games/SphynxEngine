#pragma once


class Widget
{
public:
	Widget() = default;
	virtual ~Widget() = default;

public:
	virtual void RenderGUI() = 0;

};
