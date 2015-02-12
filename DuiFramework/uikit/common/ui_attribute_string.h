#pragma once
#include "foundation/foundation.h"
#include "uikit/common/ui_font.h"
#include "uikit/common/ui_range.h"
#include <set>
#include <functional>

namespace ui
{
	typedef UIRange<Color> UIColorRange;
	typedef UIRange<scoped_refptr<UIFont>> UIFontRange;
	class UIAttributeString
	{
	public:
		UIAttributeString();
		UIAttributeString(const std::wstring& contents);
		~UIAttributeString();

		const std::wstring& string() const { return string_; }
		size_t length() const { return string_.size(); }

		void setBackgroundColorInRange(Color color, const Range& range);
		void setTextColorInRange(Color color, const Range& range);
		void setFontInRange(UIFont* font, const Range& range);

	private:
		UIColorRange* backgroundColorRange();
		UIColorRange* textColorRange();
		UIFontRange* fontRange();
		std::wstring string_;
		scoped_ptr<UIColorRange> backgroundColorRange_;
		scoped_ptr<UIColorRange> textColorRange_;
		scoped_ptr<UIFontRange> fontRange_;
	};
}