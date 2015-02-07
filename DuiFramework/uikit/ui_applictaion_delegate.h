#pragma once

namespace ui
{
	class UIView;
	class UIApplicationDelegate
	{
	public:
		virtual ~UIApplicationDelegate() {}

		virtual void OnApplicationInit() {}

		virtual void OnApplicationExit() {}
	};
}