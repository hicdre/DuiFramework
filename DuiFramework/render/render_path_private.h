#pragma once
#include "base/basictypes.h"

namespace ui
{
	class RenderContextPrivate;
	class RenderPathPrivate
	{
	public:
		RenderPathPrivate(RenderContextPrivate* context);
		~RenderPathPrivate();

		void AddArc(const Point& center, uint32 radius,
			uint32 angleFrom, uint32 angleTo,
			bool negative = false);
	private:
		RenderContextPrivate* context_;
	};
}