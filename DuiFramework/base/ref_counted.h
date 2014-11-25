#pragma once
#include "base\base_defines.h"

template <class T>
class RefCounted
{
public:
	RefCounted() {}
	~RefCounted() {}

	void AddRef() const
	{
		InterlockedIncrement(&_ref);
	}

	void Release() const
	{
		if (InterlockedDecrement(&_ref) == 0)
		{
			delete static_cast<const T*>(this);
		}
	}

private:
	mutable LONG _ref{ 0 };

	DISALLOW_COPY_AND_ASSIGN(RefCounted);
};