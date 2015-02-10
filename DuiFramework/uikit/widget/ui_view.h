#pragma once
//#include "ui/ui_document.h"
//#include "render/render_forward.h"
//#include "event/event_target.h"
#include "foundation/foundation.h"
#include "uikit/ui_responder.h"
#include "uikit/common/ui_constants.h"
#include "uikit/render/ui_render_context.h"

#include <set>

namespace ui
{
	//表示一块绘制区域
	class UIViewController;
	class UIWindow;
	class UIView : public UIResponder
	{
	public:
		UIView();
		UIView(const Rect& rect);

		virtual void Init() override;
		virtual void InitWithBounds(const Rect& rect);

		virtual UIResponder* NextResponder() const override;

		//================================================
		//Hierarchy
		UIWindow* window() const;
		UIView* parent() const;
		bool HasChildren() const;
		bool HasParent() const;
		virtual void addSubView(UIView *view);
		virtual void removeFromParent();
		virtual void insertSubViewAfter(UIView* view, UIView* viewAfter);
		virtual void insertSubViewBefore(UIView* view, UIView* viewBefore);

		//=================================================
		//position
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
		//Rect GetContentBounds() const;

		static Point ConvertPointToView(UIView* elem, const Point& pt);

		//=================================================
		//attribute
// 		void setTag(const std::string& tag);
// 		const std::string& getTag() const;
// 		void setId(const std::string& id);
// 		const std::string& getId() const;
// 
// 		void addClass(const std::string& v);
// 		void removeClass(const std::string& v);
// 		bool haveClass(const std::string& v) const;
// 		void clearClass();

		//=================================================
		//styles
// 		void UpdateStyles();
// 		UIStyles* styles() const;
// 
// 		int marginLeft() const;
// 		int marginTop() const;
// 		int marginRight() const;
// 		int marginBottom() const;
// 		int marginWidth() const { return marginLeft() + marginRight(); }
// 		int marginHeight() const { return marginTop() + marginBottom(); }
		//=================================================
		//paint
		virtual void SchedulePaint();
		virtual void SchedulePaintInRect(const Rect& r);
		virtual void OnChildSchedulePaintInRect(UIView* child, const Rect& r);
		void DoPaint(UIRenderContext* painter, const Rect& r);
		virtual void OnPaint(UIRenderContext* painter);

		//=================================================
		//layout
		virtual void Layout();
		virtual void LayoutIfNeeded();
		virtual void OnBoundsChanged();
		virtual void OnPositionChanged();
		virtual void OnChildBoundsChanged(UIView* child);

		//=================================================
		//event 
		// hittest with event
		virtual UIView* HitTestWithEvent(const Point& pt, UIEvent* event);
		virtual bool PointInsideWithEvent(const Point& pt, UIEvent* event);
// 		void HandleLocalEvents(Event* event);
// 		bool DispatchEvent(Event* event, EventPath* path = NULL);

// 		bool IsHandleMouseEvent() const { return handle_mouse_event_; }
// 		bool IsHandleKeybordEvent() const { return handle_keybord_event_; }
		//=================================================
		//state
// 		bool hovered() const { return hovered_; }
// 		bool actived() const { return !handle_keybord_event_ && actived_or_focused_; }
// 		bool focused() const { return handle_keybord_event_ && actived_or_focused_; }
// 
// 		void SetHovered(bool v);
// 		void SetActiveOrFocused(bool v);

		//text
		//void SetText(UIText* text);

		void SetBackgroundColor(Color color);
		Color backgrondColor() const { return background_color_; }

		void SetVisible(bool v);
		bool isVisible() const { return visible_; }
		virtual void OnVisibleChanged();

		//@private
		UIView* previousSibling() const;
		UIView* nextSibling() const;
		UIView* firstChild() const;
		UIView* lastChild() const;
	protected:
		virtual ~UIView();
		void Unlink(UIView* child);

		//======================================
		//view hierarchy
		void AppendTo(UIView* parent);
		void Detach();

		void Append(UIView* child);
		void Prepend(UIView* child);
		void Remove(UIView* child);
		void InsertAfterChild(UIView* ref, UIView* child);
		void InsertBeforeChild(UIView* ref, UIView* child);

		void RemoveChildren();
		void RemoveChild(UIView* n);

		void doWillMoveToWindow(UIWindow* window);
		void doDidMoveToWindow(UIWindow* window);

		virtual void willMoveToWindow(UIWindow* window);
		virtual void didMoveToWindow(UIWindow* window);
		
		void setWindow(UIWindow* window);

		//======================================
		Rect ConvertRectFromChild(UIView* child, const Rect& r);
// 		void PaintBackground(RenderContext* painter);
// 		virtual void PaintContents(RenderContext* painter);

		//UIDocumentPtr document_;

		UIWindow* window_{ NULL };
		
		UIView* parent_{ NULL };
		UIView* prev_sibling_{ NULL };
		UIView* next_sibling_{ NULL };
		UIView* first_child_{ NULL };
		UIView* last_child_{ NULL };

// 		std::string tag_;
// 		std::string id_;
// 		std::set<std::string> classes_;

		//scoped_ptr<UIStyles> styles_;
		//scoped_ptr<UIText> text_;

		Rect bounds_; // 实际位置
		bool needs_layout_{true};//self needs layout
		bool child_needs_layout_{true};

		UIViewController* controller_{ NULL };

		Color background_color_{ Color_Transparent };
		bool visible_{ true };

// 		bool hovered_{ false };
// 		bool actived_or_focused_{ false };
// 
// 		bool handle_mouse_event_{ true };
// 		bool handle_keybord_event_{ false };

		DISALLOW_COPY_AND_ASSIGN(UIView);
	};
}