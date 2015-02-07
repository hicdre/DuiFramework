#pragma once
#include "foundation/foundation.h"
#include "uikit/ui_applictaion_delegate.h"
#include "uikit/ui_responder.h"
#include "uikit/render/ui_render_engine.h"



namespace ui
{
	class UIWindow;
	class UIApplication : public UIResponder
	{
	public:
		static UIApplication* Get();

		static int Main(UIApplicationDelegate* delegate);
		UIApplication();
		virtual ~UIApplication();

		virtual void Init();
		virtual void UnInit();

		std::wstring GetAppPath() const;

		void Run();
		void Quit();


		void GetWindows(std::vector<UIWindow*>& windows) const;
		//=====================================
		//private
		void AddWindow(UIWindow* window);
		void RemoveWindow(UIWindow* window);

		UIRenderEngine* DefaultRenderEngine();
	private:
		void SetDelegate(UIApplicationDelegate* delegate);

		void InitApplictionPath();
		void InitMessageWndClass();

		static LRESULT CALLBACK WndProcThunk(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
		static LRESULT CALLBACK WndProcNormal(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
		void HandleWorkMessage();
		void HandleTimerMessage();

		bool ProcessNextWindowsMessage();
		void WaitForWork();

		static const int kMessageFilterCode = 0x5001;

		std::wstring app_path_;
		scoped_ptr<UIApplicationDelegate> delegate_;


		std::vector<UIWindow*> windows_;
		HWND message_hwnd_;
		HINSTANCE instance_;
		bool should_quit_{ false };

		scoped_ptr<UIRenderEngine> engine_;
	};
}