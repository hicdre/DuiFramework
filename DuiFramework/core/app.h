#pragma once

namespace ui
{
	class App
	{
	public:
		static App* Get();

		void Run();

	private:
		App();
		~App();

		void Init();
		void InitMessageWndClass();

		static LRESULT CALLBACK WndProcThunk(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
		static LRESULT CALLBACK WndProcNormal(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
		void HandleWorkMessage();
		void HandleTimerMessage();

		bool ProcessNextWindowsMessage();
		void WaitForWork();

		static const int kMessageFilterCode = 0x5001;

		HWND message_hwnd_;
		HINSTANCE instance_;
	};
}