#include "stdafx.h"
#include "text_storage.h"

namespace ui
{
	class TextPagraphBuilder
	{
	public:
		TextPagraphBuilder(const wchar_t* begin, size_t len)
		{
			begin_ = begin;
			len_ = len;
			offset_ = 0;
		}

		TextPagraph* nextPagraph()
		{
			TextPagraph* p = NULL;
			while (!p && offset_ < len_)
			{
				if (begin_[offset_] == '\r' && begin_[offset_ + 1] == '\n') {
					offset_ += 2;
					p = buildTextPagraph(offset_);
				}
				else if (begin_[offset_] == '\r' || begin_[offset_] == '\n') {
					offset_++;
					p = buildTextPagraph(offset_);
				}
				else {
					offset_++;
				}
			}
			return p;
		}
	private:
		TextPagraph* buildTextPagraph(size_t pos)
		{
			TextPagraph* p = NULL;
			if (pagraph_begin_ < pos)
				p = new TextPagraph(begin_, pagraph_begin_, pos);
			pagraph_begin_ = pos;
			return p;
		}
		const wchar_t* begin_;
		size_t len_;
		size_t offset_;
		size_t pagraph_begin_;
	};

	TextStorage::TextStorage()
		: pagraph_(NULL)
	{

	}

	TextStorage::~TextStorage()
	{
		clearLayout();
		clear();
	}

	void TextStorage::setText(const std::wstring& text)
	{
		clear();
		text_ = text;
		needBuild_ = true;
	}


	void TextStorage::Render(UIRenderContext* context)
	{
// 		for (TextPagraph* pagraph = firstPagraph_; pagraph; pagraph = pagraph->nextPagraph_)
// 		{
// 			context->PushState();
// 			context->Translate(pagraph->x(), pagraph->y());
// 			pagraph->Render(context);
// 			context->PopState();
// 		}
	}

	void TextStorage::clear()
	{
		if (storageType_ != Type_Null && pagraph_)
		{
			if (storageType_ == Type_Pagraph)
				delete pagraph_;
			else
				delete document_;
			pagraph_ = NULL;
			storageType_ = Type_Null;
		}
	}

	void TextStorage::build()
	{
		if (!needBuild_)
			return;

		TextPagraphBuilder builder(text_.c_str(), text_.size());
		while (TextPagraph* p = builder.nextPagraph())
		{
			if (storageType_ == Type_Pagraph || storageType_ == Type_Null) {
				TextDocument* document = new TextDocument(text_.c_str(), text_.size());
				if (storageType_ == Type_Pagraph)
					document->addTextPagraph(pagraph_);
				document_ = document;
				storageType_ = Type_Document;
			}
			document_->addTextPagraph(p);
		}

		needBuild_ = false;
	}

	void TextStorage::Layout(const Size& size)
	{
		if (!needLayout_)
			return;

		clearLayout();
		if (storageType_ == Type_Pagraph)
		{
			layout_ = new UIGlyphLayout;
			layout_->addGlyphPagraph(pagraph_->buildGlyphPagraph());
		}
		else if (storageType_ == Type_Document)
		{
			layout_ = document_->buildGlyphLayout();
		}

		needLayout_ = false;
	}

	void TextStorage::clearLayout()
	{
		if (layout_)
		{
			delete layout_;
			layout_ = NULL;
		}
	}


}