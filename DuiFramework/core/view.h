#pragma once
#include "base/basictypes.h"
#include "base/scoped_ptr.h"
#include "render/rect.h"
#include "render/size.h"
#include "render/point.h"
#include "render/transform.h"
#include "core/drawable.h"
#include "render/border.h"
#include "event/event.h"
#include "event/mouse_event.h"
#include "event/key_event.h"
#include "event/focus_evnet.h"
#include "core/focus_manager.h"
#include "core/property_object.h"
#include <vector>

namespace ui
{
	class Widget;
	class View;
	class Container;
	typedef std::vector<View*> Views;

	class View 
		: public PropertyObject
		, public EventHandler
		, public Drawable
	{
		friend class Container;
	public:
		View();
		virtual ~View();

		// Tree operations -----------------------------------------------------------
		Container* parent() const;
		
		View* prev_sibling() const;
		View* next_sibling() const;
		Container* root() const;

		View* AppendTo(Container* parent);
		View* Detach();
		
		View* GetAncestorTo(View* other);

		virtual View* Hittest(const Point& pt);//pt在本坐标系

		// Get the Widget that hosts this View, if any.
		//virtual const Widget* GetWidget() const;
		//virtual Widget* GetWidget();


		// Size and disposition ------------------------------------------------------
		
		void SetBounds(int x, int y, int width, int height);
		void SetBoundsRect(const Rect& bounds);
		void SetSize(const Size& size);
		void SetPosition(const Point& position);
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
		//去除border之后
		Rect GetContentsBounds() const;

		void SetVisible(bool visible);
		bool visible() const { return visible_; }
		bool IsDrawn() const;
		void SetEnabled(bool enabled);
		bool enabled() const { return enabled_; }
		void SetDragable(bool v);

		// Layout --------------------------------------------------------------------
		virtual void Layout();

		virtual Size GetPreferredSize() const;

		// Background ------------------------------------------------------------------
		void set_background_color(Color color);
		void set_background_image_id(const std::string& id);
		void set_background_inside(bool v);//true-在包含border的区域绘制;
		void SetBackground(View* background);
		
		// Painting ------------------------------------------------------------------
		virtual void SchedulePaint();
		virtual void SchedulePaintInRect(const Rect& r);

		virtual void DoPaint(Painter* painter, const Rect& dest) override;
		void DoPaintSelf(Painter* painter);
		virtual void OnPaint(Painter* painter);

		// Coordinate conversion -----------------------------------------------------
		Transform GetTransform() const;

		bool GetTransformRelativeTo(const View* ancestor,
			Transform* transform) const;

		// Convert a point from the coordinate system of source to target.
		Point ConvertPointFromParent(const Point& pt) const;
		Point ConvertPointFromWidget(const Point& pt) const;

		Point ConvertPointToParent(const Point& pt) const;
		Point ConvertPointToWidget(const Point& pt) const;
		Rect ConvertRectToParent(const Rect& rect) const;
		Rect ConvertRectToWidget(const Rect& rect) const;

		static void ConvertPointToTarget(View* source, View* target, Point* pt);

		// Attributes---------------------------------------------
		void SetBorder(Border* border);
		Border* border() const;

		void SetCursor(HCURSOR cursor);
		virtual HCURSOR GetCursor();

		

		//void set_layout_width_policy(LayoutSizePolicy p);
		//void set_layout_height_policy(LayoutSizePolicy p);

		//LayoutSizePolicy get_layout_width_policy() const;
		//LayoutSizePolicy get_layout_height_policy() const;

		// Event---------------------------------------------

		virtual void HandleEvent(Event* event) override;
		static void RouteEventTo(Event* event, View* v);

		EventListener& listener() { return event_listener_; }

		void RequestSetFocus();
		void SetFocusable(bool focusable);
		bool IsFocusable() const;

		virtual FocusManager* GetFocusManager() const;
		
	public:
		virtual void OnVisibleChanged();
		virtual void OnEnabledChanged();

		virtual void OnMouseMove(MouseEvent* evt);
		virtual void OnMouseDown(MouseEvent* evt);
		virtual void OnMouseUp(MouseEvent* evt);
		virtual void OnMouseDoubleClick(MouseEvent* evt);
		virtual void OnMouseEnter(MouseEvent* evt);
		virtual void OnMouseLeave(MouseEvent* evt);

		virtual void OnKeyPressed(KeyEvent* evt);
		virtual void OnKeyReleased(KeyEvent* evt);

		virtual void OnLoseFocus(FocusEvent* evt);
		virtual void OnGainFocus(FocusEvent* evt);

	protected:
		// Coordinate conversion -----------------------------------------------------
		bool ConvertPointForAncestor(const View* ancestor, Point* point) const;
		bool ConvertPointFromAncestor(const View* ancestor, Point* point) const;
		void PaintBackground(Painter* painter);
		void PaintBorder(Painter* painter);

		Container* parent_{ NULL };
		View* next_sibling_{ NULL };
		View* prev_sibling_{ NULL };
		


		// This View's bounds in the parent coordinate system.
		Rect bounds_;
		bool visible_{ true };// Whether this view is visible.
		bool enabled_{ true };// Whether this view is enabled.
		bool painting_enabled_{ true };// Whether this view is painting.

		bool needs_layout_{ true };

		bool focusable_{ false };

		//Color background_color_{0xFFFFFFFF};

		scoped_ptr<Border> border_;
		scoped_ptr<Drawable> background_;
		bool background_inside_{ false };

		HCURSOR cursor_{ NULL };

		EventListener event_listener_;

		bool enable_mouse_event_{ true };

		//LayoutSizePolicy width_policy_{ LAYOUT_SIZE_FIXED };
		//LayoutSizePolicy height_policy_{ LAYOUT_SIZE_FIXED };


		//scoped_ptr<EventDelegate> event_delegate_;
	};
}