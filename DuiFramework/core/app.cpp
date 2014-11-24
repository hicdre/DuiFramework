#include "stdafx.h"
#include "app.h"

#include <cassert>
#include "core/widget.h"
#include "utils/utils.h"
#include "render/font.h"
#include "style/style_property.h"

namespace ui
{
	static const wchar_t kMessageWndClass[] = L"DuiFramework.MessageWindow";
	static const wchar_t kWndClass[] = L"DuiFramework.Window";

	static const int kMsgHaveWork = WM_USER + 1;

	App::App()
	{
		Init();
	}

	App::~App()
	{

	}

	App* App::Get()
	{
		static App instance;
		return &instance;
	}

	void App::Run()
	{
		for (;;) {
			// If we do any work, we may create more messages etc., and more work may
			// possibly be waiting in another task group.  When we (for example)
			// ProcessNextWindowsMessage(), there is a good chance there are still more
			// messages waiting.  On the other hand, when any of these methods return
			// having done no work, then it is pretty unlikely that calling them again
			// quickly will find any work to do.  Finally, if they all say they had no
			// work, then it is a good time to consider sleeping (waiting) for more
			// work.

			ProcessNextWindowsMessage();

			if (should_quit_)
				break;

			WaitForWork();  // Wait (sleep) until we have work to do again.
		}

		UnInit();
	}

	void App::Init()
	{
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		GdiplusStartup(&gdiplusToken_, &gdiplusStartupInput, NULL);

		InitStyleProperty();

		InitMessageWndClass();
		Widget::InitClass();

		{
			app_path_.resize(MAX_PATH);
			DWORD size = GetModuleFileName(instance_, &app_path_[0], MAX_PATH);
			app_path_.resize(size);
		}
	}


	void App::UnInit()
	{
		Font::UnInitFont();
		Gdiplus::GdiplusShutdown(gdiplusToken_);
	}


	void App::InitMessageWndClass()
	{
		WNDCLASSEX wc = { 0 };
		wc.cbSize = sizeof(wc);
		wc.lpfnWndProc = WndProcThunk;
		wc.hInstance = GetModuleFromAddress(wc.lpfnWndProc);
		wc.lpszClassName = kMessageWndClass;
		instance_ = wc.hInstance;
		RegisterClassEx(&wc);

		message_hwnd_ =
			::CreateWindow(kMessageWndClass, 0, 0, 0, 0, 0, 0, HWND_MESSAGE, 0, instance_, 0);
	}

	

	LRESULT CALLBACK App::WndProcThunk(
		HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {

		LRESULT rv = 0;
		__try {
			switch (message) {
			case kMsgHaveWork:
				reinterpret_cast<App*>(wparam)->HandleWorkMessage();
				break;
			case WM_TIMER:
				reinterpret_cast<App*>(wparam)->HandleTimerMessage();
				break;
			}
			rv = DefWindowProc(hwnd, message, wparam, lparam);
		}
		__except (EXCEPTION_CONTINUE_SEARCH) {
		}
		return rv;
	}

	LRESULT CALLBACK App::WndProcNormal(
		HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {

		LRESULT rv = 0;
		__try {
			//获取window节点
		}
		__except (EXCEPTION_CONTINUE_SEARCH) {
		}
		return rv;
	}

	void App::HandleWorkMessage()
	{

	}

	void App::HandleTimerMessage()
	{

	}

	bool App::ProcessNextWindowsMessage()
	{
		bool sent_messages_in_queue = false;
		DWORD queue_status = GetQueueStatus(QS_SENDMESSAGE);
		if (HIWORD(queue_status) & QS_SENDMESSAGE)
			sent_messages_in_queue = true;

		MSG msg;
		if (!PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			return sent_messages_in_queue;

		if (WM_QUIT == msg.message) {
			// Repost the QUIT message so that it will be retrieved by the primary
			// GetMessage() loop.
			should_quit_ = true;
			PostQuitMessage(static_cast<int>(msg.wParam));
			return false;
		}

		// Todo: While running our main message pump, we discard kMsgHaveWork messages.
		if (msg.message == kMsgHaveWork && msg.hwnd == message_hwnd_)
			return true;

		if (CallMsgFilter(const_cast<MSG*>(&msg), kMessageFilterCode))
			return true;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
		return true;
	}

	void App::WaitForWork()
	{
		DWORD result;
		result = MsgWaitForMultipleObjectsEx(0, NULL, INFINITE, QS_ALLINPUT,
			MWMO_INPUTAVAILABLE);

		if (WAIT_OBJECT_0 == result) {
			// A WM_* message is available.
			// If a parent child relationship exists between windows across threads
			// then their thread inputs are implicitly attached.
			// This causes the MsgWaitForMultipleObjectsEx API to return indicating
			// that messages are ready for processing (Specifically, mouse messages
			// intended for the child window may appear if the child window has
			// capture).
			// The subsequent PeekMessages call may fail to return any messages thus
			// causing us to enter a tight loop at times.
			// The WaitMessage call below is a workaround to give the child window
			// some time to process its input messages.
			MSG msg = { 0 };
			DWORD queue_status = GetQueueStatus(QS_MOUSE);
			if (HIWORD(queue_status) & QS_MOUSE &&
				!PeekMessage(&msg, NULL, WM_MOUSEFIRST, WM_MOUSELAST, PM_NOREMOVE)) {
				WaitMessage();
			}
			return;
		}
	}

	ImageStore* App::DefaultImageStore()
	{
		if (!image_store_.get()) {
			image_store_.reset(new ImageStore);
		}
		return image_store_.get();
	}


	ResourceLoader* App::GetResourceLoader()
	{
		if (!resource_loader_.get()) {
			resource_loader_.reset(new ResourceLoader);
		}
		return resource_loader_.get();
	}


	void App::Quit()
	{
		::PostQuitMessage(0);
	}

	std::wstring App::GetAppPath() const
	{
		return app_path_;
	}


}