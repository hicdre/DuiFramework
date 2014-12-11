#pragma once
#include "base/basictypes.h"

namespace ui
{
	class RenderContext;
	class RenderPathPrivate;
	class RenderPath
	{
	public:
		RenderPath(RenderContext* context);
		~RenderPath();

		void AddArc(const Point& center, uint32 radius, 
			uint32 angleFrom, uint32 angleTo,
			bool negative = false);

	private:
		RenderPathPrivate* path_;
	};
}