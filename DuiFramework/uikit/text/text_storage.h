#pragma once
#include "uikit/text/text_pagraph.h"

namespace ui
{
	class UITextStorage
	{
	public:
		UITextStorage();
		~UITextStorage();

		void setText(const std::wstring& text);

		void addTextPagraph(UITextPagraph* pagraph);
		void clearTextPagraph();

		
		void Layout(const Size& size);
		Size GetLayoutSize() const;

		void Render(UIRenderContext* context, Color color);
	private:
		void buildTextPagraph();
		UITextPagraph* firstPagraph_{ NULL };
		UITextPagraph* lastPagraph_{ NULL };
		size_t pagraphCount_{ 0 };

		std::wstring text_;
		bool needBuild_{ true };
		Size layoutSize_;
	};
}