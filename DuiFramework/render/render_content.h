#pragma once
#include "base/basictypes.h"
#include "render/render_object.h"

namespace ui
{
	class RenderContent : public RenderObject
	{
	public:
		RenderContent(UIElement* elem);
		~RenderContent();

		virtual void Layout() override;
	};
}