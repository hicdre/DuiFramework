#pragma once
#include "base/basictypes.h"
#include "dom/dom_forward.h"
#include "render/render_forward.h"

namespace ui
{
	/*
		RenderTree
	*/
	class RenderObject : public RefCounted<RenderObject>
	{
	public:
		RenderObject(UIElement* node);
		virtual ~RenderObject();

		static RenderObject* Create(UIElement* elem, RenderStyles* s);

		RenderObject* parent() const;
		bool isDescendantOf(const RenderObject*) const;

		UIElement* GetUIElement();

		RenderObject* firstChild() const;
		RenderObject* lastChild() const;
		RenderObject* previousSibling() const;
		RenderObject* nextSibling() const;

		virtual void addChild(RenderObject* child);
		virtual void addChildAfter(RenderObject* beforechild, RenderObject* child);
		virtual void removeChild(RenderObject* child);

		void SetBounds(int x, int y, int width, int height);
		void SetBoundsRect(const Rect& bounds);
		void SetSize(const Size& size);
		void SetPosition(const Point& position);
		void SetPosition(int x, int y);
		void SetX(int x);
		void SetY(int y);

		// No transformation is applied on the size or the locations.
		const Rect& bounds() const { return bounds_; }
		int x() const { return bounds_.x(); }
		int y() const { return bounds_.y(); }
		int width() const { return bounds_.width(); }
		int height() const { return bounds_.height(); }
		const Size& size() const { return bounds_.size(); }
		Rect GetLocalBounds() const;

		virtual void SchedulePaint();
		virtual void SchedulePaintInRect(const Rect& r);
		virtual void OnChildSchedulePaintInRect(RenderObject* child, const Rect& r);
		virtual void OnBoundsChanged();

		virtual void DoPaint(RenderContext* painter, const Rect& r) = 0;
		virtual void Layout() = 0;
		virtual void LayoutIfNeeded() = 0;

		RenderStyles* styles() const;
	protected:
		RenderObject* Append(RenderObject* child);
		RenderObject* Remove(RenderObject* child);
		RenderObject* InsertAfter(RenderObject* ref, RenderObject* child);
		RenderObject* InsertBefore(RenderObject* ref, RenderObject* child);

		Rect ConvertRectFromChild(RenderObject* child, const Rect& r);

		RenderObjectPtr parent_;
		RenderObjectPtr first_;
		RenderObjectPtr last_;
		RenderObjectPtr prev_;
		RenderObjectPtr next_;

		Rect bounds_; //  µº Œª÷√

		UIElementPtr node_;

		RenderStyles* styles_;

		
	};
}