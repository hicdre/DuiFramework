#include "stdafx.h"
#include "ui_view.h"
#include "ui_view_controller.h"
#include "uikit/render/ui_render_context.h"

// #include "ui_include.h"
// 
// #include "render/render_include.h"
// #include "event/event_include.h"

namespace ui
{
	UIView::UIView()
	{

	}

	UIView::UIView(const Rect& rect)
		: bounds_(rect)
	{

	}

	UIView::~UIView()
	{
		RemoveChildren();
	}


	UIView* UIView::lastChild() const
	{
		return last_child_;
	}

	UIView* UIView::firstChild() const
	{
		return first_child_;
	}

	UIView* UIView::nextSibling() const
	{
		return next_sibling_;
	}

	UIView* UIView::previousSibling() const
	{
		return prev_sibling_;
	}

	bool UIView::HasChildren() const
	{
		return !!first_child_;
	}


	bool UIView::HasParent() const
	{
		return !!parent_;
	}


	UIView* UIView::parent() const
	{
		return parent_;
	}

	void UIView::AppendTo(UIView* parent)
	{
		parent_->Append(this);
	}

	void UIView::Detach()
	{
		if (parent_)
			parent_->Remove(this);
	}

	void UIView::Append(UIView* child)
	{
// 		if (child->document_ != document_) {
// 			assert(0);
// 			return;
// 		}

		if (child->parent_) {
			child->parent_->Unlink(child);
		}

		if (last_child_) {
			last_child_->next_sibling_ = child;
			child->prev_sibling_ = last_child_;
			last_child_ = child;
			child->next_sibling_ = NULL;
		}
		else {
			first_child_ = child;
			last_child_ = child;

			child->prev_sibling_ = NULL;
			child->next_sibling_ = NULL;
		}

		child->parent_ = this;
	}


	void UIView::Prepend(UIView* child)
	{
// 		if (child->document_ != document_) {
// 			assert(0);
// 			return;
// 		}

		if (child->parent_) {
			child->parent_->Unlink(child);
		}

		if (first_child_) {
			first_child_->prev_sibling_ = child;
			child->next_sibling_ = first_child_;
			first_child_ = child;
			child->prev_sibling_ = NULL;
		}
		else {
			first_child_ = child;
			last_child_ = child;

			child->prev_sibling_ = NULL;
			child->next_sibling_ = NULL;
		}

		child->parent_ = this;
	}

	void UIView::Unlink(UIView* child)
	{
		if (first_child_ == child) {
			first_child_ = first_child_->next_sibling_;
		}

		if (last_child_ == child) {
			last_child_ = last_child_->prev_sibling_;
		}

		if (child->prev_sibling_) {
			child->prev_sibling_->next_sibling_ = child->next_sibling_;
		}
		if (child->next_sibling_) {
			child->next_sibling_->prev_sibling_ = child->prev_sibling_;
		}
		child->parent_ = NULL;
	}

	void UIView::Remove(UIView* child)
	{
		//只允许移除子节点
		if (!child || child->parent_ != this)
		{
			assert(0); //<< "can only remove child node";
			return;
		}

		Unlink(child);

		child->prev_sibling_ = NULL;
		child->next_sibling_ = NULL;
	}

	void UIView::InsertAfterChild(UIView* ref, UIView* child)
	{
		if (ref == child || child == NULL || (ref && ref->parent_ != this))
		{
			assert(0);
			return;
		}

		if (!ref->next_sibling_) {
			Append(child);
			return;
		}

		if (child->parent_)
			child->parent_->Unlink(child);

		child->prev_sibling_ = ref;
		child->next_sibling_ = ref->next_sibling_;
		ref->next_sibling_->prev_sibling_ = child;
		ref->next_sibling_ = ref;

		child->parent_ = this;
	}

	void UIView::InsertBeforeChild(UIView* ref, UIView* child)
	{
		if (ref == child || child == NULL || (ref && ref->parent_ != this))
		{
			assert(0);// << "insert before invalid";
			return;
		}

		if (ref->prev_sibling_)
		{
			return InsertAfterChild(ref->prev_sibling_, child);
		}
		else
		{
			return Prepend(child);
		}
	}

	void UIView::RemoveChildren()
	{
		while (first_child_) {
			UIView* node = first_child_;

			Unlink(node);
		}
	}

// 	void UIView::setTag(const std::string& tag)
// 	{
// 		tag_ = tag;
// 	}
// 
// 	const std::string& UIView::getTag() const
// 	{
// 		return tag_;
// 	}
// 
// 	void UIView::setId(const std::string& id)
// 	{
// 		id_ = id;
// 	}
// 
// 	const std::string& UIView::getId() const
// 	{
// 		return id_;
// 	}
// 
// 	void UIView::addClass(const std::string& v)
// 	{
// 		classes_.insert(v);
// 	}
// 
// 	void UIView::removeClass(const std::string& v)
// 	{
// 		classes_.erase(v);
// 	}
// 
// 	bool UIView::haveClass(const std::string& v) const
// 	{
// 		return classes_.count(v) > 0;
// 	}
// 
// 	void UIView::clearClass()
// 	{
// 		classes_.clear();
// 	}


// 	void UIView::UpdateStyles()
// 	{
// 		if (!styles_.get()) {
// 			styles_.reset(new UIStyles(this));
// 		}
// 		StyleResultList new_results;
// 		document_->SelectStyles(this, &new_results);
// 		styles_->UpdateStyles(&new_results);
// 
// 		for (UIView* obj = firstChild(); obj; obj = obj->nextSibling())
// 		{
// 			obj->UpdateStyles();
// 		}
// 	}
// 
// 
// 	UIStyles* UIView::styles() const
// 	{
// 		return styles_.get();
// 	}

	void UIView::SchedulePaint()
	{
		SchedulePaintInRect(GetLocalBounds());
	}

	void UIView::SchedulePaintInRect(const Rect& r)
	{
		if (window_ && parent_) {
			parent_->OnChildSchedulePaintInRect(this, r);
		}
	}

	void UIView::OnChildSchedulePaintInRect(UIView* child, const Rect& r)
	{
		SchedulePaintInRect(ConvertRectFromChild(child, r));
	}

	Rect UIView::ConvertRectFromChild(UIView* child, const Rect& r)
	{
		Rect rect(GetLocalBounds());
		return Rect(r.x() + child->x() + rect.x(), r.y() + child->y() + rect.y(), r.width(), r.height());
	}

	void UIView::DoPaint(UIRenderContext* painter, const Rect& r)
	{
		UIScopedPainter helper(painter, Matrix(1.0, 0, 0, 1.0, r.x(), r.y()));
		OnPaint(painter);

		for (UIView* obj = firstChild(); obj; obj = obj->nextSibling())
		{
			if (obj->isVisible())
				obj->DoPaint(painter, obj->bounds());
		}
	}


	void UIView::OnPaint(UIRenderContext* painter)
	{
		if (background_color_ == Color_Transparent)
			return;
		painter->FillRect(GetLocalBounds(), background_color_);
// 		Rect rect(GetLocalBounds());
// 		UIBorderPainter borderPainter(rect, styles()->borders(), painter);
// 
// 		{
// 			ScopedClipper clipper(painter, borderPainter.ClipPath().get());
// 			PaintBackground(painter);
// 		}
// 		borderPainter.Paint();
// 
// 		{
// 			Rect rc(GetContentBounds());
// 			ScopedPainter helper(painter, Matrix(1.0, 0, 0, 1.0, rc.x(), rc.y()));
// 			PaintContents(painter);
// 		}
	}


// 	void UIView::PaintBackground(RenderContext* painter)
// 	{
// 		//ScopedClipper clipper(painter, styles()->borders()->CreateClipRgn());
// 		//Color color = styles()->backgroundColor();
// 		//painter->FillRect(GetLocalBounds(), color);
// 		const UIStyleBackground* background = styles()->background();
// 		painter->FillRect(GetLocalBounds(), background->color());
// 
// 		{
// 			const RenderImage* image = background->image();
// 			if (image) {
// 				const Rect& rc = background->source_rect();
// 				painter->DrawImage(image, rc, GetLocalBounds());
// 			}
// 		}
// 	}
// 
// 	void UIView::PaintContents(RenderContext* painter)
// 	{
// 		for (UIView* obj = firstChild(); obj; obj = obj->nextSibling())
// 		{
// 			obj->DoPaint(painter, obj->bounds());
// 		}
// 	}


	void UIView::Layout()
	{
		//默认不需要进行布局
//		//layout之前，位置已经确定
//		//计算宽度, 宽度只与父有关
// 		UIStyles* style = styles();
// 		if (style->autoWidth() && HasParent()) {
// 			UIView* p(parent());
// 			Rect rc(p->GetContentBounds());
// 			bounds_.set_width(rc.right() - style->marginRight());
// 		}
// 		else {
// 			bounds_.set_width(style->pixelWidth());
// 		}
// 
// 		UIView* bottom_obj = NULL;
		for (UIView* obj = firstChild(); obj; obj = obj->nextSibling())
		{
			obj->LayoutIfNeeded();
		}
// 
// 		if (style->autoHeight()) {
// 			bounds_.set_height(parent()->bounds().bottom());
// 		}
// 		else {
// 			bounds_.set_height(style->pixelHeight());
// 		}

		needs_layout_ = false;
		child_needs_layout_ = false;
	}

	void UIView::LayoutIfNeeded()
	{
		if (needs_layout_) {
			Layout();
		}
	}


	void UIView::SetBounds(int x, int y, int width, int height)
	{
		SetBoundsRect(Rect(x, y, (std::max)(0, width), (std::max)(0, height)));
	}

	void UIView::SetBoundsRect(const Rect& bounds)
	{
		if (bounds == bounds_) {
			return;
		}

		Rect prev = bounds_;
		bounds_ = bounds;
		if (prev.origin() != bounds_.origin()) {
			OnPositionChanged();
		}
		if (prev.size() != size()) {
			OnBoundsChanged();
		}
	}

	void UIView::SetSize(const Size& size)
	{
		SetBounds(x(), y(), size.width(), size.height());
	}

	void UIView::SetPosition(const Point& position)
	{
		SetPosition(position.x(), position.y());
	}

	void UIView::SetPosition(int x, int y)
	{
		SetBounds(x, y, width(), height());
	}

	void UIView::SetX(int x)
	{
		SetBounds(x, y(), width(), height());
	}

	void UIView::SetY(int y)
	{
		SetBounds(x(), y, width(), height());
	}

	Rect UIView::GetLocalBounds() const
	{
		return Rect(width(), height());
	}

// 	Rect UIView::GetContentBounds() const
// 	{
// 		Rect rc(GetLocalBounds());
// 		rc.Inset(styles()->borders()->GetInseting());
// 		return rc;
// 	}
// 
// 	int UIView::marginLeft() const
// 	{
// 		return styles_->marginLeft();
// 	}
// 
// 	int UIView::marginTop() const
// 	{
// 		return styles_->marginTop();
// 	}
// 
// 	int UIView::marginRight() const
// 	{
// 		return styles_->marginRight();
// 	}
// 
// 	int UIView::marginBottom() const
// 	{
// 		return styles_->marginBottom();
// 	}

	void UIView::OnBoundsChanged()
	{
		needs_layout_ = true;
		if (parent())
			parent()->OnChildBoundsChanged(this);
		else
			Layout();
	}


	void UIView::OnPositionChanged()
	{

	}


	void UIView::OnChildBoundsChanged(UIView* child)
	{
		child->Layout();
	}

// 	bool UIView::Hittest(HitTestResult* result, const Point& pt)
// 	{
// 		if (!bounds().Contains(pt))
// 			return false;
// 		
// 		Point location(pt.x() - x(), pt.y() - y());
// 		result->Add(this, location);
// 
// 		Rect rc(GetContentBounds());
// 		location.Offset(-rc.x(), -rc.y());
// 		for (UIView* obj = lastChild(); obj; obj = obj->previousSibling())
// 		{
// 			obj->Hittest(result, location);
// 		}
// 		return true;
// 	}

// 	void UIView::HandleLocalEvents(Event* event)
// 	{
// 		//if (!HasEventTargetData())
// 		//	return;
// 
// 		//if (isDisabledFormControl(this) && event->isMouseEvent())
// 		//	return;
// 
// 		FireEventListeners(event);
// 	}

	Point UIView::ConvertPointToView(UIView* elem, const Point& pt)
	{
		Point result(pt);
		UIView* p = elem;
		while (p->parent())
		{
			result.Offset(p->x(), p->y());
			p = p->parent();
		}
		return result;
	}

// 	bool UIView::DispatchEvent(Event* event, EventPath* path /*= NULL*/)
// 	{
// 		scoped_ptr<EventPath> eventPath(path ? path : new EventPath(this));
// 		event->setEventPath(eventPath.release());
// 		return EventDispatcher::DispatchEvent(this, event);
// 	}

// 	void UIView::SetHovered(bool v)
// 	{
// 		if (!handle_mouse_event_ || v == hovered_)
// 			return;
// 		hovered_ = v;
// 		UpdateStyles();
// 	}
// 
// 	void UIView::SetActiveOrFocused(bool v)
// 	{
// 		if (v == actived_or_focused_)
// 			return;
// 		actived_or_focused_ = v;
// 		UpdateStyles();
// 	}

	void UIView::Init()
	{
		InitWithBounds(Rect());
	}


	void UIView::InitWithBounds(const Rect& rect)
	{
		UIResponder::Init();
		bounds_ = rect;
	}


	UIResponder* UIView::NextResponder() const
	{
		if (controller_)
			return controller_;
		return parent();
	}

	void UIView::SetBackgroundColor(Color color)
	{
		if (color == background_color_)
			return;
		background_color_ = color;
		SchedulePaint();
	}

	void UIView::SetVisible(bool v)
	{
		if (v == visible_)
			return;
		visible_ = v;
		OnVisibleChanged();
		SchedulePaint();
	}

	void UIView::OnVisibleChanged()
	{

	}

	UIView* UIView::HitTestWithEvent(const Point& pt, UIEvent* event)
	{
		if (!PointInsideWithEvent(pt, event))
			return NULL;

		for (UIView* obj = firstChild(); obj; obj = obj->nextSibling())
		{
			UIView* result = obj->HitTestWithEvent(Point(pt.x() - obj->x(),
								pt.y() - obj->y()), event);
			if (result)
				return result;
		}
		return this;
	}

	bool UIView::PointInsideWithEvent(const Point& pt, UIEvent* event)
	{
		//判断disable

		//判断alpha

		if(!GetLocalBounds().Contains(pt))
			return false;


		return true;
	}

	UIWindow* UIView::window() const
	{
		return window_;
	}

	void UIView::addSubView(UIView *view)
	{
		UIWindow* w = window();
		view->doWillMoveToWindow(w);

		Append(view);
		view->setWindow(w);

		view->doDidMoveToWindow(w);
	}

	void UIView::removeFromParent()
	{
		UIWindow* w = window();
		doWillMoveToWindow(NULL);

		Detach();
		setWindow(NULL);
		doDidMoveToWindow(NULL);
	}

	void UIView::insertSubViewAfter(UIView* view, UIView* viewAfter)
	{
		UIWindow* w = window();
		view->doWillMoveToWindow(w);

		InsertAfterChild(viewAfter, view);
		view->setWindow(w);

		view->doDidMoveToWindow(w);
	}

	void UIView::insertSubViewBefore(UIView* view, UIView* viewBefore)
	{
		UIWindow* w = window();
		view->doWillMoveToWindow(w);

		InsertBeforeChild(viewBefore, view);
		view->setWindow(w);

		view->doDidMoveToWindow(w);
	}


	void UIView::doWillMoveToWindow(UIWindow* w)
	{
		for (UIView* obj = firstChild(); obj; obj = obj->nextSibling())
		{
			obj->doWillMoveToWindow(w);
		}
	}

	void UIView::doDidMoveToWindow(UIWindow* w)
	{
		for (UIView* obj = firstChild(); obj; obj = obj->nextSibling())
		{
			obj->doDidMoveToWindow(w);
		}
	}

	void UIView::setWindow(UIWindow* window)
	{
		window_ = window;
		for (UIView* obj = firstChild(); obj; obj = obj->nextSibling())
		{
			obj->setWindow(window_);
		}
	}

	void UIView::willMoveToWindow(UIWindow* window)
	{

	}

	void UIView::didMoveToWindow(UIWindow* window)
	{

	}

	// 	void UIView::SetText(UIText* text)
// 	{
// 		text_.reset(text);
// 	}

	

}