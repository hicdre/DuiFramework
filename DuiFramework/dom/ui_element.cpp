#include "stdafx.h"
#include "ui_element.h"
#include "ui_include.h"

#include "render/render_include.h"
#include "event/event_include.h"

namespace ui
{
	UIElement::UIElement(UIDocumentPtr doc)
		: document_(doc)
		, styles_(new UIStyles(this))
	{

	}

	UIElement::~UIElement()
	{
		RemoveChildren();
	}

	UIDocumentPtr UIElement::GetDocument()
	{
		return document_;
	}

	UIElementPtr UIElement::lastChild() const
	{
		return last_child_;
	}

	UIElementPtr UIElement::firstChild() const
	{
		return first_child_;
	}

	UIElementPtr UIElement::nextSibling() const
	{
		return next_sibling_;
	}

	UIElementPtr UIElement::previousSibling() const
	{
		return prev_sibling_;
	}

	bool UIElement::HasChildren() const
	{
		return !!first_child_;
	}


	bool UIElement::HasParent() const
	{
		return !!parent_;
	}


	UIElementPtr UIElement::parent()
	{
		return parent_;
	}

	void UIElement::AppendTo(UIElementPtr parent)
	{
		parent_->Append(this);
	}

	void UIElement::Detach()
	{
		if (parent_)
			parent_->Remove(this);
	}

	void UIElement::Append(UIElementPtr child)
	{
		if (child->document_ != document_) {
			assert(0);
			return;
		}

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

		child->parent_ = UIElementPtr(this);
	}


	void UIElement::Prepend(UIElementPtr child)
	{
		if (child->document_ != document_) {
			assert(0);
			return;
		}

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

		child->parent_ = UIElementPtr(this);
	}

	void UIElement::Unlink(UIElementPtr child)
	{
		if (first_child_.get() == child.get()) {
			first_child_ = first_child_->next_sibling_;
		}

		if (last_child_.get() == child.get()) {
			last_child_ = last_child_->prev_sibling_;
		}

		if (child->prev_sibling_) {
			child->prev_sibling_->next_sibling_ = child->next_sibling_;
		}
		if (child->next_sibling_) {
			child->next_sibling_->prev_sibling_ = child->prev_sibling_;
		}
		child->parent_.reset();
	}

	void UIElement::Remove(UIElementPtr child)
	{
		//只允许移除子节点
		if (!child || child->parent_.get() != this)
		{
			assert(0); //<< "can only remove child node";
			return;
		}

		Unlink(child);

		child->prev_sibling_ = NULL;
		child->next_sibling_ = NULL;
	}

	void UIElement::InsertAfterChild(UIElementPtr ref, UIElementPtr child)
	{
		if (ref == child || child == NULL || (ref && ref->parent_.get() != this))
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

		child->parent_ = UIElementPtr(this);
	}

	void UIElement::InsertBeforeChild(UIElementPtr ref, UIElementPtr child)
	{
		if (ref == child || child.get() == NULL || (ref && ref->parent_.get() != this))
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

	void UIElement::RemoveChildren()
	{
		while (first_child_) {
			UIElementPtr node = first_child_;

			Unlink(node);
		}
	}

	void UIElement::setTag(const std::string& tag)
	{
		tag_ = tag;
	}

	const std::string& UIElement::getTag() const
	{
		return tag_;
	}

	void UIElement::setId(const std::string& id)
	{
		id_ = id;
	}

	const std::string& UIElement::getId() const
	{
		return id_;
	}

	void UIElement::addClass(const std::string& v)
	{
		classes_.insert(v);
	}

	void UIElement::removeClass(const std::string& v)
	{
		classes_.erase(v);
	}

	bool UIElement::haveClass(const std::string& v) const
	{
		return classes_.count(v) > 0;
	}

	void UIElement::clearClass()
	{
		classes_.clear();
	}


	void UIElement::UpdateStyles()
	{
		if (!styles_.get()) {
			styles_.reset(new UIStyles(this));
		}
		StyleResultList new_results;
		document_->SelectStyles(this, &new_results);
		styles_->UpdateStyles(&new_results);

		for (UIElementPtr obj = firstChild(); obj; obj = obj->nextSibling())
		{
			obj->UpdateStyles();
		}
	}


	UIStyles* UIElement::styles() const
	{
		return styles_.get();
	}

	void UIElement::SchedulePaint()
	{
		SchedulePaintInRect(GetLocalBounds());
	}

	void UIElement::SchedulePaintInRect(const Rect& r)
	{
		if (parent_) {
			parent_->OnChildSchedulePaintInRect(this, r);
		}
	}

	void UIElement::OnChildSchedulePaintInRect(UIElement* child, const Rect& r)
	{
		SchedulePaintInRect(ConvertRectFromChild(child, r));
	}

	Rect UIElement::ConvertRectFromChild(UIElement* child, const Rect& r)
	{
		Rect rect(GetContentBounds());//加上border的偏移
		return Rect(r.x() + child->x() + rect.x(), r.y() + child->y() + rect.y(), r.width(), r.height());
	}

	void UIElement::DoPaint(RenderContext* painter, const Rect& r)
	{
		ScopedPainter helper(painter, Matrix(1.0, 0, 0, 1.0, r.x(), r.y()));

		PaintBackground(painter);
		PaintBorder(painter);
		PaintContents(painter);
	}

	void UIElement::PaintBackground(RenderContext* painter)
	{
		Color color = styles()->backgroundColor();
		painter->FillRect(GetLocalBounds(), color);
	}

	void UIElement::PaintBorder(RenderContext* painter)
	{
		const UIBorder* borders = styles()->borders();
		Rect rect(GetLocalBounds());
		int left_length = rect.height() - borders->leftTopRadius() - borders->leftBottomRadius();
		int top_length = rect.width() - borders->leftTopRadius() - borders->rightTopRadius();
		int right_length = rect.height() - borders->rightTopRadius() - borders->rightBottomRadius();
		int bottom_length = rect.width() - borders->leftBottomRadius() - borders->rightBottomRadius();

		if (borders->left().color != Color_Transparent && left_length > 0) {
			painter->FillRect(Rect(0, borders->leftTopRadius(), borders->left().size, left_length),
				borders->left().color);
		}

		//圆角, 先简单处理
		if (borders->leftTopRadius())
		{
			painter->DrawArc(Rect(0, 0, borders->leftTopRadius(), borders->leftTopRadius()),
				180, 90, borders->left().color, borders->left().size);
		}

		if (borders->top().color != Color_Transparent && top_length > 0) {
			painter->FillRect(Rect(borders->leftTopRadius(), 0, top_length, borders->top().size),
				borders->top().color);
		}

		if (borders->rightTopRadius())
		{
			painter->DrawArc(Rect(0, 0, borders->rightTopRadius(), borders->rightTopRadius()),
				270, 90, borders->top().color, borders->top().size);
		}

		if (borders->right().color != Color_Transparent && right_length > 0) {
			painter->FillRect(Rect(rect.width() - borders->right().size, borders->rightTopRadius(), 
				borders->right().size, right_length),
				borders->right().color);
		}

		if (borders->rightBottomRadius())
		{
			painter->DrawArc(Rect(0, 0, borders->rightBottomRadius(), borders->rightBottomRadius()),
				0, 90, borders->right().color, borders->right().size);
		}

		if (borders->bottom().color != Color_Transparent && bottom_length > 0) {
			painter->FillRect(Rect(borders->leftBottomRadius(), rect.height() - borders->bottom().size, 
				bottom_length, borders->bottom().size),
				borders->bottom().color);
		}

		if (borders->leftBottomRadius())
		{
			painter->DrawArc(Rect(0, 0, borders->leftBottomRadius(), borders->leftBottomRadius()),
				90, 90, borders->bottom().color, borders->bottom().size);
		}


	}


	void UIElement::PaintContents(RenderContext* painter)
	{
		Rect rc(GetContentBounds());
		ScopedPainter helper(painter, Matrix(1.0, 0, 0, 1.0, rc.x(), rc.y()));

		for (UIElementPtr obj = firstChild(); obj; obj = obj->nextSibling())
		{
			obj->DoPaint(painter, obj->bounds());
		}
	}


	void UIElement::Layout()
	{
		//layout之前，位置已经确定
		//计算宽度, 宽度只与父有关
		UIStyles* style = styles();
		if (style->autoWidth()) {
			UIElementPtr p(parent());
			Rect rc(p->GetContentBounds());
			bounds_.set_width(rc.right() - style->marginRight());
		}
		else {
			bounds_.set_width(style->pixelWidth());
		}

		UIElement* bottom_obj = NULL;
		for (UIElementPtr obj = firstChild(); obj; obj = obj->nextSibling())
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
			bounds_.set_height(parent()->bounds().bottom());
		}
		else {
			bounds_.set_height(style->pixelHeight());
		}

		needs_layout_ = false;
		child_needs_layout_ = false;
	}

	void UIElement::LayoutIfNeeded()
	{
		if (needs_layout_) {
			Layout();
		}
	}


	void UIElement::SetBounds(int x, int y, int width, int height)
	{
		SetBoundsRect(Rect(x, y, (std::max)(0, width), (std::max)(0, height)));
	}

	void UIElement::SetBoundsRect(const Rect& bounds)
	{
		if (bounds == bounds_) {
			return;
		}

		Rect prev = bounds_;
		bounds_ = bounds;
		if (prev.size() != size()) {
			OnBoundsChanged();
		}
	}

	void UIElement::SetSize(const Size& size)
	{
		SetBounds(x(), y(), size.width(), size.height());
	}

	void UIElement::SetPosition(const Point& position)
	{
		SetBounds(position.x(), position.y(), width(), height());
	}

	void UIElement::SetPosition(int x, int y)
	{
		SetBounds(x, y, width(), height());
	}

	void UIElement::SetX(int x)
	{
		SetBounds(x, y(), width(), height());
	}

	void UIElement::SetY(int y)
	{
		SetBounds(x(), y, width(), height());
	}

	Rect UIElement::GetLocalBounds() const
	{
		return Rect(width(), height());
	}

	Rect UIElement::GetContentBounds() const
	{
		Rect rc(GetLocalBounds());
		rc.Inset(styles()->borders()->GetPadding());
		return rc;
	}

	int UIElement::marginLeft() const
	{
		return styles_->marginLeft();
	}

	int UIElement::marginTop() const
	{
		return styles_->marginTop();
	}

	int UIElement::marginRight() const
	{
		return styles_->marginRight();
	}

	int UIElement::marginBottom() const
	{
		return styles_->marginBottom();
	}

	void UIElement::OnBoundsChanged()
	{
		needs_layout_ = true;
		if (parent())
			parent()->OnChildBoundsChanged(this);
		else
			Layout();
	}

	void UIElement::OnChildBoundsChanged(UIElement* child)
	{
		child->Layout();
	}

	bool UIElement::Hittest(HitTestResult* result, const Point& pt)
	{
		if (!bounds().Contains(pt))
			return false;

		Point location(pt.x() - x(), pt.y() - y());
		result->Add(this, location);

		for (UIElementPtr obj = lastChild(); obj; obj = obj->previousSibling())
		{
			obj->Hittest(result, location);
		}
		return true;
	}

	void UIElement::HandleLocalEvents(Event* event)
	{
		//if (!HasEventTargetData())
		//	return;

		//if (isDisabledFormControl(this) && event->isMouseEvent())
		//	return;

		FireEventListeners(event);
	}

	Point UIElement::ConvertPointToElement(UIElement* elem, const Point& pt)
	{
		Point result(pt);
		UIElement* p = elem;
		while (p->parent())
		{
			result.Offset(p->x(), p->y());
			p = p->parent().get();
		}
		return result;
	}

	bool UIElement::DispatchEvent(Event* event, EventPath* path /*= NULL*/)
	{
		scoped_ptr<EventPath> eventPath(path ? path : new EventPath(this));
		event->setEventPath(eventPath.release());
		return EventDispatcher::DispatchEvent(this, event);
	}

	void UIElement::SetHovered(bool v)
	{
		if (v == hovered_)
			return;
		hovered_ = v;
		UpdateStyles();
	}


}