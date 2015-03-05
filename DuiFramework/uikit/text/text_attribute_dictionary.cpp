#include "stdafx.h"
#include "text_attribute_dictionary.h"

namespace ui
{

	UITextAttributeValue::~UITextAttributeValue()
	{
		clear();
	}


	void UITextAttributeValue::setColorValue(Color c)
	{
		clear();
		colorValue_ = c;
		type_ = ColorType;
	}

	void UITextAttributeValue::setFontValue(UIFont* f)
	{
		clear();
		fontValue_ = f;
		fontValue_->AddRef();
		type_ = FontType;
	}

	void UITextAttributeValue::clear()
	{
		if (type_ == FontType) {
			assert(fontValue_);
			fontValue_->Release();
			fontValue_ = NULL;
		}
	}

	UITextAttributeValue::UITextAttributeValue(Color c)
		: colorValue_(c)
		, type_(ColorType)
	{

	}

	UITextAttributeValue::UITextAttributeValue(UIFont* f)
		: fontValue_(f)
		, type_(FontType)
	{
		assert(fontValue_);
		fontValue_->AddRef();
	}

	UITextAttributeValue* UITextAttributeValue::Copy() const
	{
		if (type_ == ColorType) {
			return new UITextAttributeValue(colorValue_);
		}
		else if (type_ == FontType) {
			return new UITextAttributeValue(fontValue_);
		}
	}

	bool UITextAttributeValue::isEqual(const UITextAttributeValue* val) const
	{
		if (type_ != val->type_)
			return false;
		if (type_ == ColorType)
			return colorValue_ == val->colorValue_;
		if (type_ == FontType)
			return fontValue_->IsEqual(val->fontValue_);
		assert(0);
		return false;
	}


	UITextAttributes::UITextAttributes()
	{

	}

	UITextAttributes::~UITextAttributes()
	{
		clear();
	}

	void UITextAttributes::allKeys(std::vector<UITextAttributeKey>& keys)
	{
		for (auto pair : container_)
		{
			keys.push_back(pair.first);
		}
	}

	void UITextAttributes::allKeysForValue(std::vector<UITextAttributeKey>& keys, UITextAttributeValue* val)
	{
		for (auto pair : container_)
		{
			if (pair.second->isEqual(val))
				keys.push_back(pair.first);
		}
	}

	UITextAttributeValue* UITextAttributes::valueForKey(UITextAttributeKey key)
	{
		auto iter = container_.find(key);
		if (iter == container_.end())
			return NULL;
		return iter->second;
	}

	void UITextAttributes::setValueForKey(UITextAttributeKey key, UITextAttributeValue* val)
	{
		auto iter = container_.find(key);
		if (iter != container_.end()) {
			delete iter->second;
		}
		iter->second = val;
	}

	void UITextAttributes::clear()
	{
		for (auto pair : container_)
		{
			delete pair.second;
		}
		container_.clear();
	}

	bool UITextAttributes::isEqual(UITextAttributes* dict)
	{
		if (container_.size() != dict->container_.size())
			return false;

		for (auto pair : container_)
		{
			UITextAttributeValue* value = dict->valueForKey(pair.first);
			if (value == NULL)
				return false;
			if (!value->isEqual(pair.second))
				return false;
		}
		return true;
	}

}