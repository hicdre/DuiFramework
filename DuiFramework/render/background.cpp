#include "stdafx.h"
#include "background.h"

#include "core/view.h"
#include "render/painter.h"
#include "utils/image_store.h"

namespace ui
{

	NormalBackground::NormalBackground()
	{

	}

	NormalBackground::NormalBackground(Color color)
		: color_(color)
	{

	}

	NormalBackground::NormalBackground(const std::string& id)
		: id_(id)
	{

	}

	void NormalBackground::SetColor(Color color)
	{
		color_ = color;
	}


	void NormalBackground::SetImageId(const std::string& id)
	{
		id_ = id;
	}


	void NormalBackground::DoPaint(View* view, Painter* painter)
	{
		if (!id_.empty())
		{
			ImageClip* clip = ImageStore::Default()->GetImageById(id_);
			if (clip) {
				painter->DrawImage(clip, view->GetLocalBounds());
				return;
			}
		}
			
		painter->FillRect(view->GetLocalBounds(), color_);
	}

}