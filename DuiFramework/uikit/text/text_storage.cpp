#include "stdafx.h"
#include "text_storage.h"

namespace ui
{

	UITextStorage::UITextStorage()
	{

	}

	UITextStorage::~UITextStorage()
	{

	}

	void UITextStorage::setText(const std::wstring& text)
	{
		text_ = text;
		needBuild_ = true;
	}

	void UITextStorage::addTextPagraph(UITextPagraph* pagraph)
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

	void UITextStorage::clearTextPagraph()
	{
		UITextPagraph* pagraph = firstPagraph_;
		while (pagraph)
		{
			UITextPagraph* deletePagraph = pagraph;
			pagraph = pagraph->nextPagraph_;
			delete deletePagraph;
		}
		firstPagraph_ = lastPagraph_ = NULL;
		pagraphCount_ = 0;
	}

	void UITextStorage::buildTextPagraph()
	{
		if (!needBuild_)
			return;
		needBuild_ = false;
		size_t count = 0;
		size_t begin_pos = 0;
		while (count < text_.size())
		{
			if (text_[count] == '\r' && text_[count + 1] == '\n') {
				addTextPagraph(new UITextPagraph(text_.c_str(), begin_pos, count));
				begin_pos = count + 2;
				count++;
			}
			else if (text_[count] == '\r' || text_[count] == '\n') {
				addTextPagraph(new UITextPagraph(text_.c_str(), begin_pos, count));
				begin_pos = count + 1;
			}

			count++;
		}
		if (begin_pos < count)
			addTextPagraph(new UITextPagraph(text_.c_str(), begin_pos, count));
	}

	void UITextStorage::Layout(const Size& size)
	{
		buildTextPagraph();
		int y = 0;
		int max_width = 0;
		for (UITextPagraph* pagraph = firstPagraph_; pagraph; pagraph = pagraph->nextPagraph_)
		{
			pagraph->SetWidth(size.width());
			pagraph->SetPosition(0, y);
			Rect rc(pagraph->GetBoundsRect());
			y += rc.height();
			if (rc.width() > max_width)
				max_width = rc.width();
		}

		layoutSize_.SetSize(max_width, y);
	}

	Size UITextStorage::GetLayoutSize() const
	{
		return layoutSize_;
	}

	void UITextStorage::Render(UIRenderContext* context, Color color)
	{
		for (UITextPagraph* pagraph = firstPagraph_; pagraph; pagraph = pagraph->nextPagraph_)
		{
			context->PushState();
			context->Translate(pagraph->x(), pagraph->y());
			pagraph->Render(context);
			context->PopState();
		}
	}

}