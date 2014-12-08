#include "stdafx.h"
#include "ui_hbox.h"
#include "ui_include.h"
#include "render/render_include.h"

namespace ui
{
	static int CalcMarging(int a, int b)
	{
		if (a > b) {
			int t = a;
			a = b;
			b = t;
		}
		//a < b
		if (a > 0 || b < 0) { // 同符号
			return b;
		}
		return a + b; //不同符号
	}

	UIHBox::UIHBox(UIDocumentPtr doc)
		: UIElement(doc)
	{

	}

	UIHBox::~UIHBox()
	{

	}

	void UIHBox::Layout()
	{
		UIStyles* style = styles();
		if (style->autoWidth()) {
			Rect rc(parent()->GetContentBounds());
			bounds_.set_width(rc.right() - style->marginRight());
		}
		else {
			bounds_.set_width(style->pixelWidth());
		}

		UIElement* right_obj = NULL;
		UIElement* bottom_obj = NULL;
		for (UIElementPtr obj = firstChild(); obj; obj = obj->nextSibling())
		{
			//水平布局
			UIStyles* s = obj->styles();
			int x = s->marginLeft();
			int y = s->marginTop();
			if (obj->previousSibling()) {
				x = obj->previousSibling()->bounds().right()
					+ CalcMarging(x, obj->previousSibling()->styles()->marginRight());
			}

			obj->SetPosition(x, y);

			obj->LayoutIfNeeded();

			if (!obj->nextSibling()) {
				right_obj = obj.get();
			}

			if (!bottom_obj || bottom_obj->bounds().bottom() < y)
				bottom_obj = obj.get();
		}

		if (style->autoHeight()) {
			bounds_.set_height(bottom_obj->bounds().bottom() + bottom_obj->styles()->marginBottom());
		}
		else {
			bounds_.set_height(style->pixelHeight());
		}

		needs_layout_ = false;
		child_needs_layout_ = false;
	}

}