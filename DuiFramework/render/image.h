#pragma once
#include "base/basictypes.h"

namespace ui
{
	class RenderContext;
	//目前只有拉伸，
	//需要增加平铺等
	class Image : public RefCounted<Image>
	{
	public:
		static Image* CreateFromResourceId(const std::string& id);
		static Image* CreateFromLocalPath(const std::wstring& path);
		static Image* CreateLinearGradient(const Point& pos_from,
			const Point& pos_to, Color color_from, Color color_to);

		virtual ~Image() {}

		virtual void DoPaint(RenderContext* painter, const Rect& dest) = 0;
	};
}