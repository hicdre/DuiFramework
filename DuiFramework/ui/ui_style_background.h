#pragma once
#include "ui/ui_forward.h"
#include "style/style_result.h"
#include "render/render_context.h"

namespace ui
{
	class UIStyleBackground
	{
	public:
		UIStyleBackground();
		~UIStyleBackground();

		void Init(UIStyles* styles);

		Color color() const { return color_; }

		const RenderImage* image() const;
		const Rect& source_rect() const { return source_rect_; }
	private:
		void InitBackgroundColor();
		void InitBackgroundImage();

		static bool GetLengthFromValue(StyleValue* v, int full, int &length);
		Color color_;
		scoped_refptr<RenderImage> image_;
		Rect source_rect_;

		UIStyles* styles_;
	};
}