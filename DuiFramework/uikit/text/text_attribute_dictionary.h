#pragma once
#include "foundation/foundation.h"
#include "uikit/common/ui_font.h"
#include <unordered_map>


namespace ui
{

	enum UITextAttributeKey {
		BackgroundColorAttributeKey = 0,
		TextColorAttributeKey,
		FontAttributeKey,

		CustomAttributeKeyBegin,
	};

	class UITextAttributeValue
	{
	public:
		enum Type {
			NullType = 0,
			ColorType,
			FontType,
		};
		UITextAttributeValue(Color c);
		UITextAttributeValue(UIFont* f);

		~UITextAttributeValue();

		UITextAttributeValue* Copy() const;

		bool isEqual(const UITextAttributeValue* val) const;

		Type valueType() const { return type_; }

		bool isColorValue() const {
			return type_ == ColorType;
		}
		bool isFontType() const {
			return type_ == FontType;
		}

		Color colorValue() const {
			assert(isColorValue());
			return colorValue_;
		}

		UIFont* fontValue() const {
			assert(isFontType());
			return fontValue_;
		}

		void setColorValue(Color c);
		void setFontValue(UIFont* f);

	private:
		void clear();
		Type type_;
		union {
			Color colorValue_;
			UIFont* fontValue_;
		};
	};


	class UITextAttributes
	{
	public:
		UITextAttributes();
		~UITextAttributes();

		bool isEqual(UITextAttributes* dict);

		void allKeys(std::vector<UITextAttributeKey>& keys);
		void allKeysForValue(std::vector<UITextAttributeKey>& keys, UITextAttributeValue* val);

		UITextAttributeValue* valueForKey(UITextAttributeKey key);
		void setValueForKey(UITextAttributeKey key, UITextAttributeValue* val);
	private:
		void clear();
		std::unordered_map<UITextAttributeKey, UITextAttributeValue*> container_;
	};
}