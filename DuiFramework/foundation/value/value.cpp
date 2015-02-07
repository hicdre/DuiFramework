#include "stdafx.h"
#include "value.h"
#include "foundation/string/string.h"

//#include "utils/utils.h"
#include <cassert>

namespace ui
{
	namespace
	{
		Value* CopyWithoutEmptyChildren(const Value* node) {
			assert(node);
			switch (node->GetType()) {
			case Value::TYPE_LIST: {
				const ListValue* list = static_cast<const ListValue*>(node);
				ListValue* copy = new ListValue;
				for (ListValue::const_iterator it = list->begin(); it != list->end();
					++it) {
					Value* child_copy = CopyWithoutEmptyChildren(*it);
					if (child_copy)
						copy->Append(child_copy);
				}
				if (!copy->empty())
					return copy;

				delete copy;
				return NULL;
			}

			case Value::TYPE_DICTIONARY: {
				const DictionaryValue* dict = static_cast<const DictionaryValue*>(node);
				DictionaryValue* copy = new DictionaryValue;
				for (DictionaryValue::Iterator it(*dict); !it.IsAtEnd(); it.Advance()) {
					Value* child_copy = CopyWithoutEmptyChildren(&it.value());
					if (child_copy)
						copy->SetWithoutPathExpansion(it.key(), child_copy);
				}
				if (!copy->empty())
					return copy;

				delete copy;
				return NULL;
			}

			default:
				// For everything else, just make a copy.
				return node->DeepCopy();
			}
		}

		// A small functor for comparing Values for std::find_if and similar.
		class ValueEquals {
		public:
			// Pass the value against which all consecutive calls of the () operator will
			// compare their argument to. This Value object must not be destroyed while
			// the ValueEquals is  in use.
			explicit ValueEquals(const Value* first) : first_(first) { }

			bool operator ()(const Value* second) const {
				return first_->Equals(second);
			}

		private:
			const Value* first_;
		};

	}
	Value::~Value() {
	}

	// static
	Value* Value::CreateNullValue() {
		return new Value(TYPE_NULL);
	}

	bool Value::GetAsBoolean(bool* out_value) const {
		return false;
	}

	bool Value::GetAsInteger(int* out_value) const {
		return false;
	}

	bool Value::GetAsDouble(double* out_value) const {
		return false;
	}

	bool Value::GetAsString(std::string* out_value) const {
		return false;
	}

	bool Value::GetAsString(std::wstring* out_value) const {
		return false;
	}

	bool Value::GetAsList(ListValue** out_value) {
		return false;
	}

	bool Value::GetAsList(const ListValue** out_value) const {
		return false;
	}

	bool Value::GetAsDictionary(DictionaryValue** out_value) {
		return false;
	}

	bool Value::GetAsDictionary(const DictionaryValue** out_value) const {
		return false;
	}

	Value* Value::DeepCopy() const {
		// This method should only be getting called for null Values--all subclasses
		// need to provide their own implementation;.
		assert(IsType(TYPE_NULL));
		return CreateNullValue();
	}

	bool Value::Equals(const Value* other) const {
		// This method should only be getting called for null Values--all subclasses
		// need to provide their own implementation;.
		assert(IsType(TYPE_NULL));
		return other->IsType(TYPE_NULL);
	}

	// static
	bool Value::Equals(const Value* a, const Value* b) {
		if ((a == NULL) && (b == NULL)) return true;
		if ((a == NULL) ^ (b == NULL)) return false;
		return a->Equals(b);
	}

	Value::Value(Type type) : type_(type) {}

	Value::Value(const Value& that) : type_(that.type_) {}

	Value& Value::operator=(const Value& that) {
		type_ = that.type_;
		return *this;
	}

	///////////////////// FundamentalValue ////////////////////

	FundamentalValue::FundamentalValue(bool in_value)
		: Value(TYPE_BOOLEAN), boolean_value_(in_value) {
	}

	FundamentalValue::FundamentalValue(int in_value)
		: Value(TYPE_INTEGER), integer_value_(in_value) {
	}

	FundamentalValue::FundamentalValue(double in_value)
		: Value(TYPE_DOUBLE), double_value_(in_value) {
		if (!_finite(double_value_)) {
			assert(0);
			//NOTREACHED() << "Non-finite (i.e. NaN or positive/negative infinity) "
			//	<< "values cannot be represented in JSON";
			double_value_ = 0.0;
		}
	}

	FundamentalValue::~FundamentalValue() {
	}

	bool FundamentalValue::GetAsBoolean(bool* out_value) const {
		if (out_value && IsType(TYPE_BOOLEAN))
			*out_value = boolean_value_;
		return (IsType(TYPE_BOOLEAN));
	}

	bool FundamentalValue::GetAsInteger(int* out_value) const {
		if (out_value && IsType(TYPE_INTEGER))
			*out_value = integer_value_;
		return (IsType(TYPE_INTEGER));
	}

	bool FundamentalValue::GetAsDouble(double* out_value) const {
		if (out_value && IsType(TYPE_DOUBLE))
			*out_value = double_value_;
		else if (out_value && IsType(TYPE_INTEGER))
			*out_value = integer_value_;
		return (IsType(TYPE_DOUBLE) || IsType(TYPE_INTEGER));
	}

	FundamentalValue* FundamentalValue::DeepCopy() const {
		switch (GetType()) {
		case TYPE_BOOLEAN:
			return new FundamentalValue(boolean_value_);

		case TYPE_INTEGER:
			return new FundamentalValue(integer_value_);

		case TYPE_DOUBLE:
			return new FundamentalValue(double_value_);

		default:
			assert(0);
			return NULL;
		}
	}

	bool FundamentalValue::Equals(const Value* other) const {
		if (other->GetType() != GetType())
			return false;

		switch (GetType()) {
		case TYPE_BOOLEAN: {
			bool lhs, rhs;
			return GetAsBoolean(&lhs) && other->GetAsBoolean(&rhs) && lhs == rhs;
		}
		case TYPE_INTEGER: {
			int lhs, rhs;
			return GetAsInteger(&lhs) && other->GetAsInteger(&rhs) && lhs == rhs;
		}
		case TYPE_DOUBLE: {
			double lhs, rhs;
			return GetAsDouble(&lhs) && other->GetAsDouble(&rhs) && lhs == rhs;
		}
		default:
			assert(0);
			return false;
		}
	}

	///////////////////// StringValue ////////////////////

	StringValue::StringValue(const std::string& in_value)
		: Value(TYPE_STRING),
		value_(in_value) {
		//assert(IsStringUTF8(in_value));
	}

	StringValue::StringValue(const std::wstring& in_value)
		: Value(TYPE_STRING),
		value_(WideToMultiByte(in_value)) {
	}

	StringValue::~StringValue() {
	}

	bool StringValue::GetAsString(std::string* out_value) const {
		if (out_value)
			*out_value = value_;
		return true;
	}

	bool StringValue::GetAsString(std::wstring* out_value) const {
		if (out_value)
			*out_value = MultiByteToWide(value_);
		return true;
	}

	StringValue* StringValue::DeepCopy() const {
		return new StringValue(value_);
	}

	bool StringValue::Equals(const Value* other) const {
		if (other->GetType() != GetType())
			return false;
		std::string lhs, rhs;
		return GetAsString(&lhs) && other->GetAsString(&rhs) && lhs.compare(rhs) == 0;
	}
#if 0
	///////////////////// BinaryValue ////////////////////

	BinaryValue::BinaryValue()
		: Value(TYPE_BINARY),
		size_(0) {
	}

	BinaryValue::BinaryValue(scoped_ptr<char[]> buffer, size_t size)
		: Value(TYPE_BINARY),
		buffer_(buffer.Pass()),
		size_(size) {
	}

	BinaryValue::~BinaryValue() {
	}

	// static
	BinaryValue* BinaryValue::CreateWithCopiedBuffer(const char* buffer,
		size_t size) {
		char* buffer_copy = new char[size];
		memcpy(buffer_copy, buffer, size);
		scoped_ptr<char[]> scoped_buffer_copy(buffer_copy);
		return new BinaryValue(scoped_buffer_copy.Pass(), size);
	}

	BinaryValue* BinaryValue::DeepCopy() const {
		return CreateWithCopiedBuffer(buffer_.get(), size_);
	}

	bool BinaryValue::Equals(const Value* other) const {
		if (other->GetType() != GetType())
			return false;
		const BinaryValue* other_binary = static_cast<const BinaryValue*>(other);
		if (other_binary->size_ != size_)
			return false;
		return !memcmp(GetBuffer(), other_binary->GetBuffer(), size_);
	}
#endif
	///////////////////// DictionaryValue ////////////////////

	DictionaryValue::DictionaryValue()
		: Value(TYPE_DICTIONARY) {
	}

	DictionaryValue::~DictionaryValue() {
		Clear();
	}

	bool DictionaryValue::GetAsDictionary(DictionaryValue** out_value) {
		if (out_value)
			*out_value = this;
		return true;
	}

	bool DictionaryValue::GetAsDictionary(const DictionaryValue** out_value) const {
		if (out_value)
			*out_value = this;
		return true;
	}

	bool DictionaryValue::HasKey(const std::string& key) const {
		//assert(IsStringUTF8(key));
		ValueMap::const_iterator current_entry = dictionary_.find(key);
		assert((current_entry == dictionary_.end()) || current_entry->second);
		return current_entry != dictionary_.end();
	}

	void DictionaryValue::Clear() {
		ValueMap::iterator dict_iterator = dictionary_.begin();
		while (dict_iterator != dictionary_.end()) {
			delete dict_iterator->second;
			++dict_iterator;
		}

		dictionary_.clear();
	}

	void DictionaryValue::Set(const std::string& path, Value* in_value) {
		//assert(IsStringUTF8(path));
		assert(in_value);

		std::string current_path(path);
		DictionaryValue* current_dictionary = this;
		for (size_t delimiter_position = current_path.find('.');
			delimiter_position != std::string::npos;
			delimiter_position = current_path.find('.')) {
			// Assume that we're indexing into a dictionary.
			std::string key(current_path, 0, delimiter_position);
			DictionaryValue* child_dictionary = NULL;
			if (!current_dictionary->GetDictionary(key, &child_dictionary)) {
				child_dictionary = new DictionaryValue;
				current_dictionary->SetWithoutPathExpansion(key, child_dictionary);
			}

			current_dictionary = child_dictionary;
			current_path.erase(0, delimiter_position + 1);
		}

		current_dictionary->SetWithoutPathExpansion(current_path, in_value);
	}

	void DictionaryValue::SetBoolean(const std::string& path, bool in_value) {
		Set(path, new FundamentalValue(in_value));
	}

	void DictionaryValue::SetInteger(const std::string& path, int in_value) {
		Set(path, new FundamentalValue(in_value));
	}

	void DictionaryValue::SetDouble(const std::string& path, double in_value) {
		Set(path, new FundamentalValue(in_value));
	}

	void DictionaryValue::SetString(const std::string& path,
		const std::string& in_value) {
		Set(path, new StringValue(in_value));
	}

	void DictionaryValue::SetString(const std::string& path,
		const std::wstring& in_value) {
		Set(path, new StringValue(in_value));
	}

	void DictionaryValue::SetWithoutPathExpansion(const std::string& key,
		Value* in_value) {
		// If there's an existing value here, we need to delete it, because
		// we own all our children.
		std::pair<ValueMap::iterator, bool> ins_res =
			dictionary_.insert(std::make_pair(key, in_value));
		if (!ins_res.second) {
			assert(ins_res.first->second!=in_value);  // This would be bogus
			delete ins_res.first->second;
			ins_res.first->second = in_value;
		}
	}

	void DictionaryValue::SetBooleanWithoutPathExpansion(
		const std::string& path, bool in_value) {
		SetWithoutPathExpansion(path, new FundamentalValue(in_value));
	}

	void DictionaryValue::SetIntegerWithoutPathExpansion(
		const std::string& path, int in_value) {
		SetWithoutPathExpansion(path, new FundamentalValue(in_value));
	}

	void DictionaryValue::SetDoubleWithoutPathExpansion(
		const std::string& path, double in_value) {
		SetWithoutPathExpansion(path, new FundamentalValue(in_value));
	}

	void DictionaryValue::SetStringWithoutPathExpansion(
		const std::string& path, const std::string& in_value) {
		SetWithoutPathExpansion(path, new StringValue(in_value));
	}

	void DictionaryValue::SetStringWithoutPathExpansion(
		const std::string& path, const std::wstring& in_value) {
		SetWithoutPathExpansion(path, new StringValue(in_value));
	}

	bool DictionaryValue::Get(
		const std::string& path, const Value** out_value) const {
		//assert(IsStringUTF8(path));
		//  LOG(WARNING) << "\n1\n";
		std::string current_path(path);
		const DictionaryValue* current_dictionary = this;
		//  LOG(WARNING) << "\n2\n";
		for (size_t delimiter_position = current_path.find('.');
			delimiter_position != std::string::npos;
			delimiter_position = current_path.find('.')) {
			const DictionaryValue* child_dictionary = NULL;
			if (!current_dictionary->GetDictionary(
				current_path.substr(0, delimiter_position), &child_dictionary))
				return false;

			current_dictionary = child_dictionary;
			current_path.erase(0, delimiter_position + 1);
		}
		//  LOG(WARNING) << "\n3\n";

		return current_dictionary->GetWithoutPathExpansion(current_path, out_value);
	}

	bool DictionaryValue::Get(const std::string& path, Value** out_value)  {
		return static_cast<const DictionaryValue&>(*this).Get(
			path,
			const_cast<const Value**>(out_value));
	}

	bool DictionaryValue::GetBoolean(const std::string& path,
		bool* bool_value) const {
		const Value* value;
		if (!Get(path, &value))
			return false;

		return value->GetAsBoolean(bool_value);
	}

	bool DictionaryValue::GetInteger(const std::string& path,
		int* out_value) const {
		const Value* value;
		if (!Get(path, &value))
			return false;

		return value->GetAsInteger(out_value);
	}

	bool DictionaryValue::GetDouble(const std::string& path,
		double* out_value) const {
		const Value* value;
		if (!Get(path, &value))
			return false;

		return value->GetAsDouble(out_value);
	}

	bool DictionaryValue::GetString(const std::string& path,
		std::string* out_value) const {
		const Value* value;
		if (!Get(path, &value))
			return false;

		return value->GetAsString(out_value);
	}

	bool DictionaryValue::GetString(const std::string& path,
		std::wstring* out_value) const {
		const Value* value;
		if (!Get(path, &value))
			return false;

		return value->GetAsString(out_value);
	}
#if 0
	bool DictionaryValue::GetStringASCII(const std::string& path,
		std::string* out_value) const {
		std::string out;
		if (!GetString(path, &out))
			return false;

		if (!IsStringASCII(out)) {
			NOTREACHED();
			return false;
		}

		out_value->assign(out);
		return true;
	}

	bool DictionaryValue::GetBinary(const std::string& path,
		const BinaryValue** out_value) const {
		const Value* value;
		bool result = Get(path, &value);
		if (!result || !value->IsType(TYPE_BINARY))
			return false;

		if (out_value)
			*out_value = static_cast<const BinaryValue*>(value);

		return true;
	}

	bool DictionaryValue::GetBinary(const std::string& path,
		BinaryValue** out_value) {
		return static_cast<const DictionaryValue&>(*this).GetBinary(
			path,
			const_cast<const BinaryValue**>(out_value));
	}
#endif
	bool DictionaryValue::GetDictionary(const std::string& path,
		const DictionaryValue** out_value) const {
		const Value* value;
		bool result = Get(path, &value);
		if (!result || !value->IsType(TYPE_DICTIONARY))
			return false;

		if (out_value)
			*out_value = static_cast<const DictionaryValue*>(value);

		return true;
	}

	bool DictionaryValue::GetDictionary(const std::string& path,
		DictionaryValue** out_value) {
		return static_cast<const DictionaryValue&>(*this).GetDictionary(
			path,
			const_cast<const DictionaryValue**>(out_value));
	}

	bool DictionaryValue::GetList(const std::string& path,
		const ListValue** out_value) const {
		const Value* value;
		bool result = Get(path, &value);
		if (!result || !value->IsType(TYPE_LIST))
			return false;

		if (out_value)
			*out_value = static_cast<const ListValue*>(value);

		return true;
	}

	bool DictionaryValue::GetList(const std::string& path, ListValue** out_value) {
		return static_cast<const DictionaryValue&>(*this).GetList(
			path,
			const_cast<const ListValue**>(out_value));
	}

	bool DictionaryValue::GetWithoutPathExpansion(const std::string& key,
		const Value** out_value) const {
		//assert(IsStringUTF8(key));
		ValueMap::const_iterator entry_iterator = dictionary_.find(key);
		if (entry_iterator == dictionary_.end())
			return false;

		const Value* entry = entry_iterator->second;
		if (out_value)
			*out_value = entry;
		return true;
	}

	bool DictionaryValue::GetWithoutPathExpansion(const std::string& key,
		Value** out_value) {
		return static_cast<const DictionaryValue&>(*this).GetWithoutPathExpansion(
			key,
			const_cast<const Value**>(out_value));
	}

	bool DictionaryValue::GetBooleanWithoutPathExpansion(const std::string& key,
		bool* out_value) const {
		const Value* value;
		if (!GetWithoutPathExpansion(key, &value))
			return false;

		return value->GetAsBoolean(out_value);
	}

	bool DictionaryValue::GetIntegerWithoutPathExpansion(const std::string& key,
		int* out_value) const {
		const Value* value;
		if (!GetWithoutPathExpansion(key, &value))
			return false;

		return value->GetAsInteger(out_value);
	}

	bool DictionaryValue::GetDoubleWithoutPathExpansion(const std::string& key,
		double* out_value) const {
		const Value* value;
		if (!GetWithoutPathExpansion(key, &value))
			return false;

		return value->GetAsDouble(out_value);
	}

	bool DictionaryValue::GetStringWithoutPathExpansion(
		const std::string& key,
		std::string* out_value) const {
		const Value* value;
		if (!GetWithoutPathExpansion(key, &value))
			return false;

		return value->GetAsString(out_value);
	}

	bool DictionaryValue::GetStringWithoutPathExpansion(const std::string& key,
		std::wstring* out_value) const {
		const Value* value;
		if (!GetWithoutPathExpansion(key, &value))
			return false;

		return value->GetAsString(out_value);
	}

	bool DictionaryValue::GetDictionaryWithoutPathExpansion(
		const std::string& key,
		const DictionaryValue** out_value) const {
		const Value* value;
		bool result = GetWithoutPathExpansion(key, &value);
		if (!result || !value->IsType(TYPE_DICTIONARY))
			return false;

		if (out_value)
			*out_value = static_cast<const DictionaryValue*>(value);

		return true;
	}

	bool DictionaryValue::GetDictionaryWithoutPathExpansion(
		const std::string& key,
		DictionaryValue** out_value) {
		const DictionaryValue& const_this =
			static_cast<const DictionaryValue&>(*this);
		return const_this.GetDictionaryWithoutPathExpansion(
			key,
			const_cast<const DictionaryValue**>(out_value));
	}

	bool DictionaryValue::GetListWithoutPathExpansion(
		const std::string& key,
		const ListValue** out_value) const {
		const Value* value;
		bool result = GetWithoutPathExpansion(key, &value);
		if (!result || !value->IsType(TYPE_LIST))
			return false;

		if (out_value)
			*out_value = static_cast<const ListValue*>(value);

		return true;
	}

	bool DictionaryValue::GetListWithoutPathExpansion(const std::string& key,
		ListValue** out_value) {
		return
			static_cast<const DictionaryValue&>(*this).GetListWithoutPathExpansion(
			key,
			const_cast<const ListValue**>(out_value));
	}

	bool DictionaryValue::Remove(const std::string& path, Value** out_value) {
		//assert(IsStringUTF8(path));
		std::string current_path(path);
		DictionaryValue* current_dictionary = this;
		size_t delimiter_position = current_path.rfind('.');
		if (delimiter_position != std::string::npos) {
			if (!GetDictionary(current_path.substr(0, delimiter_position),
				&current_dictionary))
				return false;
			current_path.erase(0, delimiter_position + 1);
		}

		return current_dictionary->RemoveWithoutPathExpansion(current_path,
			out_value);
	}

	bool DictionaryValue::RemoveWithoutPathExpansion(const std::string& key,
		Value** out_value) {
		//assert(IsStringUTF8(key));
		ValueMap::iterator entry_iterator = dictionary_.find(key);
		if (entry_iterator == dictionary_.end())
			return false;

		Value* entry = entry_iterator->second;
		if (out_value)
			*out_value = entry;
		else
			delete entry;
		dictionary_.erase(entry_iterator);
		return true;
	}

	DictionaryValue* DictionaryValue::DeepCopyWithoutEmptyChildren() {
		Value* copy = CopyWithoutEmptyChildren(this);
		return copy ? static_cast<DictionaryValue*>(copy) : new DictionaryValue;
	}

	void DictionaryValue::MergeDictionary(const DictionaryValue* dictionary) {
		for (DictionaryValue::Iterator it(*dictionary); !it.IsAtEnd(); it.Advance()) {
			const Value* merge_value = &it.value();
			// Check whether we have to merge dictionaries.
			if (merge_value->IsType(Value::TYPE_DICTIONARY)) {
				DictionaryValue* sub_dict;
				if (GetDictionaryWithoutPathExpansion(it.key(), &sub_dict)) {
					sub_dict->MergeDictionary(
						static_cast<const DictionaryValue*>(merge_value));
					continue;
				}
			}
			// All other cases: Make a copy and hook it up.
			SetWithoutPathExpansion(it.key(), merge_value->DeepCopy());
		}
	}

	void DictionaryValue::Swap(DictionaryValue* other) {
		dictionary_.swap(other->dictionary_);
	}

	DictionaryValue::Iterator::Iterator(const DictionaryValue& target)
		: target_(target),
		it_(target.dictionary_.begin()) {}

	DictionaryValue* DictionaryValue::DeepCopy() const {
		DictionaryValue* result = new DictionaryValue;

		for (ValueMap::const_iterator current_entry(dictionary_.begin());
			current_entry != dictionary_.end(); ++current_entry) {
			result->SetWithoutPathExpansion(current_entry->first,
				current_entry->second->DeepCopy());
		}

		return result;
	}

	bool DictionaryValue::Equals(const Value* other) const {
		if (other->GetType() != GetType())
			return false;

		const DictionaryValue* other_dict =
			static_cast<const DictionaryValue*>(other);
		Iterator lhs_it(*this);
		Iterator rhs_it(*other_dict);
		while (!lhs_it.IsAtEnd() && !rhs_it.IsAtEnd()) {
			if (lhs_it.key() != rhs_it.key() ||
				!lhs_it.value().Equals(&rhs_it.value())) {
				return false;
			}
			lhs_it.Advance();
			rhs_it.Advance();
		}
		if (!lhs_it.IsAtEnd() || !rhs_it.IsAtEnd())
			return false;

		return true;
	}

	///////////////////// ListValue ////////////////////

	ListValue::ListValue() : Value(TYPE_LIST) {
	}

	ListValue::~ListValue() {
		Clear();
	}

	void ListValue::Clear() {
		for (ValueVector::iterator i(list_.begin()); i != list_.end(); ++i)
			delete *i;
		list_.clear();
	}

	bool ListValue::Set(size_t index, Value* in_value) {
		if (!in_value)
			return false;

		if (index >= list_.size()) {
			// Pad out any intermediate indexes with null settings
			while (index > list_.size())
				Append(CreateNullValue());
			Append(in_value);
		}
		else {
			assert(list_[index] != in_value);
			delete list_[index];
			list_[index] = in_value;
		}
		return true;
	}

	bool ListValue::Get(size_t index, const Value** out_value) const {
		if (index >= list_.size())
			return false;

		if (out_value)
			*out_value = list_[index];

		return true;
	}

	bool ListValue::Get(size_t index, Value** out_value) {
		return static_cast<const ListValue&>(*this).Get(
			index,
			const_cast<const Value**>(out_value));
	}

	bool ListValue::GetBoolean(size_t index, bool* bool_value) const {
		const Value* value;
		if (!Get(index, &value))
			return false;

		return value->GetAsBoolean(bool_value);
	}

	bool ListValue::GetInteger(size_t index, int* out_value) const {
		const Value* value;
		if (!Get(index, &value))
			return false;

		return value->GetAsInteger(out_value);
	}

	bool ListValue::GetDouble(size_t index, double* out_value) const {
		const Value* value;
		if (!Get(index, &value))
			return false;

		return value->GetAsDouble(out_value);
	}

	bool ListValue::GetString(size_t index, std::string* out_value) const {
		const Value* value;
		if (!Get(index, &value))
			return false;

		return value->GetAsString(out_value);
	}

	bool ListValue::GetString(size_t index, std::wstring* out_value) const {
		const Value* value;
		if (!Get(index, &value))
			return false;

		return value->GetAsString(out_value);
	}
#if 0
	bool ListValue::GetBinary(size_t index, const BinaryValue** out_value) const {
		const Value* value;
		bool result = Get(index, &value);
		if (!result || !value->IsType(TYPE_BINARY))
			return false;

		if (out_value)
			*out_value = static_cast<const BinaryValue*>(value);

		return true;
	}

	bool ListValue::GetBinary(size_t index, BinaryValue** out_value) {
		return static_cast<const ListValue&>(*this).GetBinary(
			index,
			const_cast<const BinaryValue**>(out_value));
	}
#endif
	bool ListValue::GetDictionary(size_t index,
		const DictionaryValue** out_value) const {
		const Value* value;
		bool result = Get(index, &value);
		if (!result || !value->IsType(TYPE_DICTIONARY))
			return false;

		if (out_value)
			*out_value = static_cast<const DictionaryValue*>(value);

		return true;
	}

	bool ListValue::GetDictionary(size_t index, DictionaryValue** out_value) {
		return static_cast<const ListValue&>(*this).GetDictionary(
			index,
			const_cast<const DictionaryValue**>(out_value));
	}

	bool ListValue::GetList(size_t index, const ListValue** out_value) const {
		const Value* value;
		bool result = Get(index, &value);
		if (!result || !value->IsType(TYPE_LIST))
			return false;

		if (out_value)
			*out_value = static_cast<const ListValue*>(value);

		return true;
	}

	bool ListValue::GetList(size_t index, ListValue** out_value) {
		return static_cast<const ListValue&>(*this).GetList(
			index,
			const_cast<const ListValue**>(out_value));
	}

	bool ListValue::Remove(size_t index, Value** out_value) {
		if (index >= list_.size())
			return false;

		if (out_value)
			*out_value = list_[index];
		else
			delete list_[index];

		list_.erase(list_.begin() + index);
		return true;
	}

	bool ListValue::Remove(const Value& value, size_t* index) {
		for (ValueVector::iterator i(list_.begin()); i != list_.end(); ++i) {
			if ((*i)->Equals(&value)) {
				size_t previous_index = i - list_.begin();
				delete *i;
				list_.erase(i);

				if (index)
					*index = previous_index;
				return true;
			}
		}
		return false;
	}

	ListValue::iterator ListValue::Erase(iterator iter, Value** out_value) {
		if (out_value)
			*out_value = *iter;
		else
			delete *iter;

		return list_.erase(iter);
	}

	void ListValue::Append(Value* in_value) {
		assert(in_value);
		list_.push_back(in_value);
	}

	void ListValue::AppendBoolean(bool in_value) {
		Append(new FundamentalValue(in_value));
	}

	void ListValue::AppendInteger(int in_value) {
		Append(new FundamentalValue(in_value));
	}

	void ListValue::AppendDouble(double in_value) {
		Append(new FundamentalValue(in_value));
	}

	void ListValue::AppendString(const std::string& in_value) {
		Append(new StringValue(in_value));
	}

	void ListValue::AppendString(const std::wstring& in_value) {
		Append(new StringValue(in_value));
	}

	void ListValue::AppendStrings(const std::vector<std::string>& in_values) {
		for (std::vector<std::string>::const_iterator it = in_values.begin();
			it != in_values.end(); ++it) {
			AppendString(*it);
		}
	}

	void ListValue::AppendStrings(const std::vector<std::wstring>& in_values) {
		for (std::vector<std::wstring>::const_iterator it = in_values.begin();
			it != in_values.end(); ++it) {
			AppendString(*it);
		}
	}

	bool ListValue::AppendIfNotPresent(Value* in_value) {
		assert(in_value);
		for (ValueVector::const_iterator i(list_.begin()); i != list_.end(); ++i) {
			if ((*i)->Equals(in_value)) {
				delete in_value;
				return false;
			}
		}
		list_.push_back(in_value);
		return true;
	}

	bool ListValue::Insert(size_t index, Value* in_value) {
		assert(in_value);
		if (index > list_.size())
			return false;

		list_.insert(list_.begin() + index, in_value);
		return true;
	}

	ListValue::const_iterator ListValue::Find(const Value& value) const {
		return std::find_if(list_.begin(), list_.end(), ValueEquals(&value));
	}

	void ListValue::Swap(ListValue* other) {
		list_.swap(other->list_);
	}

	bool ListValue::GetAsList(ListValue** out_value) {
		if (out_value)
			*out_value = this;
		return true;
	}

	bool ListValue::GetAsList(const ListValue** out_value) const {
		if (out_value)
			*out_value = this;
		return true;
	}

	ListValue* ListValue::DeepCopy() const {
		ListValue* result = new ListValue;

		for (ValueVector::const_iterator i(list_.begin()); i != list_.end(); ++i)
			result->Append((*i)->DeepCopy());

		return result;
	}

	bool ListValue::Equals(const Value* other) const {
		if (other->GetType() != GetType())
			return false;

		const ListValue* other_list =
			static_cast<const ListValue*>(other);
		const_iterator lhs_it, rhs_it;
		for (lhs_it = begin(), rhs_it = other_list->begin();
			lhs_it != end() && rhs_it != other_list->end();
			++lhs_it, ++rhs_it) {
			if (!(*lhs_it)->Equals(*rhs_it))
				return false;
		}
		if (lhs_it != end() || rhs_it != other_list->end())
			return false;

		return true;
	}
}