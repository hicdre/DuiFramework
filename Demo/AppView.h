#pragma once
#include "duiframework.h"

class AppView : public ui::UIWindow
{
public:
	virtual void InitWithBounds(const ui::Rect& rect) override;
};