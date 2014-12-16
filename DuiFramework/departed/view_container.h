#pragma once
#include "base\basictypes.h"
#include <vector>


namespace ui
{
	enum LayoutType
	{
		AbsoulteLayout,
		HorizonalLayout,
		VerticalLayout,
	};

	class View;
	class RenderContext;
	class StyleSheetList;
	class Container
	{
	public:
		Container();
		Container(View* parent);
		virtual ~Container();

		virtual LayoutType layoutType() const = 0;

		virtual int32 GetChildCount() const = 0;
		virtual void AddChild(View* v) = 0;
		virtual void RemoveChild(View* v) = 0;
		virtual View* GetView(int index) = 0;

		virtual void DoPaint(RenderContext* painter, const Rect& dest) = 0;
		virtual void Layout() = 0;

		virtual void UpdateStyleRule(StyleSheetList* sheet) = 0;

		virtual int32 GetAutoWidth() = 0;
		virtual int32 GetAutoHeight() = 0;
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
		virtual void DoPaint(RenderContext* painter, const Rect& dest) override;
		virtual void UpdateStyleRule(StyleSheetList* sheet) override;
	protected:
		std::vector<View*> views_;
	};

	class AbsoulteContainer : public SequenceContainer
	{
	public:
		AbsoulteContainer();
		AbsoulteContainer(View* parent);
		virtual LayoutType layoutType() const override;
		virtual void Layout() override;
		virtual int32 GetAutoWidth() override;
		virtual int32 GetAutoHeight() override;
	};

	class HorizonalContainer : public SequenceContainer
	{
	public:
		HorizonalContainer();
		HorizonalContainer(View* parent);
		virtual LayoutType layoutType() const override;
		virtual void Layout() override;
		virtual int32 GetAutoWidth() override;
		virtual int32 GetAutoHeight() override;
	};

	class VerticalContainer : public SequenceContainer
	{
	public:
		VerticalContainer();
		VerticalContainer(View* parent);
		virtual LayoutType layoutType() const override;
		virtual void Layout() override;
		virtual int32 GetAutoWidth() override;
		virtual int32 GetAutoHeight() override;
	};
}