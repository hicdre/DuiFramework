#include "stdafx.h"
#include "relative_layout.h"
#include "utils/utils.h"

namespace ui
{
	const char kRelativeLayout[] = "relative_layout_data";
	const char kRelativeTo[] = "relative.to";

	RelativeLayoutManager::RelativeLayoutManager()
	{

	}

	RelativeLayoutManager::~RelativeLayoutManager()
	{

	}

	void RelativeLayoutManager::Layout(View* host)
	{
		Views layot_views;
		for (View* v = host->first_child(); v; v = v->next_sibling())
		{
			if (!v->visible() || !IsRelativeLayout(v))
				continue;
			layot_views.push_back(v);
		}
	}

	bool RelativeLayoutManager::IsRelativeLayout(View* v)
	{
		RelativeLayoutData* data = GetLayoutData(v);
		if (data)
			return true;

		return ParseRelativeData(v);
	}

	RelativeLayoutData* RelativeLayoutManager::GetLayoutData(View* v)
	{
		return static_cast<RelativeLayoutData*>(v->GetData(kRelativeLayout));
	}

	bool RelativeLayoutManager::ParseRelativeData(View* v)
	{
		scoped_ptr<RelativePositionData> data;
		data->relative_to = RELATIVE_TO_PARENT;
		data->relative_position = RELATIVE_LEFT_TOP;
		data->x = data->y = 0;
		std::string relative_to_string;
		if (v->GetPropertyString(kRelativeTo, &relative_to_string))
		{
			std::vector<std::string> params;
			spilt_string(relative_to_string, ':', params, 2);
			if (params[0] == "parent")
				data->relative_to = RELATIVE_TO_PARENT;
			else if (params[0] == "prev")
				data->relative_to = RELATIVE_TO_PREV;

			if (params.size() == 2)
			{//有第二个参数
				if (params[1] == "leftTop")
					data->relative_position = RELATIVE_LEFT_TOP;
				else if (params[1] == "middleTop")
					data->relative_position = RELATIVE_MIDDLE_TOP;
				else if (params[1] == "rightTop")
					data->relative_position = RELATIVE_RIGHT_TOP;
				else if (params[1] == "leftMiddle")
					data->relative_position = RELATIVE_LEFT_MIDDLE;
				else if (params[1] == "middleMiddle" || params[1] == "center")
					data->relative_position = RELATIVE_MIDDLE_MIDDLE;
				else if (params[1] == "rightMiddle")
					data->relative_position = RELATIVE_RIGHT_MIDDLE;
				else if (params[1] == "leftBottom")
					data->relative_position = RELATIVE_LEFT_BOTTOM;
				else if (params[1] == "middleBottom")
					data->relative_position = RELATIVE_MIDDLE_BOTTOM;
				else if (params[1] == "rightBottom")
					data->relative_position = RELATIVE_RIGHT_BOTTOM;
			}
		}

		std::string x_string;
		if (v->GetPropertyString("x", &x_string))
		{
			data->x = atoi(x_string.c_str());
		}

		std::string y_string;
		if (v->GetPropertyString("x", &y_string))
		{
			data->y = atoi(y_string.c_str());
		}
		return true;
	}

}