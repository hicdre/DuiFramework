#include "stdafx.h"
#include "ui_style_background.h"
#include "ui_styles.h"

namespace ui
{

	UIStyleBackground::UIStyleBackground()
		: styles_(NULL)
	{

	}

	UIStyleBackground::~UIStyleBackground()
	{

	}

	void UIStyleBackground::Init(UIStyles* styles)
	{
		styles_ = styles;
		InitBackgroundColor();
		InitBackgroundImage();
	}


	void UIStyleBackground::InitBackgroundColor()
	{
		StyleValue* color = styles_->FindProperty(Style_BackgroundColor);
		if (color && color->IsColorValue())
		{
			color_ = color->GetColorValue();
			return;
		}

		color_ = Color_Transparent;
	}


	void UIStyleBackground::InitBackgroundImage()
	{
		StyleValue* val = styles_->FindProperty(Style_BackgroundImage);
		if (!val || val->IsAutoValue() || val->IsNull())
			return;

		if (val->IsUrlValue()) {
			image_ = RenderEngine::CreateRenderImage(val->GetUrlValue());

			Size size = image_->GetSize();
			source_rect_.SetSize(size.width(), size.height());
			return;
		}

		if (val->IsFunctionValue()) {
			StyleValueFunction* functionValue = val->GetFunctionValue();
			if (functionValue->GetFunctionName() == "image-rect") {
				StyleValue* url = functionValue->GetParam(0);
				scoped_refptr<RenderImage> image = RenderEngine::CreateRenderImage(url->GetUrlValue());
				Size size = image->GetSize();

				do 
				{
					int x, y, width, height;
					if (!GetLengthFromValue(functionValue->GetParam(1), size.width(), x)
						|| !GetLengthFromValue(functionValue->GetParam(2), size.height(), y)
						|| !GetLengthFromValue(functionValue->GetParam(3), size.width(), width)
						|| !GetLengthFromValue(functionValue->GetParam(4), size.height(), height)) {
						break;
					}

					image_ = image;
					source_rect_.SetRect(x, y, width, height);

				} while (false);
			}
			//todo: LinearGradient
		}
	}

	const RenderImage* UIStyleBackground::image() const
	{
		return image_.get();
	}

	bool UIStyleBackground::GetLengthFromValue(StyleValue* v, int full, int &length)
	{
		if (v == NULL)
			return false;

		if (v->IsPixelValue()){
			length = v->GetPixel();
			return true;
		}

		if (v->IsPercentValue()) {
			length = v->GetPercentValue() * full;
			return true;
		}
		return false;
	}

}