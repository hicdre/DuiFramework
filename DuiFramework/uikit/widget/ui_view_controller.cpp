#include "stdafx.h"
#include "ui_view_controller.h"

namespace ui
{


	UIViewController::UIViewController()
		: view_(NULL)
	{

	}

	UIViewController::~UIViewController()
	{
		if (view_)
		{
			view_->setViewController(NULL);
		}
	}

	UIResponder* UIViewController::NextResponder() const
	{
		assert(view_);
		return view_->parent();
	}

	void UIViewController::Init()
	{
		UIResponder::Init();

		UIView* v = view();
		if (v)
		{
			v->setViewController(this);
		}
	}

}