#pragma once
#include "core/view.h"
#include "layout/layout_manager.h"

namespace ui
{
	/*
	*/
	class FluidLayout : public LayoutManager
	{
	public:
		FluidLayout(bool horizonal);
		virtual ~FluidLayout() override;

		void SetNearView(View* view);
		void SetCenterView(View* view);
		void SetFarView(View* view);

		virtual void Layout(View* host) override;
		virtual Size GetPreferredSize(const View* host) const override;
	private:
		void LayoutHorizonal(const Rect& child_area);
		void LayoutVerical(const Rect& child_area);
		bool IsInLayout(View* v) const;
		View* near_{ NULL };//πÃ∂®
		View* center_{ NULL };//¿©’≈
		View* far_{ NULL };//πÃ∂®
		bool is_horizonal_;
	};
}

