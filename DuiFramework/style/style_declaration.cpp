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


// 	void StyleDeclarationList::GetOrderedPropertyList(StylePropertyList& l)
// 	{
// 		std::set<StyleProperty> s;
// 		for (auto iter : container_)
// 		{
// 			s.insert(iter.first);
// 		}
// 
// 		for (StyleProperty p : s)
// 		{
// 			l.push_back(p);
// 		}
// 	}

// 	void StyleDeclarationList::GetPropertyList(StylePropertyList& l)
// 	{
// 		for (auto iter : container_)
// 		{
// 			l.push_back(iter.first);
// 		}
// 	}

	void StyleDeclarationList::Insert(StyleDeclaration* declaration)
	{
		scoped_refptr<StyleDeclaration> p(declaration);
		container_.insert(p);
	}

	StyleValue* StyleDeclarationList::FindValue(StyleProperty p) const
	{
		for (const scoped_refptr<StyleDeclaration>& t : container_)
		{
			if (t->GetProperty() == p)
				return t->GetValue();

			if (t->GetProperty() > p)
				return NULL;
		}
		return NULL;
	}


	void StyleDeclarationList::Remove(StyleProperty p)
	{
		for (auto iter = container_.begin(); iter != container_.end(); iter++)
		{
			if ((*iter)->GetProperty() == p) {
				container_.erase(iter);
				break;
			}
		}
	}


	StyleDeclaration* StyleDeclarationList::Find(StyleProperty p) const
	{
		for (const scoped_refptr<StyleDeclaration>& t : container_)
		{
			if (t->GetProperty() == p)
				return t.get();

			if (t->GetProperty() > p)
				return NULL;
		}
		return NULL;
	}

	int StyleDeclarationList::GetCount() const
	{
		return container_.size();
	}

	void StyleDeclarationList::Clear()
	{
		container_.clear();
	}

	StyleDeclarationList::~StyleDeclarationList()
	{
		Clear();
	}

	StyleDeclarationList::StyleDeclarationList()
	{

	}

	void StyleDeclarationList::Swap(StyleDeclarationList* other)
	{
		container_.swap(other->container_);
	}

	void StyleDeclarationList::TransferTo(StyleDeclarationList* other)
	{
		for (const scoped_refptr<StyleDeclaration>& d : container_)
		{
			other->Insert(d.get());
		}
		Clear();
	}

}