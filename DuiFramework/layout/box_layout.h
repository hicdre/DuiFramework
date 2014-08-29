#pragma once
#include "base/basictypes.h"
#include "render/size.h"
#include "render/padding.h"
#include "core/container.h"

namespace ui
{
	//hbox or vbox
	//子元素完全扩充空间
	class BoxLayout : public Container
	{
	public:
		BoxLayout(bool vertical = false);
		virtual ~BoxLayout();

		void SetVertical(bool v);
		bool IsVertical() const;

		void SetChildSpaceing(int i);
		void SetPadding(int left, int top, int right, int bottom);

		virtual void Layout() override;
		virtual Size GetPreferredSize() const override;

	private:
		bool is_vertical_{ false };
		int child_spaceing_{ 0 };
		Padding padding_;
		DISALLOW_COPY_AND_ASSIGN(BoxLayout);
	};
}