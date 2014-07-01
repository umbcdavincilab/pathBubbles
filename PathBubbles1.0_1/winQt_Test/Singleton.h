#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include <cassert>

template<typename T>
class Singleton
{
public:
	Singleton()
	{
		assert( !_instance );
		if ( _instance == 0 )
		{
			int offset = (int)(T*)1 - (int)(Singleton <T>*)(T*)1;
			_instance = (T*)((int)this + offset);
		}
	}
	~Singleton()
	{
		_instance = 0;
	}
	static T& getInstance()
	{
		return (*_instance);
	}
	static T* getInstancePtr()
	{
		return _instance;
	}

protected:

private:
	static T* _instance; 
};

template <typename T> 
T* Singleton<T>::_instance = 0x0;

#endif