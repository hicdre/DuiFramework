#include "stdafx.h"
#include "ui_application.h"

#include "uikit/render/ui_render_engine.h"
#include "uikit/widget/ui_platform_window.h"

namespace ui
{
	static const wchar_t kMessageWndClass[] = L"DuiFramework.MessageWindow";
	static const wchar_t kWndClass[] = L"DuiFramework.Window";

	static const int kMsgHaveWork = WM_USER + 1;

	

	HMODULE GetModuleFromAddress(void* address)
	{
		HMODULE instance = NULL;
		if (!::GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
			GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
			static_cast<char*>(address),
			&instance)) {
			assert(0);
		}
		return instance;
	}


	UIApplication* UIApplication::Get()
	{
		static UIApplication instance;
		return &instance;
	}

	UIApplication::UIApplication()
	{

	}

	UIApplication::~UIApplication()
	{

	}

	void UIApplication::Init()
	{
		instance_ = GetModuleFromAddress(WndProcThunk);

		InitApplictionPath();
		InitMessageWndClass();

		UIPlatformWindow::InitClass();
	}

	void UIApplication::UnInit()
	{
	}

	void UIApplication::InitApplictionPath()
	{
		app_path_.resize(MAX_PATH);
		DWORD size = GetModuleFileName(instance_, &app_path_[0], MAX_PATH);
		app_path_.resize(size);
	}

	void UIApplication::InitMessageWndClass()
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

	LRESULT CALLBACK UIApplication::WndProcThunk(
		HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {

		LRESULT rv = 0;
		__try {
			switch (message) {
			case kMsgHaveWork:
				reinterpret_cast<UIApplication*>(wparam)->HandleWorkMessage();
				break;
			case WM_TIMER:
				reinterpret_cast<UIApplication*>(wparam)->HandleTimerMessage();
				break;
			}
			rv = DefWindowProc(hwnd, message, wparam, lparam);
		}
		__except (EXCEPTION_CONTINUE_SEARCH) {
		}
		return rv;
	}

	void UIApplication::HandleWorkMessage()
	{

	}

	void UIApplication::HandleTimerMessage()
	{

	}

	bool UIApplication::ProcessNextWindowsMessage()
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

	void UIApplication::WaitForWork()
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

	void UIApplication::Quit()
	{
		::PostQuitMessage(0);
	}

	std::wstring UIApplication::GetAppPath() const
	{
		return app_path_;
	}

	void UIApplication::SetDelegate(UIApplicationDelegate* delegate)
	{
		delegate_.reset(delegate);
	}

	void UIApplication::Run()
	{
		delegate_->OnApplicationInit();
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

		delegate_->OnApplicationExit();

		UnInit();
	}

	int UIApplication::Main(UIApplicationDelegate* delegate)
	{
		if (!delegate)
			return -1;

		UIApplication::Get()->Init();
		UIApplication::Get()->SetDelegate(delegate);
		UIApplication::Get()->Run();
		
		return 0;
	}

	void UIApplication::AddWindow(UIWindow* window)
	{
		windows_.push_back(window);
	}

	void UIApplication::RemoveWindow(UIWindow* window)
	{
		for (auto iter = windows_.begin(); iter != windows_.end(); ++iter)
		{
			if (*iter == window)
			{
				windows_.erase(iter);
				break;
			}
		}
	}

	void UIApplication::GetWindows(std::vector<UIWindow*>& windows) const
	{
		windows = windows_;
	}

	UIRenderEngine* UIApplication::DefaultRenderEngine()
	{
		if (!engine_.get())
		{
			engine_.reset(UIRenderEngine::Init(UIRenderEngine::EngineCairo));
		}
		return engine_.get();
	}

}