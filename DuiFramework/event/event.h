#pragma once
#include "base/basictypes.h"
#include "core/constants.h"
#include "render/point.h"
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <list>
#include <set>

namespace ui
{

	class View;
	class Widget;
	class Event
	{
	public:
		Event();
		Event(EventType type);
		virtual ~Event();

		EventType type() const { return type_; }
		void SetEventType(EventType type) { type_ = type; }

		bool IsMouseEvent() const;
		bool IsKeyEvent() const;

		void StopDispatch();
		bool stopped_dispatch() const { return !is_continue_dispatch_; }

	protected:
		EventType type_;
		bool is_continue_dispatch_{ true }; //ÊÇ·ñ¼ÌÐøÃ°ÅÝ
		
	};

	Point GetMousePosition(Widget* v);
	//int GetMouseKeyFlags();

	typedef std::function<void(Event*)> EventCallback;

	class EventHandler
	{
	public:
		virtual ~EventHandler() {}
		virtual void HandleEvent(Event*) = 0;
	};

	class EventCallbackSet
	{
	public:
		void Insert(EventCallback c, uint32 group = 0);
		void Remove(uint32 group);
		void Invoke(Event* evt);
	private:
		struct Data{
			EventCallback c;
			uint32 group{ 0 };
		};
		std::vector<Data> container_;
	};

	class EventListener : public EventHandler
	{
	public:
		EventListener();
		virtual ~EventListener();

		virtual void HandleEvent(Event* evt) override;

		EventListener& Listen(EventType type, const EventCallback& c, uint32 group = 0);
		EventListener& UnListen(EventType type);
		EventListener& UnListen(EventType type, uint32 group);
		EventListener& UnListen(uint32 group);
	private:
		std::unordered_map<EventType, EventCallbackSet> listen_map_;
	};
}