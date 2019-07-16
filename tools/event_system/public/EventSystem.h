#pragma once

#include <functional>
#include <vector>
#include <algorithm>

namespace event_system
{


#pragma region events marcros

#define DECLARE_EVENT_OneParam(Name,Type,PramaName1) event_system::Event_OneParam<Type> Name

#define DECLARE_EVENT_TwoParams(Name,Type1,PramaName1,Type2,PramaName2) event_system::Event_TwoParam<Type1,Type2> Name


#define BIND_MEMBER_FUNCTION_Signal(signal_name,function,member)  (signal_name).bind(std::bind(&function, member));

#define BIND_MEMBER_FUNCTION_OneParam(event_name,function,member)  (event_name).bind(std::bind(&function, member, std::placeholders::_1))

#define BIND_MEMBER_FUNCTION_TwoParams(event_name,function,member)  (event_name).bind(std::bind(&function, member, std::placeholders::_1, std::placeholders::_2));

#define BIND_MEMBER_FUNCTION_TwoParamsCustomParamValue(event_name,function,member,param_1,param_2)  (event_name).bind(std::bind(&function, member, param_1,param_2));


#pragma endregion


#pragma region signal

	using CallbackHandler = int;

	class Signal
	{

	private:

		struct EventWrapper
		{
			std::function<void()> fun;
			CallbackHandler id;

			EventWrapper(std::function<void()> f) : fun{ f }
			{
				static int HandlerCounter;
				id = HandlerCounter++;
			}

		};


	public:

		void operator()()
		{
			for (auto call : _callbackList)
			{
				if (call.fun != nullptr)
				{
					call.fun();
				}
			}
		}

		CallbackHandler bind(std::function<void()> fun)
		{
			EventWrapper wrapper(fun);
			_callbackList.push_back(wrapper);
			return wrapper.id;
		}


		bool unbind(CallbackHandler callbackID)
		{

			auto iterator = (std::remove_if(_callbackList.begin(), _callbackList.end(),
				[callbackID](EventWrapper & x)
			{
				return x.id == callbackID;
			}));

			if (iterator != _callbackList.end())
			{
				_callbackList.erase(iterator);
				return true;
			}
			else
			{
				return false;
			}
		}

		void unbind_all()
		{
			_callbackList.clear();
		}

	private:

		std::vector <EventWrapper> _callbackList;
	};


	template<class T>
	class Event_OneParam
	{
	private:

		using TemplatedFunction = std::function<void(T)>;

		struct EventWrapper
		{
			TemplatedFunction fun;
			CallbackHandler id;

			EventWrapper(TemplatedFunction f) : fun{ f }
			{
				static int HandlerCounter;
				id = HandlerCounter++;
			}

		};

	
	public:
		void operator()(T  arg)
		{
			for (auto call : _callbackList)
			{
				if (call.fun != nullptr)
				{
					call.fun(arg);
				}
			}
		}

		CallbackHandler bind(TemplatedFunction fun)
		{
			EventWrapper wrapper(fun);
			_callbackList.push_back(wrapper);
			return wrapper.id;
		}


		bool unbind(CallbackHandler callbackID)
		{

			auto iterator = (std::remove_if(_callbackList.begin(), _callbackList.end(),
				[callbackID](EventWrapper & x)
			{
				return x.id == callbackID;
			}));

			if (iterator != _callbackList.end())
			{
				_callbackList.erase(iterator);
				return true;
			}
			else
			{
				return false;
			}
		}

		void unbind_all()
		{
			_callbackList.clear();
		}

	private:

		std::vector <EventWrapper> _callbackList;
	};

#pragma endregion


#pragma region events
	template<class T1, class T2  >
	class Event_TwoParam
	{
	private:

		using TemplatedFunction = std::function<void(T1, T2)>;

		struct EventWrapper
		{
			TemplatedFunction fun;
			CallbackHandler id;

			EventWrapper(TemplatedFunction f) : fun{ f }
			{
				static int HandlerCounter;
				id = HandlerCounter++;
			}

		};


	public:
		void operator()(T1  arg1, T2  arg2)
		{
			for (auto call : _callbackList)
			{
				if (call.fun != nullptr)
				{
					call.fun(arg1, arg2);
				}
			}
		}

		CallbackHandler bind(TemplatedFunction fun)
		{
			EventWrapper wrapper(fun);
			_callbackList.push_back(wrapper);
			return wrapper.id;
		}


		bool unbind(CallbackHandler callbackID)
		{

			auto iterator = (std::remove_if(_callbackList.begin(), _callbackList.end(),
				[callbackID](EventWrapper & x)
			{
				return x.id == callbackID;
			}));

			if (iterator != _callbackList.end())
			{
				_callbackList.erase(iterator);
				return true;
			}
			else
			{
				return false;
			}
		}

		void unbind_all()
		{
			_callbackList.clear();
		}

	private:

		std::vector <EventWrapper> _callbackList;
	};


#pragma endregion

}