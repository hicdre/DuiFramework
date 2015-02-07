#pragma once
#include "foundation/foundation.h"
#include "uikit/common/ui_font.h"
#include <set>
#include <functional>

namespace ui
{
	struct UICharAttribute
	{
		UICharAttribute(const UICharAttribute& o)
			: fontColor(o.fontColor)
			, backgroundColor(o.backgroundColor)
			, font(o.font)
		{

		}

		UICharAttribute()
			: font(UIFont::CreateDefault())
		{
		}
		bool Equals(const UICharAttribute& other) const {
			return fontColor == other.fontColor
				&& backgroundColor == other.backgroundColor
				&& font->IsEqual(other.font);
		}

		Color fontColor{ Color_Black };
		Color backgroundColor{ Color_Transparent };
		scoped_refptr<UIFont> font;
	};

	struct UICharAttributeRange
	{
		Range range;
		UICharAttribute attr;
		UICharAttributeRange* prev{ NULL };
		UICharAttributeRange* next{ NULL };
		UICharAttributeRange(const Range& r) : range(r) {};
		UICharAttributeRange(const Range& r, const UICharAttribute& t) 
			: range(r)
			, attr(t)
		{}
	};

	class UICharAttributeRangeList
	{
	public:
		UICharAttributeRangeList();
		UICharAttributeRangeList(const Range& r);
		~UICharAttributeRangeList();

		UICharAttribute* GetAttributeAtIndex(size_t index);
		UICharAttribute* GetAttributeAtRange(const Range& r);

		void ForIn(std::function<bool(const Range&, const UICharAttribute&)> callback) const;

		void UpdateAttributeInRange(const Range& r, std::function<void(UICharAttribute&)> callback);
		void UpdateAttributeBreakRange(const Range& r, std::function<void(UICharAttribute&)> callback);

		Range GetRangeWithSameAttribute(size_t index) const;

		UICharAttributeRangeList* Copy(const Range& r) const;
		void Normalize();
		void Offset(int32 offset);
	private:
		void Build();
		void Clear();
		UICharAttributeRange* PushBack(UICharAttributeRange* newRange);
		//p为NULL，表示在first前插入
		UICharAttributeRange* InsertAfter(UICharAttributeRange* prevRange, UICharAttributeRange* newRange);
		//返回p->prev, 当删除的为第一个时返回NULL
		UICharAttributeRange* Remove(UICharAttributeRange* deleteRange);
		UICharAttributeRange* first{ NULL };
		UICharAttributeRange* last{ NULL };
	};

	class UIAttributeString
	{
	public:

		UIAttributeString();
		UIAttributeString(const std::wstring& contents);
		~UIAttributeString();


	private:
		std::wstring string_;
		UICharAttributeRangeList* attributes_;
	};
}