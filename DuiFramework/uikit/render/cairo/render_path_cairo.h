#pragma once
#include "uikit/render/ui_render_path.h"
#include "uikit/render/cairo/cairo_forward.h"

namespace ui
{
	class UIRenderTargetCairo;
	class UIRenderPathCairo : public UIRenderPath
	{
	public:
		UIRenderPathCairo(UIRenderTargetCairo* target);
		~UIRenderPathCairo();

		virtual void Arc(const Point& center, uint32 radius,
			uint32 degreeFrom, uint32 degreeTo,
			bool negative = false) override;

		virtual void BezierTo(const Point &aCP1,
			const Point &aCP2,
			const Point &aCP3) override;

		virtual void QuadraticBezierTo(const Point &aCP1,
			const Point &aCP2) override;

		virtual void Rectangle(const Rect& rect, bool negative = false) override;

		virtual void RoundRectangle(const RoundRect& rect, bool negative = false) override;

		virtual void LineTo(const Point &aPoint) override;

		virtual void MoveTo(const Point &aPoint) override;

		virtual Point CurrentPoint() const override;

		virtual void EndPath() override;

		cairo_path_t* get_path() const;
	private:
		scoped_refptr<UIRenderTargetCairo> target_;
		cairo_t* cairo_;
		cairo_path_t* path_;
	};
}