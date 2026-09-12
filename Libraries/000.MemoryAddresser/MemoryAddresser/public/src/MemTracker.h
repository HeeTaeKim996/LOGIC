#pragma once

#include <stdio.h>
#include <vector>
#include <typeinfo>
#include <string>
#include <new>
#include <utility>
#include "000.MemoryAddresser_setting.h"

using namespace std;




	class IMemCounter
	{
	public:
		virtual void printAll() const = 0;
		virtual void printNewState() const = 0;
		virtual void printAllocState() const = 0;
		virtual void printConstructState() const = 0;
	};
















	class MemCounterManager
	{
	public:
		static MemCounterManager* instance()
		{
			static MemCounterManager* _instance = new MemCounterManager();
			return _instance;
		}
	private:
		MemCounterManager() {}


	public:
		inline void registerCounter(IMemCounter* counter) 
		{
			_counters.push_back(counter); 
		}
		inline void printAll() 
		{ 
			if (_counters.empty())
			{
				printf("MemCounterManager : _counters is empty\n");
			}
			else
			{
				for (IMemCounter* counter : _counters) counter->printAll();
			}
		}
		inline void printNewState() 
		{ 
			if (_counters.empty())
			{
				printf("MemCounterManager : _counters is empty\n");
			}
			else
			{
				for (IMemCounter* counter : _counters) counter->printNewState();
			}
		}
		inline void printAllocState() 
		{
			if (_counters.empty())
			{
				printf("MemCounterManager : _counters is empty\n");
			}
			else
			{
				for (IMemCounter* counter : _counters) counter->printAllocState();
			}
		}
		inline void printConstructState() 
		{ 
			if (_counters.empty())
			{
				printf("MemCounterManager : _counters is empty\n");
			}
			else
			{
				for (IMemCounter* counter : _counters) counter->printConstructState();
			}
		}
	public:
		vector<IMemCounter*> _counters;

	};













	template<typename T>
	class MemCounter : public IMemCounter
	{
	public:
		inline static MemCounter<T>* instance()
		{
			static MemCounter<T>* _instance = new MemCounter();
			return _instance;
		}

	private:
		inline MemCounter() 
		{
			MemCounterManager::instance()->registerCounter(this);
			typeName = typeid(T).name();

			if (typeName.compare(0, 6, "class ") == 0)
			{
				typeName = typeName.substr(6);
			}
		}

	public:
		virtual void printAll() const
		{
			printf("%s : new(%zu), delete(%zu), alloc(%zu), dealloc(%zu), construct(%zu), destruct(%zu)\n",
				typeName.c_str(), newCount, deleteCount, allocateCount, deallocateCount, constructCount, destructCount);
		}
		virtual void printNewState() const
		{
			printf("%s : new(%zu), delete(%zu)\n", typeName.c_str(), newCount, deleteCount);
		}
		virtual void printAllocState() const
		{
			printf("%s : alloc(%zu), dealloc(%zu)\n", typeName.c_str(), allocateCount, deallocateCount);
		}
		virtual void printConstructState() const
		{
			printf("%s : construct(%zu), destruct(%zu)\n", typeName.c_str(), constructCount, destructCount);
		}

	public:
		void addNewCount() { ++newCount; }
		void addDeleteCount() { ++deleteCount; }

		void addAllocCount() { ++allocateCount; }
		void addDeallocCount() { ++deallocateCount; }

		void addConstructCount() { ++constructCount; }
		void addDestructCount() { ++destructCount; }

	private:
		size_t newCount = 0;
		size_t deleteCount = 0;

		size_t allocateCount = 0;
		size_t deallocateCount = 0;

		size_t constructCount = 0;
		size_t destructCount = 0;

		string typeName;
	};



	template<typename T, typename... Args>
	inline T* TrackNew(Args&&... args)
	{
#ifdef FLAG_USE_MEMRY_TRACKER
		MemCounter<T>::instance()->addNewCount();
#endif
		return new T(std::forward<Args>(args)...);
	}


	template<typename T>
	void TrackDelete(T*& ptr)
	{
		if (ptr != nullptr)
		{
#ifdef FLAG_USE_MEMRY_TRACKER
			MemCounter<T>::instance()->addDeleteCount();
#endif

			delete ptr;
			ptr = nullptr;
		}
	}

	template<typename T>
	inline T* TrackAllocate(size_t capacity)
	{
#ifdef FLAG_USE_MEMRY_TRACKER
		MemCounter<T>::instance()->addAllocCount();
#endif
		return static_cast<T*>(::operator new(capacity * sizeof(T)));
	}

	template<typename T>
	inline void TrackDeallocate(T* pos)
	{
#ifdef FLAG_USE_MEMRY_TRACKER
		MemCounter<T>::instance()->addDeallocCount();
#endif
		::operator delete(pos);
	}



	template<typename T, typename... Args>
	inline void TrackConstruct(T* pos, Args&&... args)
	{
#ifdef FLAG_USE_MEMRY_TRACKER
		MemCounter<T>::instance()->addConstructCount();
#endif
		::new(pos) T(std::forward<Args>(args)...);
	}

	template<typename T>
	inline void TrackDestruct(T* pos)
	{
#ifdef FLAG_USE_MEMRY_TRACKER
		MemCounter<T>::instance()->addDestructCount();
#endif
		pos->~T();
	}

