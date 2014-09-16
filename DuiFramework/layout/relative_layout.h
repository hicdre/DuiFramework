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
		RELATIVE_NULL = 0,
		RELATIVE_LEFT,
		RELATIVE_TOP = RELATIVE_LEFT,
		RELATIVE_RIGHT,
		RELATIVE_BOTTOM = RELATIVE_RIGHT,
		RELATIVE_CENTER,
	};


	enum RelativeSize
	{
		RLAYOUT_SIZE_NOTSET = 0,
		RLAYOUT_SIZE_FIXED,
		RLAYOUT_SIZE_RELATIVE_PERCENT,
		RLAYOUT_SIZE_RELATIVE_OFFSET,
	};


	struct RLayoutData : public View::Data
	{
		struct PositionType
		{
			PositionType() : position(RELATIVE_NULL), to(RELATIVE_TO_PARENT), offset(0) {}
			bool IsSet() { return position != RELATIVE_NULL; }
			void Set(RelativePosition p, RelativeToType t, int o) {
				position = p;
				to = t;
				offset = o;
			}
			RelativePosition position : 4;
			RelativeToType to : 4;
			int offset : 24;
		};

		struct SizeType
		{
			SizeType() : type(RLAYOUT_SIZE_NOTSET), data(0) {}
			bool IsSet() { return type != RLAYOUT_SIZE_NOTSET; }
			void Set(RelativeSize p, RelativeToType t, int o) {
				type = p;
				to = t;
				data = o;
			}
			RelativeSize type : 4;
			RelativeToType to : 4;
			int data : 24;
		};
		
		PositionType left, top, right, bottom;
		SizeType width, height;

	};


	class RelativeLayoutManager : public LayoutManager
	{
	public:
		RelativeLayoutManager();
		~RelativeLayoutManager();

		virtual void Layout(View* host) override;
	protected:
		static bool IsRelativeLayout(View* v);
		static RLayoutData* GetLayoutData(View* v);
		static bool ParseRelativeData(View* v);
		
	};

	void RelativeLayoutTest();
}