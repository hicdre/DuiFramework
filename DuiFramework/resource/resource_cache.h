#pragma once
#include "base/basictypes.h"

namespace ui
{
	struct Resource : public RefCounted<Resource>
	{
		std::wstring resoure_path;
	};

	class ResourceCache
	{
	public:
		ResourceCache();
		~ResourceCache();

		Resource* Access(const std::wstring& path);

	private:
		void Update();
	};
}