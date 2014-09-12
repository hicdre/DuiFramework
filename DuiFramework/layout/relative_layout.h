#pragma once

#include "layout/layout_manager.h"
#include "core/view.h"

namespace ui
{
	enum RelativeToType
	{
		RELATIVE_TO_PARENT = 0,
		RELATIVE_TO_PREV,
		RELATIVE_TO_NEXT,
	};

	enum RelativePosition
	{
		RELATIVE_LEFT_TOP = 0,
		RELATIVE_MIDDLE_TOP,
		RELATIVE_RIGHT_TOP,
		RELATIVE_LEFT_MIDDLE,
		RELATIVE_MIDDLE_MIDDLE,
		RELATIVE_RIGHT_MIDDLE,
		RELATIVE_LEFT_BOTTOM,
		RELATIVE_MIDDLE_BOTTOM,
		RELATIVE_RIGHT_BOTTOM,
	};


	enum RelativeSizeType
	{
		RELATIVE_SIZE_PIXEL,
		RELATIVE_SIZE_PERCENT,
	};



	struct RelativePositionData 
	{
		RelativeToType relative_to;
		RelativePosition relative_position;
		int x, y;
	};

	struct RelativeSizeData
	{
		RelativeSizeType w_type;
		RelativeSizeType h_type;
		int w, h;
	};

	struct RelativeLayoutData : public View::Data
	{
		RelativePositionData xy;
		union 
		{
			RelativePositionData rb;
			RelativeSizeData wh;
		};
	};
	class RelativeLayoutManager : public LayoutManager
	{
	public:
		RelativeLayoutManager();
		~RelativeLayoutManager();

		virtual void Layout(View* host) override;
	protected:
		static bool IsRelativeLayout(View* v);
		static RelativeLayoutData* GetLayoutData(View* v);
		static bool ParseRelativeData(View* v);
	};
}