#pragma once
#include "base/basictypes.h"
#include "render/size.h"
#include "render/padding.h"
#include "layout/layout_manager.h"

namespace ui
{
	class BoxLayout : public LayoutManager
	{
	public:
		BoxLayout(bool vertical = false);
		virtual ~BoxLayout();

		void SetVertical(bool v);
		bool IsVertical() const;

		void SetChildSpaceing(int i);
		void SetPadding(int left, int top, int right, int bottom);

		// Overridden from LayoutManager:
		virtual void Layout(View* host) override;
		virtual Size GetPreferredSize(const View* host) const override;

	private:
		bool is_vertical_{ false };
		int child_spaceing_{ 0 };
		Padding padding_;
		DISALLOW_COPY_AND_ASSIGN(BoxLayout);
	};
}