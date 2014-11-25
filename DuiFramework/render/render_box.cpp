#include "stdafx.h"
#include "render_box.h"
#include "dom/dom_element.h"
#include "render_content.h"

namespace ui
{


	RenderBox::RenderBox(UIElement* elem)
		: RenderObject(elem)
	{

	}

	RenderBox::~RenderBox()
	{

	}

	RenderContent* RenderBox::GetRenderChild()
	{
		return child_.get();
	}

	void RenderBox::Layout()
	{
		if (child_)
			child_->Layout();
	}

}