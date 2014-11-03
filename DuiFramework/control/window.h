#pragma once
#include "base/basictypes.h"
#include "core/widget.h"
#include "view/view.h"

namespace ui
{
	class Window 
		: public View
		, public Widget::MessageHandler
	{
		friend class WindowBuilder;
	public:
		Window();
		virtual ~Window();

		void Load(const std::string& str);
		void LoadFile(const std::wstring& file);

		void AttachWidget(Widget* widget);
		Widget* DetachWidget();
		Widget* widget() const { return owned_widget_; }

		void SetWindowBounds(int x, int y, int width, int height);
		Rect GetWindowBounds();
		void SetWindowSize(int w, int h);
		Size GetWindowSize();

		void CenterWindow();
		void Close();

		virtual void OnChildSchedulePaintInRect(View* child, const Rect& r) override;
	private:
		void SchedulePaint(const Rect& r);
		virtual BOOL ProcessWindowMessage(HWND window,
			UINT message,
			WPARAM w_param,
			LPARAM l_param,
			LRESULT& result) override;
#if 0
		void ProcessMouseMessage(UINT message, WPARAM w_param, LPARAM l_param);
		void DispatchMouseLeaveEvent(View* from, View* to, const Point& pt);
		void DispatchMouseEnterEvent(View* from, View* to, const Point& pt);

		void ProcessKeyMessage(UINT message, WPARAM w_param, LPARAM l_param);

		void UpdateHittest(const Point& pt);
#endif
		bool constructed_{ false };
		Widget* owned_widget_{ NULL };

		//scoped_ptr<FocusManager> focus_manager_;
		View* focused_view_{ NULL };
		View* hittest_view_{ NULL };
		View* captured_view_{ NULL };
	};
}