#pragma once
#include "style/style_property.h"
#include "style/style_value.h"
#include <unordered_map>
#include <set>

namespace ui
{
	class StyleDeclaration : public RefCounted<StyleDeclaration>
	{
	public:
		StyleDeclaration(StyleProperty p, StyleValue* v);
		StyleDeclaration(StyleProperty p);

		StyleProperty GetProperty() const;
		const char* GetPropertyName() const;

		StyleValue* GetValue() const;
	private:
		friend class RefCounted < StyleDeclaration > ;
		~StyleDeclaration();
		StyleProperty property_;
		StyleValue* value_;
		DISALLOW_COPY_AND_ASSIGN(StyleDeclaration);
	};

	class StyleDeclarationList
	{
	public:
		StyleDeclarationList();
		~StyleDeclarationList();

		void Clear();

		int GetCount() const;

		StyleDeclaration* Find(StyleProperty p) const;

		StyleValue* FindValue(StyleProperty p) const;

		void Insert(StyleDeclaration* declaration);

		void GetPropertyList(StylePropertyList& l);

		void GetOrderedPropertyList(StylePropertyList& l);

	private:
		std::unordered_map<StyleProperty, StyleDeclaration*> container_;
	};
}