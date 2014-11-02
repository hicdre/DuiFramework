#pragma once
#include "base\basictypes.h"
#include <vector>


namespace ui
{
	class View;
	class RenderContext;
	class Container
	{
	public:
		Container();
		Container(View* parent);
		virtual ~Container();

		virtual int32 GetChildCount() const = 0;
		virtual void AddChild(View* v) = 0;
		virtual void RemoveChild(View* v) = 0;
		virtual View* GetView(int index) = 0;

		virtual void DoPaint(RenderContext* painter, const Rect& dest) = 0;
		virtual void Layout() = 0;
	protected:
		View* parent_;
	};

	class SequenceContainer : public Container
	{
	public:
		SequenceContainer();
		SequenceContainer(View* parent);

		virtual int32 GetChildCount() const override;
		virtual void AddChild(View* v) override;
		virtual void RemoveChild(View* v) override;
		virtual View* GetView(int index) override;
	protected:
		std::vector<View*> views_;
	};

	class AbsoulteContainer : public SequenceContainer
	{
	public:
		AbsoulteContainer();
		AbsoulteContainer(View* parent);
		virtual void DoPaint(RenderContext* painter, const Rect& dest) override;
		virtual void Layout() override;
	};
}