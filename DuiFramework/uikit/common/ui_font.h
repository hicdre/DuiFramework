#pragma once
#include "foundation/foundation.h"

namespace ui
{
	//   +--------+-------------------+------------------+
	//   |        |                   | internal leading |
	//   |        | ascent (baseline) +------------------+
	//   | height |                   | cap height       |
	//   |        |-------------------+------------------+
	//   |        | descent (height - baseline)          |
	//   +--------+--------------------------------------+
	class UIRenderFont;
	class UIFont : public RefCounted<UIFont>
	{
	public:
		enum FontStyle {
			NORMAL = 0,
			BOLD = 1,
			ITALIC = 2,
			UNDERLINE = 4,
		};
		~UIFont();

		static scoped_refptr<UIFont> CreateDefault();
		static scoped_refptr<UIFont> CreateFont(const std::wstring& font_name,
			int font_size,
			int style = 0);
		UIFont* Derive(int size_delta, int style) const;

		int style() const { return style_; }
		const std::wstring& font_name() const { return font_name_; }
		int font_size() const { return font_size_; }

		bool isItalic() const { return !!(style_ & ITALIC); }
		bool isUnderline() const { return !!(style_ & UNDERLINE); }
		bool isBold() const { return !!(style_ & BOLD); }

		bool IsEqual(UIFont* other) const;

		scoped_refptr<UIRenderFont> renderFont();

		std::wstring description() const;
	private:
		friend class UIFontFactory;
		UIFont(const std::wstring& font_name,
			int font_size,
			int style);
		// Creates a font with the specified name in UTF-8 and size in pixels.
		UIFont(const std::wstring& font_name, int font_size);
		const int font_size_;
		const int style_;
		const std::wstring font_name_;
		scoped_refptr<UIRenderFont> render_font_;
		DISALLOW_COPY_AND_ASSIGN(UIFont);
	};
}