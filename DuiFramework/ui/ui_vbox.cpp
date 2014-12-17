#include "stdafx.h"
#include "ui_vbox.h"
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


	UIVBox::UIVBox(UIDocumentPtr doc)
		: UIElement(doc)
	{
		tag_ = "VBox";
	}

	UIVBox::~UIVBox()
	{

	}

	void UIVBox::Layout()
	{
		UIStyles* style = styles();
		if (style->autoWidth()) {
			Rect rc(parent()->GetContentBounds());
			bounds_.set_width(rc.right() - style->marginRight());
		}
		else {
			bounds_.set_width(style->pixelWidth());
		}

		UIElement* bottom_obj = NULL;
		for (UIElementPtr obj = firstChild(); obj; obj = obj->nextSibling())
		{
			//垂直布局
			UIStyles* s = obj->styles();
			int x = s->marginLeft();
			int y = s->marginTop();
			if (obj->previousSibling()) {
				y = obj->previousSibling()->bounds().bottom()
					+ CalcMarging(y, obj->previousSibling()->marginBottom());
			}

			obj->SetPosition(x, y);

			obj->LayoutIfNeeded();

			if (!obj->nextSibling()) {
				bottom_obj = obj.get();
			}


		}

		if (style->autoHeight()) {
			//RenderBox* p = dynamic_cast<RenderBox*>(parent());
			bounds_.set_height(bottom_obj->bounds().bottom() + bottom_obj->marginBottom());
		}
		else {
			bounds_.set_height(style->pixelHeight());
		}

		needs_layout_ = false;
		child_needs_layout_ = false;
	}

}