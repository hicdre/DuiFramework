#include "stdafx.h"
#include "ui_window.h"
#include "ui_platform_window.h"

#include "uikit/render/ui_render_engine.h"
#include "uikit/render/ui_render_context.h"
#include "uikit/event/ui_event_manager.h"
#include "uikit/ui_application.h"

namespace ui
{
	class UIWindowPrivate 
		: public UIPlatformWindow
		, public UIPlatformWindow::MessageHandler
	{
	public:
		UIWindowPrivate(UIWindow* window)
			: window_(window)
		{

		}

		~UIWindowPrivate()
		{
		}

		void Init(HWND hwnd, const Rect& rect)
		{
			UIPlatformWindow::Init(hwnd, rect);
			SetMessageHanler(this);
		}

		virtual BOOL ProcessWindowMessage(HWND window,
			UINT message,
			WPARAM w_param,
			LPARAM l_param,
			LRESULT& result) override
		{
			if (message == WM_PAINT)
			{
				scoped_refptr<UIRenderContext> context(
					UIRenderEngine::current()->CreateRenderContextForHWND(hwnd()));
				window_->DoPaint(context, window_->GetLocalBounds());
				return TRUE;
			}
			else if (message == WM_SIZE)
			{
				//Size sz = { LOWORD(l_param), HIWORD(l_param) };
				//window_->SetSize(sz);
				RECT rc;
				GetWindowRect(&rc);
				window_->SetBounds(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
				return TRUE;
			}
			else if ((message >= WM_MOUSEFIRST && message <= WM_MOUSELAST)
				|| (message >= WM_NCMOUSEMOVE && message <= WM_NCXBUTTONDBLCLK)
				|| message == WM_MOUSELEAVE
				|| message == WM_NCMOUSELEAVE)
			{
// 				scoped_refptr<MouseEvent> event = BuildMouseEvent(owned_widget_->hwnd(), message, w_param, l_param, GetTickCount());
// 				event_handler_.HandleMouseEvent(event.get());
				UIEventManager::Get()->HanleMessage(window_, message, w_param, l_param);
				return TRUE;
			}
			return FALSE;
		}

	private:
		UIWindow* window_;
	};

	UIWindow::UIWindow()
		: private_(NULL)
	{
		
	}

	UIWindow::~UIWindow()
	{
		UIApplication::Get()->RemoveWindow(this);

		if (private_)
		{
			delete private_;
			private_ = NULL;
		}
	}

	void UIWindow::InitWithBounds(const Rect& rect)
	{
		UIView::InitWithBounds(rect);
		InitSubViews();
		PrivateInit();
	}

	void UIWindow::PrivateInit()
	{
		willAppear();
		private_ = new UIWindowPrivate(this);
		private_->Init(NULL, bounds_);
		window_ = this;

		UIApplication::Get()->AddWindow(this);

		if (visible_)
			private_->Show(SW_SHOWNORMAL);
		private_->SetBounds(bounds_);

		didAppear();
	}



	void UIWindow::SchedulePaintInRect(const Rect& r)
	{
		if (!r.IsEmpty() && private_)
			private_->InvalidateRect(r);
	}

	void UIWindow::Close()
	{
		private_->Close();
	}

	UIResponder* UIWindow::NextResponder() const
	{
		return UIApplication::Get();
	}

	void UIWindow::OnVisibleChanged()
	{
		if (visible_)
			private_->Show(SW_SHOWNORMAL);
		else
			private_->Hide();
	}

	Point UIWindow::ConvertFromScreen(const Point& pt)
	{
		return Point(pt.x() - x(), pt.y() - y());
	}

	Point UIWindow::ConvertToScreen(const Point& pt)
	{
		return Point(pt.x() + x(), pt.y() + y());
	}

	void UIWindow::SetCursor(CursorId id)
	{
		if (id == Cursor_Arrow)
			private_->SetCursor(::LoadCursor(NULL, IDC_ARROW));
		else if (id == Cursor_Hand)
			private_->SetCursor(::LoadCursor(NULL, IDC_HAND));
	}


	bool UIWindow::PointInsideWithEvent(const Point& pt, UIEvent* event)
	{
		bool ret = UIView::PointInsideWithEvent(pt, event);
		if (ret)
		{

			event->mouse()->modifyCursor(Cursor_Arrow);
		}
		return ret;
	}

	void UIWindow::mouseMove(UIMouse* mouse, UIEvent* event)
	{
		UIView::mouseMove(mouse, event);

		if (may_start_drag_)
		{
			private_->SetCaptured();
			Point prev_position = last_position_;
			last_position_ = ConvertToScreen(mouse->positionInWindow());
			int dx = last_position_.x() - prev_position.x();
			int dy = last_position_.y() - prev_position.y();
			SetPosition(x() + dx, y() + dy);
			std::cout << "drag move" << std::endl;
		}
		else
		{
			std::cout << "mouse move" << std::endl;
		}
	}

	void UIWindow::mousePress(UIMouse* mouse, UIEvent* event)
	{
		UIView::mousePress(mouse, event);

		if (mouse->view() == this
			&& mouse->changedButton() == UIMouse::ButtonLeft)
		{
			may_start_drag_ = true;
			last_position_ = ConvertToScreen(mouse->positionInWindow());
			std::cout << "drag may start" << std::endl;
		}
	}

	void UIWindow::mouseRelease(UIMouse* mouse, UIEvent* event)
	{
		UIView::mouseRelease(mouse, event);

		if (mouse->changedButton() == UIMouse::ButtonLeft)
		{
			may_start_drag_ = false;
			private_->ReleaseCaptured();
			std::cout << "release capture" << std::endl;
		}
		std::cout << "release mouse" << std::endl;
	}

	void UIWindow::OnPositionChanged()
	{
		UIView::OnPositionChanged();

		private_->SetWindowPos(x(), y());
	}

	void UIWindow::InitSubViews()
	{

	}

}
