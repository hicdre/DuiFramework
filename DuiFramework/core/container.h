#pragma once
#include "core/view.h"

namespace ui
{
	class Container : public View
	{
	public:
		Container();
		virtual ~Container();

		View* first_child() const;
		View* last_child() const;

		void GetViews(Views &child_array) const;
		int32 GetViewCount() const;

		View* Append(View* child);
		View* Remove(View* child);
		View* InsertAfter(View* ref, View* child);
		View* InsertBefore(View* ref, View* child);

		bool HasDescender(View* descender, int* step = NULL);

		virtual View* Hittest(const Point& pt);//pt在本坐标系
		bool Hittest(const Point& pt, Views& views);

		virtual void Layout() override;

		virtual void DoPaint(Painter* painter, const Rect& dest) override;
	protected:
		void DoPaintChildren(Painter* painter);
		View* first_child_{ NULL };
		View* last_child_{ NULL };
		int32 child_count_{ 0 };
	};
}