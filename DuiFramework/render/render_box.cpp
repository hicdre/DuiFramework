#include "stdafx.h"
#include "render_box.h"
#include "dom/dom_element.h"
#include "render/render_include.h"

namespace ui
{
	int CalcMarging(int a, int b)
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

	RenderBox::RenderBox(UIElement* elem)
		: RenderObject(elem)
		, needs_layout_(true)
		, child_needs_layout_(true)
	{

	}

	RenderBox::~RenderBox()
	{

	}

	void RenderBox::Layout()
	{
		//layout之前，位置已经确定
		//计算宽度, 宽度只与父有关
		UIStyles* style = styles();
		if (style->autoWidth()) {
			RenderBox* p = dynamic_cast<RenderBox*>(parent());
			Rect rc(p->GetContentBounds());
			bounds_.set_width(rc.right() - style->marginRight());
		}
		else {
			bounds_.set_width(style->pixelWidth());
		}

		RenderObject* bottom_obj = NULL;
		for (RenderObjectPtr obj = firstChild(); obj; obj = obj->nextSibling())
		{
			//绝对布局
			UIStyles* s = obj->styles();
			int x = s->marginLeft();
			int y = s->marginTop();

			obj->SetPosition(x, y);

			obj->LayoutIfNeeded();

			int bottom = obj->bounds().bottom();
			if (!bottom_obj || bottom_obj->bounds().bottom() < bottom)
				bottom_obj = obj.get();
		}

		if (style->autoHeight()) {
			RenderBox* p = dynamic_cast<RenderBox*>(parent());
			bounds_.set_height(bottom_obj->bounds().bottom());
		}
		else {
			bounds_.set_height(style->pixelHeight());
		}

		needs_layout_ = false;
		child_needs_layout_ = false;
	}


	void RenderBox::LayoutIfNeeded()
	{
		if (needs_layout_) {
			Layout();
		}
	}



	void RenderBox::DoPaint(RenderContext* painter, const Rect& r)
	{
		ScopedPainter helper(painter, Matrix(1.0, 0, 0, 1.0, r.x(), r.y()));

		PaintBackground(painter);
		PaintBorder(painter);
		PaintContents(painter);
	}

	void RenderBox::PaintBackground(RenderContext* painter)
	{
		Color color = styles()->backgroundColor();
		painter->FillRect(GetLocalBounds(), color);
	}

	void RenderBox::PaintBorder(RenderContext* painter)
	{

	}

	void RenderBox::PaintContents(RenderContext* painter)
	{
		Rect rc(GetContentBounds());
		ScopedPainter helper(painter, Matrix(1.0, 0, 0, 1.0, rc.x(), rc.y()));

		for (RenderObjectPtr obj = firstChild(); obj; obj = obj->nextSibling())
		{
			obj->DoPaint(painter, obj->bounds());
		}
	}

	Rect RenderBox::GetContentBounds()
	{
		return GetLocalBounds();
	}

	void RenderBox::OnBoundsChanged()
	{
		needs_layout_ = true;
		RenderBox* pbox = parentBox();
		if (pbox)
			pbox->OnChildBoundsChanged(this);
		else
			Layout();
	}


	void RenderBox::OnChildBoundsChanged(RenderBox* box)
	{
		box->Layout();
	}


	RenderBox* RenderBox::parentBox()
	{
		return dynamic_cast<RenderBox*>(parent());
	}

	int RenderBox::marginLeft() const
	{
		return styles_->marginLeft();
	}

	int RenderBox::marginTop() const
	{
		return styles_->marginTop();
	}

	int RenderBox::marginRight() const
	{
		return styles_->marginRight();
	}

	int RenderBox::marginBottom() const
	{
		return styles_->marginBottom();
	}


	RenderHBox::RenderHBox(UIElement* elem)
		: RenderBox(elem)
	{

	}

	RenderHBox::~RenderHBox()
	{

	}

	void RenderHBox::Layout()
	{
		UIStyles* style = styles();
		if (style->autoWidth()) {
			RenderBox* p = dynamic_cast<RenderBox*>(parent());
			Rect rc(p->GetContentBounds());
			bounds_.set_width(rc.right() - style->marginRight());
		}
		else {
			bounds_.set_width(style->pixelWidth());
		}

		RenderObject* right_obj = NULL;
		RenderObject* bottom_obj = NULL;
		for (RenderObjectPtr obj = firstChild(); obj; obj = obj->nextSibling())
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
			RenderBox* p = dynamic_cast<RenderBox*>(parent());
			bounds_.set_height(bottom_obj->bounds().bottom() + bottom_obj->styles()->marginBottom());
		}
		else {
			bounds_.set_height(style->pixelHeight());
		}

		needs_layout_ = false;
		child_needs_layout_ = false;
	}



	RenderVBox::RenderVBox(UIElement* elem)
		: RenderBox(elem)
	{

	}

	RenderVBox::~RenderVBox()
	{

	}

	void RenderVBox::Layout()
	{
		UIStyles* style = styles();
		if (style->autoWidth()) {
			RenderBox* p = dynamic_cast<RenderBox*>(parent());
			Rect rc(p->GetContentBounds());
			bounds_.set_width(rc.right() - style->marginRight());
		}
		else {
			bounds_.set_width(style->pixelWidth());
		}

		RenderObject* bottom_obj = NULL;
		for (RenderObjectPtr obj = firstChild(); obj; obj = obj->nextSibling())
		{
			//垂直布局
			UIStyles* s = obj->styles();
			int x = s->marginLeft();
			int y = s->marginTop();
			if (obj->previousSibling()) {
				y = obj->previousSibling()->bounds().bottom()
					+ CalcMarging(y, obj->previousSibling()->styles()->marginBottom());
			}

			obj->SetPosition(x, y);

			obj->LayoutIfNeeded();

			if (!obj->nextSibling()) {
				bottom_obj = obj.get();
			}

			
		}

		if (style->autoHeight()) {
			RenderBox* p = dynamic_cast<RenderBox*>(parent());
			bounds_.set_height(bottom_obj->bounds().bottom() + bottom_obj->styles()->marginBottom());
		}
		else {
			bounds_.set_height(style->pixelHeight());
		}

		needs_layout_ = false;
		child_needs_layout_ = false;
	}

}