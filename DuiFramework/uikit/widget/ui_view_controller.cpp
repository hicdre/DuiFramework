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
	}

	UIResponder* UIViewController::NextResponder() const
	{
		assert(view_);
		return view_->parent();
	}

	void UIViewController::Init()
	{
		UIResponder::Init();

	}

	void UIViewController::viewWillAppear()
	{

	}

	void UIViewController::viewDidAppear()
	{

	}

	void UIViewController::viewWillDisappear()
	{

	}

	void UIViewController::viewDidDisappear()
	{

	}

	void UIViewController::loadView()
	{

	}

	UIView* UIViewController::view()
	{
		if (view_ == NULL) {
			loadView();
		}
		return view_;
	}

}