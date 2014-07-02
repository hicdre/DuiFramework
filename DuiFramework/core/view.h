#pragma once
#include "base/basictypes.h"
#include "base/scoped_ptr.h"
#include "render/rect.h"
#include "render/size.h"
#include "render/point.h"
#include "render/transform.h"
#include "render/painter.h"
#include "render/border.h"
#include "render/background.h"
#include <vector>

namespace ui
{
	class Widget;
	class View
	{
	public:
		View();
		virtual ~View();

		typedef std::vector<View*> Views;

		// Tree operations -----------------------------------------------------------
		View* parent() const;
		View* first_child() const;
		View* last_child() const;
		View* prev_sibling() const;
		View* next_sibling() const;

		View* root() const;

		void GetViews(Views &child_array) const;
		int32 GetViewCount() const;

		View* AppendTo(View* parent);
		View* Detach();

		View* Append(View* child);
		View* Remove(View* child);
		View* InsertAfter(View* ref, View* child);
		View* InsertBefore(View* ref, View* child);


		// Get the Widget that hosts this View, if any.
		virtual const Widget* GetWidget() const;
		virtual Widget* GetWidget();
		void SetOwnedWidget(Widget* w);


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

		void SetVisible(bool visible);
		bool visible() const { return visible_; }
		bool IsDrawn() const;
		void SetEnabled(bool enabled);
		bool enabled() const { return enabled_; }

		// Layout --------------------------------------------------------------------
		virtual void Layout();

		// Painting ------------------------------------------------------------------
		void set_background_color(Color color);
		void set_background_image_id(const std::string& id);
		void SetBackground(Background* background);
		
		virtual void SchedulePaint();
		virtual void SchedulePaintInRect(const Rect& r);

		void DoPaint(Painter* painter);
		virtual void OnPaint(Painter* painter);

		// Coordinate conversion -----------------------------------------------------
		Transform GetTransform() const;

		// Convert a point from the coordinate system of one View to another.
		//
		// |source| and |target| must be in the same widget, but doesn't need to be in
		// the same view hierarchy.
		// |source| can be NULL in which case it means the screen coordinate system.
		//static void ConvertPointToView(const View* source, const View* target, Point* point);
		// Convert a point from a View's coordinate system to that of its Widget.
		//static void ConvertPointToWidget(const View* src, Point* point);
		// Convert a point from the coordinate system of a View's Widget to that
		// View's coordinate system.
		//static void ConvertPointFromWidget(const View* dest, Point* p);
		// Convert a point from a View's coordinate system to that of the screen.
		//static void ConvertPointToScreen(const View* src, Point* point);
		// Convert a point from a View's coordinate system to that of the screen.
		//static void ConvertPointFromScreen(const View* dst, Point* point);
		// Applies transformation on the rectangle, which is in the view's coordinate
		// system, to convert it into the parent's coordinate system.
		Rect ConvertRectToParent(const Rect& rect) const;
		// Converts a rectangle from this views coordinate system to its widget
		// coordinate system.
		Rect ConvertRectToWidget(const Rect& rect) const;

		// Attributes---------------------------------------------
		void SetBorder(Border* border);
		Border* border() const;

	public:
		virtual void OnVisibleChanged();
		virtual void OnEnabledChanged();

	protected:
		// Coordinate conversion -----------------------------------------------------
		//bool ConvertPointForAncestor(const View* ancestor, Point* point) const;
		//bool ConvertPointFromAncestor(const View* ancestor, Point* point) const;
		void PaintBackground(Painter* painter);
		void PaintBorder(Painter* painter);

		View* parent_{ NULL };
		View* first_child_{ NULL };
		View* last_child_{ NULL };
		View* next_sibling_{ NULL };
		View* prev_sibling_{ NULL };
		int32 child_count_{ 0 };

		Widget* owned_widget_{ NULL };

		// This View's bounds in the parent coordinate system.
		Rect bounds_;
		bool visible_{ true };// Whether this view is visible.
		bool enabled_{ true };// Whether this view is enabled.
		bool painting_enabled_{ true };// Whether this view is painting.

		bool needs_layout_{ true };

		//Color background_color_{0xFFFFFFFF};

		scoped_ptr<Border> border_;
		scoped_ptr<Background> background_{ new NormalBackground(0xFFFFFFFF) };
	};
}