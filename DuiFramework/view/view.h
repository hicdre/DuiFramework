#pragma once
#include "base/basictypes.h"
#include "base/scoped_ptr.h"
#include "view/view_border.h"
#include "view/view_background.h"
#include "view/view_container.h"
#include <vector>

namespace ui
{
	class Widget;
	class View;
	class Container;
	typedef std::vector<View*> Views;

	class View 
	{
	public:
		View();
		virtual ~View();

		// Tree operations -----------------------------------------------------------
		View* parent() const;

		Container* GetContainer();
		void SetContainer(Container* container);
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

		// Layout --------------------------------------------------------------------
		void Layout();

		// Painting ------------------------------------------------------------------
		virtual void SchedulePaint();
		virtual void SchedulePaintInRect(const Rect& r);

		//在当前RenderContext的r区域作画
		void DoPaint(RenderContext* painter, const Rect& r);


		// Attributes---------------------------------------------
		void SetBackground(Background* background);
		Background* background();

		void SetBorder(Border* border);
		Border* border() const;

		void SetCursor(HCURSOR cursor);
		HCURSOR GetCursor();

		// Event---------------------------------------------

		//自身事件
		virtual void OnVisibleChanged();
		virtual void OnEnabledChanged();
		virtual void OnBeforeChildPaint(RenderContext* painter);
		virtual void OnAfterChildPaint(RenderContext* painter);
		virtual void OnLayout();

		//Parent 事件
		virtual void OnChildVisibilityChanged(View* child);
		virtual void OnChildSchedulePaintInRect(View* child, const Rect& r);
		Rect ConvertRectFromChild(View* child, const Rect& r);
	protected:
		//层级关系
		scoped_ptr<Container> container_;
		View* parent_{ NULL };

		//公共属性
		scoped_ptr<Border> border_; // border-
		scoped_ptr<Background> background_; // background-
		HCURSOR cursor_{ NULL }; //	cursor

		//私有属性
		Matrix m_;//自身变换, 缩放, 旋转等
		Rect bounds_; // 实际位置
		bool visible_{ true };// Whether this view is visible.
		bool enabled_{ true };
		bool needs_layout_{ true };//强制layout
	};
}