#pragma once

#include "base/basictypes.h"
#include "render/rect.h"

namespace ui
{
	class Widget
	{
	public:
		class MessageHandler {
		public:
			virtual ~MessageHandler() {}

			virtual BOOL ProcessWindowMessage(HWND window,
				UINT message,
				WPARAM w_param,
				LPARAM l_param,
				LRESULT& result) = 0;
		};

		Widget();
		virtual ~Widget();

		static Widget* Create(Widget* parent, const Rect& bounds);
		static Widget* Create();

		static Widget* GetWidgetForHWND(HWND hwnd);

		static void InitClass();

		void Init(HWND parent, const Rect& bounds);

		// Returns the HWND associated with this Window.
		HWND hwnd() const { return hwnd_; }

		// Sets the window styles. This is ONLY used when the window is created.
		// In other words, if you invoke this after invoking Init, nothing happens.
		void set_window_style(DWORD style) { window_style_ = style; }
		DWORD window_style() const { return window_style_; }

		// Sets the extended window styles. See comment about |set_window_style|.
		void set_window_ex_style(DWORD style) { window_ex_style_ = style; }
		DWORD window_ex_style() const { return window_ex_style_; }

		// Sets the class style to use. The default is CS_DBLCLKS.
		void set_initial_class_style(UINT class_style) {
			// We dynamically generate the class name, so don't register it globally!
			class_style_ = class_style;
		}
		UINT initial_class_style() const { return class_style_; }

		// Sets the specified view as the contents of this Widget. There can only
		// be one contents view child of this Widget's RootView. This view is sized to
		// fit the entire size of the RootView. The RootView takes ownership of this
		// View, unless it is set as not being parent-owned.
		void SetMessageHanler(MessageHandler* view);
		MessageHandler* GetMessageHanler();

		// Returns the bounds of the Widget in screen coordinates.
		Rect GetWindowScreenBounds() const;

		// Retrieves the restored bounds for the window.
		//Rect GetRestoredBounds() const;

		// Sizes and/or places the widget to the specified bounds, size or position.
		void SetBounds(const Rect& bounds);
		void SetSize(const Size& size);

		// Sizes the window to the specified size and centerizes it.
		void CenterWindow(const Size& size);

		bool IsWindow();
		HWND GetParent() const;
		LONG GetWindowLong(int index);
		BOOL GetWindowRect(RECT* rect) const;
		LONG SetWindowLong(int index, LONG new_long);
		BOOL SetWindowPos(HWND hwnd_after, int x, int y, int cx, int cy, UINT flags);
		BOOL SetWindowPos(int x, int y);
		BOOL MoveWindow(int x, int y, int width, int height);
		BOOL MoveWindow(int x, int y, int width, int height, BOOL repaint);
		int  SetWindowRgn(HRGN region, BOOL redraw);
		BOOL GetClientRect(RECT* rect) const;
		void SetCursor(HCURSOR cursor);

		void SetFocus();
		bool IsFocused();

		void InvalidateRect(const Rect& r);


		void Close();

		void Show(int state);
		void Hide();

		void Active();

		bool IsActive() const;

		void Maximize();
		void Minimize();
		void Restore();

		// Whether or not the window is maximized or minimized.
		virtual bool IsMaximized() const;
		bool IsMinimized() const;

		bool IsVisible() const;

	protected:
		// Handles the WndProc callback for this object.
		virtual LRESULT OnWndProc(UINT message, WPARAM w_param, LPARAM l_param);

	private:
		static LRESULT CALLBACK WndProc(HWND window,
			UINT message,
			WPARAM w_param,
			LPARAM l_param);

		BOOL ProcessWindowMessage(HWND window,
			UINT message,
			WPARAM w_param,
			LPARAM l_param,
			LRESULT& result);

		MessageHandler* handler_;

		// The restored bounds used for the initial show. This is only used if
		// |saved_show_state_| is maximized.
		Rect initial_restored_bounds_;

		HWND hwnd_;

		// Window Styles used when creating the window.
		DWORD window_style_;

		// Window Extended Styles used when creating the window.
		DWORD window_ex_style_;

		// Style of the class to use.
		UINT class_style_;

		HCURSOR previous_cursor_;


		DISALLOW_COPY_AND_ASSIGN(Widget);
	};
}