#pragma once
#include "base/basictypes.h"

namespace ui
{
	/*
		RenderTree
	*/
	class RenderObject
	{
	public:
		RenderObject();
		~RenderObject();

		RenderObject* parent() const;
		bool isDescendantOf(const RenderObject*) const;

		RenderObject* firstChild() const;
		RenderObject* lastChild() const;
		RenderObject* previousSibling() const;
		RenderObject* nextSibling() const;


	private:
		RenderObject* parent_;
		RenderObject* first_;
		RenderObject* last_;
		RenderObject* prev_;
		RenderObject* next_;
	};
}