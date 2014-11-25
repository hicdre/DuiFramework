#include "stdafx.h"
#include "render_content.h"
#include "render_include.h"
#include "dom/dom_include.h"

namespace ui
{


	RenderContent::RenderContent(UIElement* elem)
		: RenderObject(elem)
	{

	}

	RenderContent::~RenderContent()
	{

	}

	void RenderContent::Layout()
	{
		Rect content_bounds(bounds());
		for (RenderObjectPtr obj = firstChild(); obj; obj = obj->nextSibling())
		{
			//obj->Layout();
		}
	}

}