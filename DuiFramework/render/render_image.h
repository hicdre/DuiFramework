#pragma once
#include "base/basictypes.h"

namespace ui
{
	class RenderImage : public RefCounted<RenderImage>
	{
	public:
		virtual ~RenderImage() {}

		virtual const Size& GetSize() const = 0;
	};

}