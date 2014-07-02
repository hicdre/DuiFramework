#pragma once
#include "render/rect.h"
#include "render/color.h"
#include "render/range.h"
#include "render/text_constants.h"
#include "render/font.h"
#include "render/painter.h"

#include <string>

namespace ui
{
	class RenderText 
	{
	public:
		virtual ~RenderText();

		// Creates a platform-specific or cross-platform RenderText instance.
		static RenderText* CreateInstance();

		const std::wstring& text() const { return text_; }
		void SetText(const std::wstring& text);

		HorizontalAlignment horizontal_alignment() const {
			return horizontal_alignment_;
		}
		void SetHorizontalAlignment(HorizontalAlignment alignment);

		//const FontList& font_list() const { return font_list_; }
		//void SetFontList(const FontList& font_list);
		const Font& font() const { return font_; }
		void SetFont(const Font& font);

		bool cursor_enabled() const { return cursor_enabled_; }
		void SetCursorEnabled(bool cursor_enabled);

		bool cursor_visible() const { return cursor_visible_; }
		void set_cursor_visible(bool visible) { cursor_visible_ = visible; }

		bool insert_mode() const { return insert_mode_; }
		void ToggleInsertMode();

		Color cursor_color() const { return cursor_color_; }
		void set_cursor_color(Color color) { cursor_color_ = color; }

		Color selection_color() const { return selection_color_; }
		void set_selection_color(Color color) { selection_color_ = color; }

		Color selection_background_focused_color() const {
			return selection_background_focused_color_;
		}
		void set_selection_background_focused_color(Color color) {
			selection_background_focused_color_ = color;
		}

		bool focused() const { return focused_; }
		void set_focused(bool focused) { focused_ = focused; }

		bool clip_to_display_rect() const { return clip_to_display_rect_; }
		void set_clip_to_display_rect(bool clip) { clip_to_display_rect_ = clip; }

		// In an obscured (password) field, all text is drawn as asterisks or bullets.
		bool obscured() const { return obscured_; }
		void SetObscured(bool obscured);

		// Makes a char in obscured text at |index| to be revealed. |index| should be
		// a UTF16 text index. If there is a previous revealed index, the previous one
		// is cleared and only the last set index will be revealed. If |index| is -1
		// or out of range, no char will be revealed. The revealed index is also
		// cleared when SetText or SetObscured is called.
		void SetObscuredRevealIndex(int index);

		// TODO(ckocagil): Multiline text rendering is currently only supported on
		// Windows. Support other platforms.
		bool multiline() const { return multiline_; }
		void SetMultiline(bool multiline);

		// Set the maximum length of the displayed layout text, not the actual text.
		// A |length| of 0 forgoes a hard limit, but does not guarantee proper
		// functionality of very long strings. Applies to subsequent SetText calls.
		// WARNING: Only use this for system limits, it lacks complex text support.
		void set_truncate_length(size_t length) { truncate_length_ = length; }

		// Elides the text to fit in |display_rect| according to the specified
		// |elide_behavior|. |ELIDE_MIDDLE| is not supported. If a truncate length and
		// an elide mode are specified, the shorter of the two will be applicable.
		void SetElideBehavior(ElideBehavior elide_behavior);

		const Rect& display_rect() const { return display_rect_; }
		void SetDisplayRect(const Rect& r);

		bool background_is_transparent() const { return background_is_transparent_; }
		void set_background_is_transparent(bool transparent) {
			background_is_transparent_ = transparent;
		}

		// Set the text color over the entire text or a logical character range.
		// The |range| should be valid, non-reversed, and within [0, text().length()].
		void SetColor(Color value);
		void ApplyColor(Color value, const Range& range);

		// Set various text styles over the entire text or a logical character range.
		// The respective |style| is applied if |value| is true, or removed if false.
		// The |range| should be valid, non-reversed, and within [0, text().length()].
		void SetStyle(TextStyle style, bool value);
		void ApplyStyle(TextStyle style, bool value, const Range& range);

		// Returns whether this style is enabled consistently across the entire
		// RenderText.
		bool GetStyle(TextStyle style) const;

		// Set or get the text directionality mode and get the text direction yielded.
		void SetDirectionalityMode(DirectionalityMode mode);
		DirectionalityMode directionality_mode() const {
			return directionality_mode_;
		}

		// Returns the size required to display the current string (which is the
		// wrapped size in multiline mode). The returned size does not include space
		// reserved for the cursor or the offset text shadows.
		Size GetStringSize();


		// Returns the width of the content (which is the wrapped width in multiline
		// mode). Reserves room for the cursor if |cursor_enabled_| is true.
		int GetContentWidth();

		// Returns the common baseline of the text. The return value is the vertical
		// offset from the top of |display_rect_| to the text baseline, in pixels.
		// The baseline is determined from the font list and display rect, and does
		// not depend on the text.
		int GetBaseline();

		void Draw(Painter* painter);

		// Given an |index| in text(), return the next or previous grapheme boundary
		// in logical order (i.e. the nearest cursorable index). The return value is
		// in the range 0 to text().length() inclusive (the input is clamped if it is
		// out of that range). Always moves by at least one character index unless the
		// supplied index is already at the boundary of the string.
		size_t IndexOfAdjacentGrapheme(size_t index);


		// Sets shadows to drawn with text.
		//void set_shadows(const ShadowValues& shadows) { shadows_ = shadows; }

		typedef std::pair<Font, Range> FontSpan;
		// For testing purposes, returns which fonts were chosen for which parts of
		// the text by returning a vector of Font and Range pairs, where each range
		// specifies the character range for which the corresponding font has been
		// chosen.
		std::vector<FontSpan> GetFontSpansForTesting();

		// Gets the horizontal bounds (relative to the left of the text, not the view)
		// of the glyph starting at |index|. If the glyph is RTL then the returned
		// Range will have is_reversed() true.  (This does not return a Rect because a
		// Rect can't have a negative width.)
		Range GetGlyphBounds(size_t index);

	protected:
		RenderText();

		const BreakList<Color>& colors() const { return colors_; }
		const std::vector<BreakList<bool> >& styles() const { return styles_; }

		const std::vector<internal::Line>& lines() const { return lines_; }
		void set_lines(std::vector<internal::Line>* lines) { lines_.swap(*lines); }

		// Returns the baseline of the current text.  The return value depends on
		// the text and its layout while the return value of GetBaseline() doesn't.
		// GetAlignmentOffset() takes into account the difference between them.
		//
		// We'd like a RenderText to show the text always on the same baseline
		// regardless of the text, so the text does not jump up or down depending
		// on the text.  However, underlying layout engines return different baselines
		// depending on the text.  In general, layout engines determine the minimum
		// bounding box for the text and return the baseline from the top of the
		// bounding box.  So the baseline changes depending on font metrics used to
		// layout the text.
		//
		// For example, suppose there are FontA and FontB and the baseline of FontA
		// is smaller than the one of FontB.  If the text is laid out only with FontA,
		// then the baseline of FontA may be returned.  If the text includes some
		// characters which are laid out with FontB, then the baseline of FontB may
		// be returned.
		//
		// GetBaseline() returns the fixed baseline regardless of the text.
		// GetLayoutTextBaseline() returns the baseline determined by the underlying
		// layout engine, and it changes depending on the text.  GetAlignmentOffset()
		// returns the difference between them.
		virtual int GetLayoutTextBaseline() = 0;

		const Vector2d& GetUpdatedDisplayOffset();

		void set_cached_bounds_and_offset_valid(bool valid) {
			cached_bounds_and_offset_valid_ = valid;
		}

		// Get the selection model that visually neighbors |position| by |break_type|.
		// The returned value represents a cursor/caret position without a selection.
		SelectionModel GetAdjacentSelectionModel(const SelectionModel& current,
			BreakType break_type,
			VisualCursorDirection direction);

		// Get the selection model visually left/right of |selection| by one grapheme.
		// The returned value represents a cursor/caret position without a selection.
		virtual SelectionModel AdjacentCharSelectionModel(
			const SelectionModel& selection,
			VisualCursorDirection direction) = 0;

		// Get the selection model visually left/right of |selection| by one word.
		// The returned value represents a cursor/caret position without a selection.
		virtual SelectionModel AdjacentWordSelectionModel(
			const SelectionModel& selection,
			VisualCursorDirection direction) = 0;

		// Get the SelectionModels corresponding to visual text ends.
		// The returned value represents a cursor/caret position without a selection.
		SelectionModel EdgeSelectionModel(VisualCursorDirection direction);

		// Sets the selection model, the argument is assumed to be valid.
		virtual void SetSelectionModel(const SelectionModel& model);

		// Get the visual bounds containing the logical substring within the |range|.
		// If |range| is empty, the result is empty. These bounds could be visually
		// discontinuous if the substring is split by a LTR/RTL level change.
		// These bounds are in local coordinates, but may be outside the visible
		// region if the text is longer than the textfield. Subsequent text, cursor,
		// or bounds changes may invalidate returned values.
		virtual std::vector<Rect> GetSubstringBounds(const Range& range) = 0;

		// Convert between indices into |text_| and indices into |obscured_text_|,
		// which differ when the text is obscured. Regardless of whether or not the
		// text is obscured, the character (code point) offsets always match.
		virtual size_t TextIndexToLayoutIndex(size_t index) const = 0;
		virtual size_t LayoutIndexToTextIndex(size_t index) const = 0;

		// Reset the layout to be invalid.
		virtual void ResetLayout() = 0;

		// Ensure the text is laid out, lines are computed, and |lines_| is valid.
		virtual void EnsureLayout() = 0;

		// Draw the text.
		virtual void DrawVisualText(Canvas* canvas) = 0;

		// Returns the text used for layout, which may be obscured or truncated.
		const base::string16& GetLayoutText() const;

		// Returns layout text positions that are suitable for breaking lines.
		const BreakList<size_t>& GetLineBreaks();

		// Apply (and undo) temporary composition underlines and selection colors.
		void ApplyCompositionAndSelectionStyles();
		void UndoCompositionAndSelectionStyles();

		// Returns the line offset from the origin after applying the text alignment
		// and the display offset.
		Vector2d GetLineOffset(size_t line_number);

		// Convert points from the text space to the view space and back. Handles the
		// display area, display offset, application LTR/RTL mode and multiline.
		Point ToTextPoint(const Point& point);
		Point ToViewPoint(const Point& point);

		// Convert a text space x-coordinate range to rects in view space.
		std::vector<Rect> TextBoundsToViewBounds(const Range& x);

		// Returns the line offset from the origin, accounts for text alignment only.
		Vector2d GetAlignmentOffset(size_t line_number);

		// Applies fade effects to |renderer|.
		void ApplyFadeEffects(internal::SkiaTextRenderer* renderer);

		// Applies text shadows to |renderer|.
		void ApplyTextShadows(internal::SkiaTextRenderer* renderer);

		// A convenience function to check whether the glyph attached to the caret
		// is within the given range.
		static bool RangeContainsCaret(const Range& range,
			size_t caret_pos,
			LogicalCursorDirection caret_affinity);

	private:
		// Creates a platform-specific RenderText instance.
		static RenderText* CreateNativeInstance();

		// Set the cursor to |position|, with the caret trailing the previous
		// grapheme, or if there is no previous grapheme, leading the cursor position.
		// If |select| is false, the selection start is moved to the same position.
		// If the |position| is not a cursorable position (not on grapheme boundary),
		// it is a NO-OP.
		void MoveCursorTo(size_t position, bool select);

		// Updates |layout_text_| if the text is obscured or truncated.
		void UpdateLayoutText();

		// Elides |text| to fit in the |display_rect_| with given |elide_behavior_|.
		// See ElideText in ui/gfx/text_elider.cc for reference.
		base::string16 ElideText(const base::string16& text);

		// Update the cached bounds and display offset to ensure that the current
		// cursor is within the visible display area.
		void UpdateCachedBoundsAndOffset();

		// Draw the selection.
		void DrawSelection(Canvas* canvas);

		// Logical UTF-16 string data to be drawn.
		base::string16 text_;

		// Horizontal alignment of the text with respect to |display_rect_|.  The
		// default is to align left if the application UI is LTR and right if RTL.
		HorizontalAlignment horizontal_alignment_;

		// The text directionality mode, defaults to DIRECTIONALITY_FROM_TEXT.
		DirectionalityMode directionality_mode_;

		// The cached text direction, potentially computed from the text or UI locale.
		// Use GetTextDirection(), do not use this potentially invalid value directly!
		base::i18n::TextDirection text_direction_;

		// A list of fonts used to render |text_|.
		//FontList font_list_;
		Font font_;

		// Logical selection range and visual cursor position.
		//SelectionModel selection_model_;

		// The cached cursor bounds; get these bounds with GetUpdatedCursorBounds.
		//Rect cursor_bounds_;

		// Specifies whether the cursor is enabled. If disabled, no space is reserved
		// for the cursor when positioning text.
		bool cursor_enabled_;

		// The cursor visibility and insert mode.
		bool cursor_visible_;
		bool insert_mode_;

		// The color used for the cursor.
		Color cursor_color_;

		// The color used for drawing selected text.
		Color selection_color_;

		// The background color used for drawing the selection when focused.
		Color selection_background_focused_color_;

		// The focus state of the text.
		bool focused_;

		// Composition text range.
		Range composition_range_;

		// Color and style breaks, used to color and stylize ranges of text.
		// BreakList positions are stored with text indices, not layout indices.
		// TODO(msw): Expand to support cursor, selection, background, etc. colors.
		BreakList<Color> colors_;
		std::vector<BreakList<bool> > styles_;

		// Breaks saved without temporary composition and selection styling.
		BreakList<Color> saved_colors_;
		BreakList<bool> saved_underlines_;
		bool composition_and_selection_styles_applied_;

		// A flag to obscure actual text with asterisks for password fields.
		bool obscured_;
		// The index at which the char should be revealed in the obscured text.
		int obscured_reveal_index_;

		// The maximum length of text to display, 0 forgoes a hard limit.
		size_t truncate_length_;

		// The behavior for eliding, fading, or truncating.
		ElideBehavior elide_behavior_;

		// The obscured and/or truncated text that will be displayed.
		std::wstring layout_text_;

		// Whether the text should be broken into multiple lines. Uses the width of
		// |display_rect_| as the width cap.
		bool multiline_;

		// Is the background transparent (either partially or fully)?
		bool background_is_transparent_;

		// The local display area for rendering the text.
		Rect display_rect_;

		// Flag to work around a Skia bug with the PDF path (http://crbug.com/133548)
		// that results in incorrect clipping when drawing to the document margins.
		// This field allows disabling clipping to work around the issue.
		// TODO(asvitkine): Remove this when the underlying Skia bug is fixed.
		bool clip_to_display_rect_;

		// The offset for the text to be drawn, relative to the display area.
		// Get this point with GetUpdatedDisplayOffset (or risk using a stale value).
		Vector2d display_offset_;

		// The baseline of the text.  This is determined from the height of the
		// display area and the cap height of the font list so the text is vertically
		// centered.
		int baseline_;

		// The cached bounds and offset are invalidated by changes to the cursor,
		// selection, font, and other operations that adjust the visible text bounds.
		bool cached_bounds_and_offset_valid_;

		// Text shadows to be drawn.
		//ShadowValues shadows_;

		// A list of valid layout text line break positions.
		BreakList<size_t> line_breaks_;

		// Lines computed by EnsureLayout. These should be invalidated with
		// ResetLayout and on |display_rect_| changes.
		std::vector<internal::Line> lines_;

		DISALLOW_COPY_AND_ASSIGN(RenderText);
	};
}