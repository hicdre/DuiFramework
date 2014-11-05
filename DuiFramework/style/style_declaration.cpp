#include "stdafx.h"
#include "style_declaration.h"

namespace ui
{
	StyleDeclaration::StyleDeclaration(StyleProperty p)
		: property_(p), value_(new StyleValue)
	{
		value_->AddRef();
	}

	StyleDeclaration::StyleDeclaration(StyleProperty p, StyleValue* v)
		: property_(p), value_(v)
	{
		value_->AddRef();
	}

	StyleDeclaration::~StyleDeclaration()
	{
		if (value_)
			value_->Release();
		value_ = NULL;
	}

	StyleValue* StyleDeclaration::GetValue() const
	{
		return value_;
	}

	const char* StyleDeclaration::GetPropertyName() const
	{
		return StyleFindString(property_);
	}

	ui::StyleProperty StyleDeclaration::GetProperty() const
	{
		return property_;
	}


	void StyleDeclarationList::GetOrderedPropertyList(StylePropertyList& l)
	{
		std::set<StyleProperty> s;
		for (auto iter : container_)
		{
			s.insert(iter.first);
		}

		for (StyleProperty p : s)
		{
			l.push_back(p);
		}
	}

	void StyleDeclarationList::GetPropertyList(StylePropertyList& l)
	{
		for (auto iter : container_)
		{
			l.push_back(iter.first);
		}
	}

	void StyleDeclarationList::Insert(StyleDeclaration* declaration)
	{
		StyleProperty p = declaration->GetProperty();
		if (container_.count(p))
		{
			container_.at(p)->Release();
		}
		container_[p] = declaration;
		declaration->AddRef();
	}

	StyleValue* StyleDeclarationList::FindValue(StyleProperty p) const
	{
		if (container_.count(p))
			return container_.at(p)->GetValue();
		else
			return NULL;
	}

	StyleDeclaration* StyleDeclarationList::Find(StyleProperty p) const
	{
		if (container_.count(p))
			return container_.at(p);
		else
			return NULL;
	}

	int StyleDeclarationList::GetCount() const
	{
		return container_.size();
	}

	void StyleDeclarationList::Clear()
	{
		for (auto item : container_)
		{
			item.second->Release();
		}
		container_.clear();
	}

	StyleDeclarationList::~StyleDeclarationList()
	{
		Clear();
	}

	StyleDeclarationList::StyleDeclarationList()
	{

	}

}