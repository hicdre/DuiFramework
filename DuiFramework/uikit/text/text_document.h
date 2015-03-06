#pragma once
#include "uikit/text/text_pagraph.h"

namespace ui
{
	class UIGlyphLayout;
	class TextDocument
	{
	public:
		TextDocument(const wchar_t* text, size_t length);
		~TextDocument();

		void setText(const wchar_t* text, size_t length);

		void addTextPagraph(TextPagraph* pagraph);
		void clearTextPagraph();

		UIGlyphLayout* buildGlyphLayout();
	private:
		void buildPagraph();
		TextPagraph* firstPagraph_{ NULL };
		TextPagraph* lastPagraph_{ NULL };
		size_t pagraphCount_{ 0 };
		bool needBuildPagraph_{ true };

		const wchar_t* text_{ NULL };
		size_t length_{ 0 };
		
	};
}