#pragma once
#include "base/basictypes.h"
#include "core/container.h"

namespace ui
{
	class FrameLayout : public Container
	{
	public:
		FrameLayout();
		virtual ~FrameLayout();

	public:
		virtual void Layout() override;
		virtual Size GetPreferredSize() const override;
	private:
		DISALLOW_COPY_AND_ASSIGN(FrameLayout);
	};
}