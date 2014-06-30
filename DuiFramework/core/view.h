#pragma once
#include "base/basictypes.h"
#include <vector>

namespace ui
{
	class View
	{
	public:
		View();
		virtual ~View();

		typedef std::vector<View*> Views;

		View* parent() const;
		View* first_child() const;
		View* last_child() const;
		View* prev_sibling() const;
		View* next_sibling() const;

		View* root() const;

		void GetViews(Views &child_array) const;
		int32 GetViewCount() const;

		View* AppendTo(View* parent);
		View* Detach();

		View* Append(View* child);
		View* Remove(View* child);
		View* InsertAfter(View* ref, View* child);
		View* InsertBefore(View* ref, View* child);

	protected:
		View* parent_;
		View* first_child_;
		View* last_child_;
		View* next_sibling_;
		View* prev_sibling_;
		int32 child_count_;
	};
}