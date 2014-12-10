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

	struct StyleDeclarationCompare
	{
		bool operator()(const scoped_refptr<StyleDeclaration> a,
			const scoped_refptr<StyleDeclaration> b) const {
			return a->GetProperty() < b->GetProperty();
		}
	};

	class StyleDeclarationList
	{
	public:
		typedef std::set<scoped_refptr<StyleDeclaration>, StyleDeclarationCompare> ContainerType;
		typedef ContainerType::iterator iterator;
		typedef ContainerType::const_iterator const_iterator;
		StyleDeclarationList();
		~StyleDeclarationList();

		void Clear();

		int GetCount() const;

		StyleDeclaration* Find(StyleProperty p) const;

		StyleValue* FindValue(StyleProperty p) const;

		void Insert(StyleDeclaration* declaration);
		void Remove(StyleProperty p);
		//void GetPropertyList(StylePropertyList& l);

		//void GetOrderedPropertyList(StylePropertyList& l);

		iterator begin() { return container_.begin(); }
		iterator end() { return container_.end(); }

		const_iterator begin() const { return container_.begin(); }
		const_iterator end() const { return container_.end(); }

		void Swap(StyleDeclarationList* other);
		void TransferTo(StyleDeclarationList* other);
	private:
		ContainerType container_;
		DISALLOW_COPY_AND_ASSIGN(StyleDeclarationList);
	};
}