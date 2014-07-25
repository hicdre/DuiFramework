#pragma once
#include "base/basictypes.h"
#include "render/size.h"
#include "layout/layout_manager.h"

namespace ui
{
	//×Óview³ß´çµÈÓë¸¸view³ß´ç
	class FillLayout : public LayoutManager 
	{
	public:
		FillLayout();
		virtual ~FillLayout();

		// Overridden from LayoutManager:
		virtual void Layout(View* host) override;
		virtual Size GetPreferredSize(const View* host) const override;

	private:
		DISALLOW_COPY_AND_ASSIGN(FillLayout);
	};
}