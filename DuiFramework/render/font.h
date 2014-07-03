#pragma once
#include "base/basictypes.h"
#include "base/ref_counted.h"

#include <string>

namespace ui
{
	//   +--------+-------------------+------------------+
	//   |        |                   | internal leading |
	//   |        | ascent (baseline) +------------------+
	//   | height |                   | cap height       |
	//   |        |-------------------+------------------+
	//   |        | descent (height - baseline)          |
	//   +--------+--------------------------------------+
	class Font
	{
	public:
		enum FontStyle {
			NORMAL = 0,
			BOLD = 1,
			ITALIC = 2,
			UNDERLINE = 4,
		};
		Font();
		// Creates a font that is a clone of another font object.
		Font(const Font& other);
		Font& operator=(const Font& other);

		// Creates a font from the specified native font.
		explicit Font(HFONT hfont);
		~Font();

		// Creates a font with the specified name in UTF-8 and size in pixels.
		Font(const std::wstring& font_name, int font_size);

		// Returns a new Font derived from the existing font.
		// |size_delta| is the size in pixels to add to the current font. For example,
		// a value of 5 results in a font 5 pixels bigger than this font.
		// The style parameter specifies the new style for the font, and is a
		// bitmask of the values: BOLD, ITALIC and UNDERLINE.
		Font Derive(int size_delta, int style) const;

		int GetHeight() const;
		int GetBaseline() const;
		int GetCapHeight() const;

		int GetStyle() const;
		std::wstring GetFontName() const;
		int GetFontSize() const; //pixels

		HFONT ToHFONT() const;

		
	private:
		friend class App;
		class HFontRef : public RefCounted<HFontRef> {
		public:
			// This constructor takes control of the HFONT, and will delete it when
			// the HFontRef is deleted.
			HFontRef(HFONT hfont,
				int font_size,
				int height,
				int baseline,
				int cap_height,
				int ave_char_width,
				int style);

			// Accessors
			HFONT hfont() const { return hfont_; }
			int height() const { return height_; }
			int baseline() const { return baseline_; }
			int cap_height() const { return cap_height_; }
			int ave_char_width() const { return ave_char_width_; }
			int style() const { return style_; }
			const std::wstring& font_name() const { return font_name_; }
			int font_size() const { return font_size_; }
			int requested_font_size() const { return requested_font_size_; }

			// Returns the average character width in dialog units.
			//int GetDluBaseX();

		private:
			friend class RefCounted<HFontRef>;

			~HFontRef();

			const HFONT hfont_;
			const int font_size_;
			const int height_;
			const int baseline_;
			const int cap_height_;
			const int ave_char_width_;
			const int style_;
			// Average character width in dialog units. This is queried lazily from the
			// system, with an initial value of -1 meaning it hasn't yet been queried.
			//int dlu_base_x_;
			std::wstring font_name_;

			// If the requested font size is not possible for the font, |font_size_|
			// will be different than |requested_font_size_|. This is stored separately
			// so that code that increases the font size in a loop will not cause the
			// loop to get stuck on the same size.
			int requested_font_size_;

			DISALLOW_COPY_AND_ASSIGN(HFontRef);
		};
		explicit Font(HFontRef* hfont_ref);

		void InitWithCopyOfHFONT(HFONT hfont);
		void InitWithFontNameAndSize(const std::wstring& font_name,
			int font_size);

		//static void InitFont(const std::wstring& font_name,
		//	int font_size);
		static void UnInitFont();

		static HFontRef* CreateHFontRef(HFONT font);
		static HFontRef* CreateHFontRef(HFONT font, const TEXTMETRIC& font_metrics);
		static HFontRef* CreateDefalutHFontRef();

		static Font::HFontRef* base_font_ref_;

		HFontRef* font_ref_;
	};
}