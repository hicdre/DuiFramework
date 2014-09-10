#include "stdafx.h"
#include "view.h"

#include <algorithm>
#include <cassert>

#include "core/rectangle.h"
#include "core/image.h"
#include "core/container.h"

namespace ui
{

	View::View()
	{

	}

	View::~View()
	{
		//EventListenManager::Default()->RemoveView(this);
	}

	Container* View::parent() const
	{
		return parent_;
	}

	View* View::prev_sibling() const
	{
		return prev_sibling_;
	}

	View* View::next_sibling() const
	{
		return next_sibling_;
	}

	Container* View::root() const
	{
		const View* p = this;
		while (p->parent_)
			p = p->parent_;
		if (!p)
			return NULL;
		return const_cast<Container*>(static_cast<const Container*>(p));
	}

	View* View::AppendTo(Container* parent)
	{
		return parent->Append(this);
	}

	View* View::Detach()
	{
		return parent_->Remove(this);
	}

	View* View::GetAncestorTo(View* other)
	{
		Container* root_view = root();
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
		//然后同时向前查找
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


	View* View::Hittest(const Point& pt)
	{
		//hittest self
		if (!GetLocalBounds().Contains(pt))
			return NULL;
		return this;
	}


	//const Widget* View::GetWidget() const
	//{
	//	return parent_ ? parent_->GetWidget() : NULL;
	//}

	//Widget* View::GetWidget()
	//{
	//	return const_cast<Widget*>(const_cast<const View*>(this)->GetWidget());
	//}

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
			//if (parent_)
			//	parent_->ChildVisibilityChanged(this);

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
	}

	Size View::GetPreferredSize() const
	{
		return Size();
	}

	void View::OnVisibleChanged()
	{

	}

	void View::OnEnabledChanged()
	{

	}


	void View::SchedulePaint()
	{
		SchedulePaintInRect(GetLocalBounds());
	}

	void View::SchedulePaintInRect(const Rect& r)
	{
		if (!visible_ || !painting_enabled_)
			return;

		if (parent_) {
			// Translate the requested paint rect to the parent's coordinate system
			// then pass this notification up to the parent.
			parent_->SchedulePaintInRect(ConvertRectToParent(r));
		}
	}

	void View::DoPaint(Painter* painter, const Rect& dest)
	{
		if (!visible_)
			return;

		//ScopedPainter helper(painter, GetTransform());
		ScopedPainter helper(painter, Transform(1.0, 0, 0, 1.0, dest.x(), dest.y()));
		DoPaintSelf(painter);
	}

	void View::DoPaintSelf(Painter* painter)
	{
		PaintBackground(painter);
		PaintBorder(painter);
		OnPaint(painter);
	}

	void View::OnPaint(Painter* painter)
	{
		//painter->DrawStringRect(L"测试", Font(L"微软雅黑", 18), ColorSetRGB(255,0,0,0), GetLocalBounds());
	}

	Transform View::GetTransform() const
	{
		return Transform(1.0, 0, 0, 1.0, x(), y());
	}


	bool View::GetTransformRelativeTo(const View* ancestor, Transform* transform) const
	{
		const View* p = this;

		while (p && p != ancestor) {
			transform->ConcatTransform(p->GetTransform());
			p = p->parent_;
		}

		return p == ancestor;
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

	}*/

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

	void View::PaintBackground(Painter* painter)
	{
		if (!background_.get())
			return;

		background_->DoPaint(painter, background_inside_ ? 
			GetContentsBounds() : GetLocalBounds());
	}

	void View::SetBorder(Border* border)
	{
		border_.reset(border);
	}

	Border* View::border() const
	{
		return border_.get();
	}

	void View::PaintBorder(Painter* painter)
	{
		if (!border_.get())
			return;

		border_->DoPaint(painter, GetLocalBounds());
	}

	void View::SetBackground(View* background)
	{
		background_.reset(background);
	}

	void View::set_background_color(Color color)
	{
		if (!background_.get()) {
			background_.reset(new SolidRectangle(color));
		}

		SolidRectangle* normal_background
			= dynamic_cast<SolidRectangle*>(background_.get());
		if (!normal_background)
			return;

		normal_background->SetColor(color);
	}

	void View::set_background_image_id(const std::string& id)
	{
		if (id.empty())
			return;

		if (!background_.get()) {
			background_.reset(new ResourceImage(id));
		}

		ResourceImage* normal_background
			= dynamic_cast<ResourceImage*>(background_.get());
		if (!normal_background)
			return;

		normal_background->SetImageId(id);
	}


	void View::set_background_inside(bool v)
	{
		background_inside_ = v;
	}

	void View::SetCursor(HCURSOR cursor)
	{
		cursor_ = cursor;
	}


	HCURSOR View::GetCursor()
	{
		return cursor_ ? cursor_ : parent_->GetCursor();
	}

	

	void View::ConvertPointToTarget(View* source, View* target, Point* pt)
	{
		if (source == target)
			return;

		View* ancestor = source->GetAncestorTo(target);

		source->ConvertPointForAncestor(ancestor, pt);
		target->ConvertPointFromAncestor(ancestor, pt);
	}

	

	

	

	

	bool View::ConvertPointForAncestor(const View* ancestor, Point* point) const
	{
		Transform trans;
		bool result = GetTransformRelativeTo(ancestor, &trans);
		trans.TransformPoint(*point);
		return result;
	}

	bool View::ConvertPointFromAncestor(const View* ancestor, Point* point) const
	{
		Transform trans;
		bool result = GetTransformRelativeTo(ancestor, &trans);
		trans.Invert().TransformPoint(*point);
		return result;
	}

	void View::HandleEvent(Event* event)
	{
		if (!enabled_ && event->IsMouseEvent())
		{
			RouteEventTo(event, parent_);
			return;
		}

		if ((!enabled_ || !focusable_) && event->IsKeyEvent())
			return;

		event_listener_.HandleEvent(event);
		if (event->stopped_dispatch())
			return;

		bool route_to_parent = false;
		switch (event->type())
		{
		case EVENT_MOUSE_MOVE:
			OnMouseMove(static_cast<MouseEvent*>(event));
			route_to_parent = true;
			break;
		case EVENT_MOUSE_DOWN:
			OnMouseDown(static_cast<MouseEvent*>(event));
			route_to_parent = true;
			break;
		case EVENT_MOUSE_UP:
			OnMouseUp(static_cast<MouseEvent*>(event));
			route_to_parent = true;
			break;
		case EVENT_MOUSE_DBCLICK:
			OnMouseDoubleClick(static_cast<MouseEvent*>(event));
			route_to_parent = true;
			break;
		case EVENT_MOUSE_ENTER:
			OnMouseEnter(static_cast<MouseEvent*>(event));
			break;
		case EVENT_MOUSE_LEAVE:
			OnMouseLeave(static_cast<MouseEvent*>(event));
			break;
		case EVENT_KEY_PRESSED:
			OnKeyPressed(static_cast<KeyEvent*>(event));
			break;
		case EVENT_KEY_RELEASED:
			OnKeyReleased(static_cast<KeyEvent*>(event));
			break;
		case EVENT_CHAR:
			route_to_parent = false;
			break;
		case EVENT_LOSE_FOCUS:
			OnLoseFocus(static_cast<FocusEvent*>(event));
			break;
		case EVENT_GAIN_FOCUS:
			OnGainFocus(static_cast<FocusEvent*>(event));
			break;
		default:
			route_to_parent = true;
			break;
		}

		if (event->stopped_dispatch())
			return;

		if (route_to_parent && parent_)
			RouteEventTo(event, parent_);
	}


	void View::RouteEventTo(Event* event, View* v)
	{
		if (v)
			v->HandleEvent(event);
	}


	void View::RequestSetFocus()
	{
		if (!focusable_)
			return;
		FocusManager* manager = GetFocusManager();
		if (manager) {
			manager->SetFocus(this);
		}
	}

	void View::SetFocusable(bool focusable)
	{
		focusable_ = focusable;
	}

	bool View::IsFocusable() const
	{
		return focusable_;
	}

	FocusManager* View::GetFocusManager() const
	{
		return parent_ ? parent_->GetFocusManager() : NULL;
	}

	void View::OnMouseMove(MouseEvent* evt)
	{

	}

	void View::OnMouseDown( MouseEvent* evt )
	{

	}

	void View::OnMouseUp( MouseEvent* evt )
	{

	}


	void View::OnMouseDoubleClick(MouseEvent* evt)
	{

	}


	void View::OnMouseEnter( MouseEvent* evt )
	{

	}

	void View::OnMouseLeave( MouseEvent* evt )
	{

	}

	void View::OnKeyPressed( KeyEvent* evt )
	{

	}

	void View::OnKeyReleased( KeyEvent* evt )
	{

	}

	void View::OnLoseFocus(FocusEvent* evt)
	{

	}

	void View::OnGainFocus( FocusEvent* evt )
	{

	}

	void View::SetDragable(bool dragable)
	{
		dragable_ = dragable;
	}

	bool View::IsDragable() const
	{
		return dragable_;
	}

	void View::SetHittestOverride(const HittestOverride& f)
	{
		hittest_override_ = f;
	}

	const View::HittestOverride& View::GetHittestOverride() const
	{
		return hittest_override_;
	}

	bool View::HasHittestOverride() const
	{
		return !!hittest_override_;
	}

	bool View::is_container() const
	{
		return is_container_;
	}

	View* View::GetDragableView() const
	{
		if (dragable_)
			return const_cast<View*>(this);
		return parent_ ? parent_->GetDragableView() : NULL;
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