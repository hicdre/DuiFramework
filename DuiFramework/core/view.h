#pragma once
#include "base/basictypes.h"
#include "base/scoped_ptr.h"
#include "render/rect.h"
#include "render/size.h"
#include "render/point.h"
#include "render/transform.h"
#include "render/painter.h"
#include "render/border.h"
#include "event/event.h"
#include "event/event_handler.h"
#include "core/focus_manager.h"
#include "core/value.h"
#include "layout/layout_manager.h"
#include <vector>

namespace ui
{
	class Widget;
	class View;
	typedef std::vector<View*> Views;

	class View
	{
	public:
		View();
		virtual ~View();

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

		void set_leaf_view(bool v);

		bool HasDescender(View* descender, int* step = NULL);
		View* GetAncestorTo(View* other);

		View* Hittest(const Point& pt);//pt在本坐标系
		bool Hittest(const Point& pt, Views& views);

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
		void LayoutBackground();

		void SetLayout(LayoutManager* layout);
		virtual Size GetPreferredSize() const;

		// Background ------------------------------------------------------------------
		void set_background_color(Color color);
		void set_background_image_id(const std::string& id);
		void set_background_inside(bool v);//true-在包含border的区域绘制;
		void SetBackground(View* background);
		
		// Painting ------------------------------------------------------------------
		virtual void SchedulePaint();
		virtual void SchedulePaintInRect(const Rect& r);

		void DoPaint(Painter* painter);
		virtual void OnPaint(Painter* painter);
		virtual void OnPaintChildren(Painter* painter);

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

		//void HandleEvent(Event* event);
		//void DispatchPropagation(Event* event);
		//void SetEventDelegate(EventDelegate* delegate);

		//virtual EventDispatcher* GetEventDispatcher() const;

		void SetFocus();
		void SetFocusable(bool focusable);
		bool IsFocusable() const;

		virtual FocusManager* GetFocusManager() const;

		// Property-------------------------------------------
		void SetPropertyBoolean(const std::string& path, bool in_value);
		void SetPropertyInteger(const std::string& path, int in_value);
		void SetPropertyDouble(const std::string& path, double in_value);
		void SetPropertyString(const std::string& path, const std::string& in_value);
		void SetPropertyString(const std::string& path, const std::wstring& in_value);

		bool GetPropertyBoolean(const std::string& path, bool* out_value) const;
		bool GetPropertyInteger(const std::string& path, int* out_value) const;
		bool GetPropertyDouble(const std::string& path, double* out_value) const;
		bool GetPropertyString(const std::string& path, std::string* out_value) const;
		bool GetPropertyString(const std::string& path, std::wstring* out_value) const;
	public:
		virtual void OnVisibleChanged();
		virtual void OnEnabledChanged();

	protected:
		// Coordinate conversion -----------------------------------------------------
		bool ConvertPointForAncestor(const View* ancestor, Point* point) const;
		bool ConvertPointFromAncestor(const View* ancestor, Point* point) const;
		void PaintBackground(Painter* painter);
		void PaintBorder(Painter* painter);

		View* parent_{ NULL };
		View* first_child_{ NULL };
		View* last_child_{ NULL };
		View* next_sibling_{ NULL };
		View* prev_sibling_{ NULL };
		int32 child_count_{ 0 };
		bool is_leaf_view_{ false };


		// This View's bounds in the parent coordinate system.
		Rect bounds_;
		bool visible_{ true };// Whether this view is visible.
		bool enabled_{ true };// Whether this view is enabled.
		bool painting_enabled_{ true };// Whether this view is painting.

		bool needs_layout_{ true };

		bool focusable_{ false };

		//Color background_color_{0xFFFFFFFF};

		scoped_ptr<Border> border_;
		scoped_ptr<View> background_;
		bool background_inside_{ false };

		scoped_ptr<LayoutManager> layout_manager_;

		HCURSOR cursor_{ NULL };

		scoped_ptr<DictionaryValue> property_;

		//LayoutSizePolicy width_policy_{ LAYOUT_SIZE_FIXED };
		//LayoutSizePolicy height_policy_{ LAYOUT_SIZE_FIXED };


		//scoped_ptr<EventDelegate> event_delegate_;
	};
}