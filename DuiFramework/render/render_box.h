#pragma once
#include "render/render_object.h"

namespace ui
{
	class RenderImage;
	class RenderText;
	class RenderBorder;
	class RenderHorizonalContainer;
	class RenderVerticalContainer;

	class RenderBox : public RenderObject
	{
	public:
		RenderBox(UIElement* elem);
		~RenderBox();

		RenderContent* GetRenderChild();



		virtual void Layout() override;
	private:
		RenderChildPtr child_;
	};
}