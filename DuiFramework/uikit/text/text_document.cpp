#include "stdafx.h"
#include "text_document.h"
#include "glyph_layout.h"

namespace ui
{

	TextDocument::TextDocument(const wchar_t* text, size_t length)
	{
		setText(text, length);
	}

	TextDocument::~TextDocument()
	{
		clearTextPagraph();
	}

	void TextDocument::setText(const wchar_t* text, size_t length)
	{
		if (text_ == text && length_ == length)
			return;
		text_ = text;
		length_ = length;
		needBuildPagraph_ = true;
	}

	void TextDocument::buildPagraph()
	{
		if (!needBuildPagraph_)
			return;

		size_t index = 0;
		size_t begin_pos = 0;
		while (index < length_)
		{
			if (text_[index] == '\r' && text_[index + 1] == '\n') {
				addTextPagraph(new TextPagraph(text_, begin_pos, index));
				begin_pos = index + 2;
				index++;
			}
			else if (text_[index] == '\r' || text_[index] == '\n') {
				addTextPagraph(new TextPagraph(text_, begin_pos, index));
				begin_pos = index + 1;
			}

			index++;
		}
		if (begin_pos < index)
			addTextPagraph(new TextPagraph(text_, begin_pos, index));
		needBuildPagraph_ = false;
	}

	void TextDocument::addTextPagraph(TextPagraph* pagraph)
	{
		if (lastPagraph_) {
			lastPagraph_->nextPagraph_ = pagraph;
			pagraph->prevPagraph_ = lastPagraph_;
			pagraph->nextPagraph_ = NULL;
			lastPagraph_ = pagraph;
		}
		else {
			assert(firstPagraph_ == NULL);
			firstPagraph_ = lastPagraph_ = pagraph;
			pagraph->prevPagraph_ = pagraph->nextPagraph_ = NULL;
		}
		pagraphCount_++;
	}

	void TextDocument::clearTextPagraph()
	{
		TextPagraph* pagraph = firstPagraph_;
		while (pagraph)
		{
			TextPagraph* deletePagraph = pagraph;
			pagraph = pagraph->nextPagraph_;
			delete deletePagraph;
		}
		firstPagraph_ = lastPagraph_ = NULL;
		pagraphCount_ = 0;
	}

	UIGlyphLayout* TextDocument::buildGlyphLayout()
	{
		buildPagraph();
		if (firstPagraph_ == NULL)
			return NULL;
		UIGlyphLayout* layout = new UIGlyphLayout;
		for (TextPagraph* pagraph = firstPagraph_; pagraph; pagraph = pagraph->nextPagraph_)
		{
			layout->addGlyphPagraph(pagraph->buildGlyphPagraph());
		}
		return layout;
	}

}