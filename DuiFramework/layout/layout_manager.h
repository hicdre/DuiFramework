#pragma once

namespace ui
{
	class View;
	class LayoutManager
	{
	public:
		virtual ~LayoutManager() {}

		virtual void Installed(View* host) {}

		virtual void Uninstalled(View* host) {}

		virtual void Layout(View* host) = 0;

		virtual Size GetPreferredSize(const View* host) const = 0;
	};
}