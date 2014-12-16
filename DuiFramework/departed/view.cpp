#include "stdafx.h"
#include "view.h"

#include <algorithm>
#include <cassert>

#include "render/render_context.h"

namespace ui
{

	View::View()
		: layout_data_(new LayoutBox())
	{

	}

	View::~View()
	{
	}

	View* View::parent() const
	{
		return parent_;
	}

	View* View::prevSibling() const
	{
		return prev_;
	}

	View* View::nextSibling() const
	{
		return next_;
	}

	void View::setParent(View* v)
	{
		parent_ = v;
	}

	void View::setPrevSibling(View* v)
	{
		prev_ = v;
	}

	void View::setNextSibling(View* v)
	{
		next_ = v;
	}

	Container* View::GetContainer()
	{
		return container_.get();
	}


	void View::SetContainer(Container* container)
	{
		container_.reset(container);
	}


#if 0
	View* View::Append(View* child)
	{
		return InsertAfter(last_child_, child);
	}

	View* View::Remove(View* child)
	{
		//ֻ�����Ƴ��ӽڵ�
		if (!child || child->parent_ != this)
		{
			assert(0); //<< "can only remove child node";
			return NULL;
		}

		//���¸��ӹ�ϵ
		child->parent_ = NULL;
		if (first_child_ == child)
		{
			first_child_ = child->next_sibling_;
		}
		if (last_child_ == child)
		{
			last_child_ = child->prev_sibling_;
		}

		//�����ֵܹ�ϵ
		if (child->prev_sibling_)
		{
			child->prev_sibling_->next_sibling_ = child->next_sibling_;
		}

		if (child->next_sibling_)
		{
			child->next_sibling_->prev_sibling_ = child->prev_sibling_;
		}

		child->next_sibling_ = NULL;
		child->prev_sibling_ = NULL;

		child_count_--;

		return child;
	}

	View* View::InsertAfter(View* ref, View* child)
	{
		if (ref == child || child == NULL || (ref && ref->parent_ != this))
		{
			assert(0);
			return NULL;
		}

		//�ȴ�ԭ�ڵ��Ƴ�
		if (child->parent_)
		{
			child->parent_->Remove(child);
		}
		child->parent_ = this;

		child->prev_sibling_ = ref;
		if (ref)
		{//�嵽�м�����
			child->next_sibling_ = ref->next_sibling_;
			if (ref->next_sibling_)
			{
				ref->next_sibling_->prev_sibling_ = child;
			}
			ref->next_sibling_ = child;

			assert(last_child_);
			//�嵽�����
			if (last_child_->next_sibling_)
			{
				last_child_ = last_child_->next_sibling_;
			}
		}
		else
		{//�嵽��ǰ
			child->next_sibling_ = first_child_;
			if (first_child_)
			{
				first_child_->prev_sibling_ = child;
			}
			first_child_ = child;

			//ԭ��û����Ԫ��
			if (last_child_ == NULL)
			{
				last_child_ = child;
			}
		}
		child_count_++;

		return child;
	}

	View* View::InsertBefore(View* ref, View* child)
	{
		if (ref == child || child == NULL || (ref && ref->parent_ != this))
		{
			assert(0);// << "insert before invalid";
			return NULL;
		}

		if (ref)
		{
			return InsertAfter(ref->prev_sibling_, child);
		}
		else
		{
			return InsertAfter(last_child_, child);
		}
	}

	View* View::AppendTo(View* parent)
	{
		return parent->Append(this);
	}

	View* View::Detach()
	{
		return parent_->Remove(this);
	}

	View* View::GetAncestorTo(View* other)
	{
		View* root_view = root();
		int step_this = 0;
		int step_that = 0;
		if (!root_view->HasDescender(other, &step_that))
			return NULL;

		if (step_that == 0)
			return other;

		if (root_view == this)
			return this;

		root_view->HasDescender(this, &step_this);

		View* view_this = this;
		View* view_that = other;

		if (step_this > step_that) {
			int delta = step_this - step_that;
			while (delta) {
				delta--;
				view_this = view_this->parent_;
			}
		} else if (step_this < step_that) {
			int delta = step_that - step_this;
			while (delta) {
				delta--;
				view_that = view_that->parent_;
			}
		}
		//Ȼ��ͬʱ��ǰ����
		while (view_this && view_that && view_this != view_that)
		{
			view_this = view_this->parent_;
			view_that = view_that->parent_;
		}

		if (view_this == view_that)
			return view_this;
		assert(0);
		return NULL;
	}

	bool View::HasDescender(View* descender, int* step /*= NULL*/)
	{
		*step = 0;
		for (View* v = descender; v; v = v->parent(), (*step)++)
		{
			if (v == this)
				return true;
		}
		return false;
	}
#endif


	void View::SetBounds(int x, int y, int width, int height)
	{
		SetBoundsRect(Rect(x, y, (std::max)(0, width), (std::max)(0, height)));
	}

	void View::SetBoundsRect(const Rect& bounds)
	{
		if (bounds == bounds_) {
			if (needs_layout_) {
				needs_layout_ = false;
				Layout();
				SchedulePaint();
			}
			return;
		}

		if (visible_) {
			// Paint where the view is currently.
			SchedulePaint();
		}

		Rect prev = bounds_;
		bounds_ = bounds;
		if (prev.size() != size()) {
			needs_layout_ = false;
			Layout();
			SchedulePaint();
		}
	}

	void View::SetSize(const Size& size)
	{
		SetBounds(x(), y(), size.width(), size.height());
	}

	void View::SetPosition(const Point& position)
	{
		SetBounds(position.x(), position.y(), width(), height());
	}

	void View::SetX(int x)
	{
		SetBounds(x, y(), width(), height());
	}

	void View::SetY(int y)
	{
		SetBounds(x(), y, width(), height());
	}

	Rect View::GetLocalBounds() const
	{
		return Rect(width(), height());
	}


	Rect View::GetContentsBounds() const
	{
		if (!border_.get())
			return GetLocalBounds();

		Rect rc(GetLocalBounds());
		rc.Inset(border_->GetPadding());
		return rc;
	}


	void View::SetVisible(bool visible)
	{
		if (visible != visible_) {
			// If the View is currently visible, schedule paint to refresh parent.
			// TODO(beng): not sure we should be doing this if we have a layer.
			if (visible_)
				SchedulePaint();

			visible_ = visible;

			OnVisibleChanged();

			// Notify the parent.
			if (parent_)
				parent_->OnChildVisibilityChanged(this);

			// This notifies all sub-views recursively.
			//PropagateVisibilityNotifications(this, visible_);
			//UpdateLayerVisibility();

			// If we are newly visible, schedule paint.
			if (visible_)
				SchedulePaint();
		}
	}

	bool View::IsDrawn() const
	{
		return visible_ && parent_ ? parent_->IsDrawn() : false;
	}

	void View::SetEnabled(bool enabled)
	{
		if (enabled != enabled_) {
			enabled_ = enabled;
			OnEnabledChanged();
		}
	}

	void View::Layout()
	{
		needs_layout_ = false;

		OnLayout();
		
		if (container_.get())
			container_->Layout();

	}

	void View::OnVisibleChanged()
	{

	}

	void View::OnEnabledChanged()
	{

	}

	void View::OnLayout()
	{

	}


	void View::SchedulePaint()
	{
		SchedulePaintInRect(GetLocalBounds());
	}

	void View::SchedulePaintInRect(const Rect& r)
	{
		if (!visible_)
			return;

		if (parent_) {
			// Translate the requested paint rect to the parent's coordinate system
			// then pass this notification up to the parent.
			//�˴���Ҫ���б任
			parent_->OnChildSchedulePaintInRect(this, r);
		}
	}

	void View::DoPaint(RenderContext* painter, const Rect& r)
	{
		if (!visible_)
			return;

		ScopedPainter helper(painter, Matrix(1.0, 0, 0, 1.0, r.x(), r.y()));

		if (background_.get())
			background_->DoPaint(painter, GetLocalBounds());

		if (border_.get())
			border_->DoPaint(painter, GetLocalBounds());

		OnBeforeChildPaint(painter);

		if (container_.get())
			container_->DoPaint(painter, GetContentsBounds());

		OnAfterChildPaint(painter);
	}


	void View::OnBeforeChildPaint(RenderContext* painter)
	{
		//painter->DrawStringRect(L"����", Font(L"΢���ź�", 18), ColorSetRGB(255,0,0,0), GetLocalBounds());
	}

	void View::OnAfterChildPaint(RenderContext* painter)
	{
		//painter->DrawStringRect(L"����", Font(L"΢���ź�", 18), ColorSetRGB(255,0,0,0), GetLocalBounds());
	}

	/*void View::ConvertPointToView(const View* source, const View* target, Point* point)
	{
		if (source == target)
			return;

		// |source| can be NULL.
		const View* root = target->root();
		if (source) {
			assert(source->root() == root);

			if (source != root)
				source->ConvertPointForAncestor(root, point);
		}

		if (target != root)
			target->ConvertPointFromAncestor(root, point);

		// API defines NULL |source| as returning the point in screen coordinates.
		if (!source) {
			*point = point->Subtract(
				root->GetWidget()->GetClientAreaScreenBounds().origin());
		}
	}

	void View::ConvertPointToWidget(const View* src, Point* point)
	{

	}

	void View::ConvertPointFromWidget(const View* dest, Point* p)
	{

	}

	void View::ConvertPointToScreen(const View* src, Point* point)
	{

	}

	void View::ConvertPointFromScreen(const View* dst, Point* point)
	{

	}

	Point View::ConvertPointFromWidget(const Point& pt) const
	{
		Views vs;
		for (const View* v = this; v; v = v->parent())
			vs.push_back(const_cast<View*>(v));

		Point x_pt = pt;
		for (View* v : vs) {
			x_pt = v->ConvertPointFromParent(x_pt);
		}
		return x_pt;
	}

	Point View::ConvertPointFromParent(const Point& pt) const
	{
		return GetTransform().Invert().Apply(pt);
	}

	Point View::ConvertPointToParent(const Point& pt) const
	{
		return GetTransform().Apply(pt);
	}

	Point View::ConvertPointToWidget(const Point& pt) const
	{
		Point x_pt = pt;
		for (const View* v = this; v; v = v->parent())
			x_pt = v->ConvertPointToParent(x_pt);
		return x_pt;
	}

	Rect View::ConvertRectToParent(const Rect& rect) const
	{
		return GetTransform().Apply(rect);
	}

	Rect View::ConvertRectToWidget(const Rect& rect) const
	{
		Rect x_rect = rect;
		for (const View* v = this; v; v = v->parent())
			x_rect = v->ConvertRectToParent(x_rect);
		return x_rect;
	}
	*/

	void View::SetBackground(Background* background)
	{
		background_.reset(background);
	}

	Background* View::background()
	{
		return background_.get();
	}

	void View::SetBorder(Border* border)
	{
		border_.reset(border);
	}

	Border* View::border() const
	{
		return border_.get();
	}

	void View::SetCursor(HCURSOR cursor)
	{
		cursor_ = cursor;
	}


	HCURSOR View::GetCursor()
	{
		return cursor_;
	}

	void View::OnChildVisibilityChanged(View* child)
	{

	}

	void View::OnChildSchedulePaintInRect(View* child, const Rect& r)
	{
		SchedulePaintInRect(ConvertRectFromChild(child, r));
	}

	Rect View::ConvertRectFromChild(View* child, const Rect& r)
	{
		return Rect(r.x() + child->x(), r.y() + child->y(), r.width(), r.height());
	}

	LayoutBox* View::layoutBox()
	{
		return layout_data_.get();
	}

	int View::layoutWidth()
	{
		StyleValueType type = StyleValue_Auto;
		StyleValue* v = style_declarations_.FindValue(Style_Width);
		if (v)
			type = v->GetType();
		if (type == StyleValue_Pixel) {
			return v->GetPixel();
		}

		if (type == StyleValue_Percent && parent_) {
			return parent_->GetContentsBounds().width() * v->GetPercentValue();
		}

		if (type == StyleValue_Auto) {
			if (parent_ && parent_->IsVerticalLayout()) {
				return parent_->GetContentsBounds().width() - marginLeft() - marginRight();
			}

			if (container_.get()) {
				return container_->GetAutoWidth();
			}
		}

		return 0;
	}

	int View::layoutHeight()
	{
		StyleValueType type = StyleValue_Auto;
		StyleValue* v = style_declarations_.FindValue(Style_Height);
		if (v)
			type = v->GetType();
		if (type == StyleValue_Pixel) {
			return v->GetPixel();
		}

		if (type == StyleValue_Percent && parent_) {
			return parent_->GetContentsBounds().height() * v->GetPercentValue();
		}

		if (type == StyleValue_Auto) {
			if (parent_ && parent_->IsHorizonalLayout()) {
				return parent_->GetContentsBounds().height() - marginTop() - marginBottom();
			}

			if (container_.get()) {
				return container_->GetAutoHeight();
			}
		}

		return 0;
	}

	int View::layoutX()
	{
		if (!parent_)
			return 0;

		int dx = marginLeft();
		View* prev_silbing = prevSibling();
		while (prev_silbing && !prev_silbing->visible())
		{
			prev_silbing = prev_silbing->prevSibling();
		}
		if (parent_->IsHorizonalLayout() && prev_silbing)
		{//������������ǰһ��
			int pdx = prev_silbing->marginRight();
			if (dx < pdx) {
				int t = dx;
				dx = pdx;
				pdx = t;
			}
			if (pdx > 0 || dx < 0) {
				return dx + prev_silbing->bounds().right();
			}
			else {
				return dx + pdx + prev_silbing->bounds().right();
			}
		}
		return dx;
	}

	int View::layoutY()
	{
		if (!parent_)
			return 0;
		int dy = marginTop();
		View* prev_silbing = prevSibling();
		while (prev_silbing && !prev_silbing->visible())
		{
			prev_silbing = prev_silbing->prevSibling();
		}
		if (parent_->IsVerticalLayout() && prev_silbing)
		{//������������ǰһ��
			int pdy = prev_silbing->marginTop();
			if (dy < pdy) {
				int t = dy;
				dy = pdy;
				pdy = t;
			}
			if (pdy > 0 || dy < 0) {
				return dy + prev_silbing->bounds().bottom();
			}
			else {
				return dy + pdy + prev_silbing->bounds().bottom();
			}
		}
		return dy;
	}

	int View::marginLeft() const
	{
		return GetMarginValue(Style_MarginLeft);
	}

	int View::marginTop() const
	{
		return GetMarginValue(Style_MarginTop);
	}

	int View::marginRight() const
	{
		return GetMarginValue(Style_MarginRight);
	}

	int View::marginBottom() const
	{
		return GetMarginValue(Style_MarginBottom);
	}


	int View::GetMarginValue(StyleProperty p) const
	{
		StyleValue* v = style_declarations_.FindValue(p);
		if (!v)
			return 0;
		StyleValueType type = v->GetType();
		if (type == StyleValue_Pixel) {
			return v->GetPixel();
		}

		if (type == StyleValue_Percent && parent_)	{
			if (p == Style_MarginLeft || p == Style_MarginRight)
				return parent_->GetContentsBounds().width() * v->GetPercentValue();
			else if (p == Style_MarginTop || p == Style_MarginBottom)
				return parent_->GetContentsBounds().height() * v->GetPercentValue();
		}
		return 0;
	}



	bool View::IsAbsouletLayout() const
	{
		return container_.get() && container_->layoutType() == AbsoulteLayout;
	}

	bool View::IsHorizonalLayout() const
	{
		return container_.get() && container_->layoutType() == HorizonalLayout;
	}

	bool View::IsVerticalLayout() const
	{
		return container_.get() && container_->layoutType() == VerticalLayout;
	}


	const char* View::tag() const
	{
		return "View";
	}

	void View::SetId(const std::string& id)
	{
		id_ = id;
	}

	const std::string& View::id() const
	{
		return id_;
	}

	void View::AddClass(const std::string& v)
	{
		classes_.insert(v);
	}

	void View::RemoveClass(const std::string& v)
	{
		classes_.erase(v);
	}

	bool View::HaveClass(const std::string& v) const
	{
		return classes_.count(v) > 0;
	}

	void View::ClearClass()
	{
		classes_.clear();
	}

	void View::GetClass(std::vector<std::string>& v) const
	{
		for (auto iter : classes_)
		{
			v.push_back(iter);
		}
	}

	void View::UpdateStyleRules(StyleSheetList* sheets)
	{
		//sheets->MatchRules(this, style_declarations_);

		{//backaground-color
			StyleValue* color = style_declarations_.FindValue(Style_BackgroundColor);
			if (color && color->IsColorValue())
			{
				if (!background_.get())
				{
					background_.reset(new Background);
				}
				background_->SetColor(color->GetColorValue());
			}
		}

		UpdateBorderStyle();
		
		//cursor

		////set into layout box
		

		if (container_.get())
			container_->UpdateStyleRule(sheets);


	}

	void View::UpdateBorderStyle()
	{
		const StyleProperty kBorderColor[] = {
			Style_BorderLeftColor,
			Style_BorderTopColor,
			Style_BorderRightColor,
			Style_BorderBottomColor
		};
		for (int i = 0; i < 4; i++)
		{
			StyleValue* color = style_declarations_.FindValue(kBorderColor[i]);
			if (color && color->IsColorValue())
			{
				if (!border_.get())
				{
					border_.reset(new Border);
				}
				border_->SetBorderColor((Border::Direction)(Border::LEFT + i), color->GetColorValue());
			}

			StyleValue* width = style_declarations_.FindValue(StyleProperty(kBorderColor[i] + 1));
			if (width && width->GetType() == StyleValue_Pixel)
			{
				if (!border_.get())
				{
					border_.reset(new Border);
				}
				border_->SetBorderWidth(Border::LEFT, width->GetPixel());
			}
		}
			
	}


	

	
#if 0
	void View::set_layout_width_policy(LayoutSizePolicy p)
	{
		width_policy_ = p;
	}

	void View::set_layout_height_policy(LayoutSizePolicy p)
	{
		height_policy_ = p;
	}

	ui::LayoutSizePolicy View::get_layout_width_policy() const
	{
		return width_policy_;
	}

	ui::LayoutSizePolicy View::get_layout_height_policy() const
	{
		return height_policy_;
	}
#endif

	




	

	

}