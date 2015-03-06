#pragma once
#include "uikit/text/text_document.h"
#include "uikit/text/glyph_layout.h"

namespace ui
{
	//根据内容变换不同的数据储存结构
	class TextStorage
	{
	public:
		TextStorage();
		~TextStorage();

		void setText(const std::wstring& text);

		void addTextPagraph(TextPagraph* pagraph);
		void clearTextPagraph();

 		void Layout(const Size& size);
// 		Size GetLayoutSize() const;
// 
 		void Render(UIRenderContext* context);
	private:
		enum InternalStorageType{
			Type_Null = 0,
			Type_Pagraph,
			Type_Document,
		};
		void clear();
		void build();
		void clearLayout();


		std::wstring text_;

		union {
			TextPagraph* pagraph_;//样式不一致，一段
			TextDocument* document_;//多段
		};
		InternalStorageType storageType_{ Type_Null };
		bool needBuild_{ true };

		UIGlyphLayout* layout_{ NULL };
		bool needLayout_{ true };
	};
}