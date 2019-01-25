
#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include <assert.h>
#include <cstdlib>

#define ASSERT_MSG( expr, msg )\
	if (expr == false) ERROR_MSG( msg );
#define ASSERT( expr ) \
	if (expr == false ) ERROR_MSG( "Error" );
#define ERROR_MSG( msg )\
	assert( false );

#define API_SINGLETON(T)\
class SingletonTraits {\
public:\
	SingletonTraits() { ASSERT( mInstance == nullptr ); }\
	~SingletonTraits() { ASSERT( mInstance == nullptr ); }\
	static void create();\
	static T* mInstance;\
};\
SingletonTraits singletonTraits

#define API_DYNAMIC_SINGLETON(T)\
class SingletonTraits {\
public:\
	SingletonTraits() {}\
	~SingletonTraits() {}\
	static void create();\
	static void set( T* inst );\
	static T* mInstance;\
};\
SingletonTraits singletonTraits



#define DEFINE_SINGLETON( T ) \
void T::SingletonTraits::create()\
{\
	static_singleton_impl< T, StaticCreatePolicy< T > >::create();\
}\
T* T::SingletonTraits::mInstance = nullptr;\
singleton_impl< T >::State singleton_impl< T >::mState = singleton_impl<T>::State::Uninitialized;

#define DEFINE_DYNAMIC_SINGLETON( T ) \
void T::SingletonTraits::create()\
{\
	ERROR_MSG( "Dynamic Singleton was not set." );\
}\
T* T::SingletonTraits::mInstance = nullptr;


template<class T>
inline T* getSingleton() {
	using traits = T::SingletonTraits;
	if (traits::mInstance == nullptr)
	{
		traits::create();
	}
	return( traits::mInstance );
}


class SingletonStack
{
public:

	API_SINGLETON( SingletonStack );
	SingletonStack()
	{
		// have singleton stack get cleaned up on exit
		::atexit( onExit );
	}

	using TFunc = void(*)();
	void registerSingleton( TFunc func );
	static void onExit();

private:

};

template< typename T >
class StaticCreatePolicy
{
public:

	static T* create()
	{
		return(new T());
	}
};

class DynamicCreatePolicy
{};

class BasicThreadPolicy;

template< typename T >
class singleton_impl
{
public:

	enum class State
	{
		Uninitialized,
		Constructing,
		Constructed,
		Destroyed,
	};

	static void destroy()
	{
		T* inst = T::SingletonTraits::mInstance;
		T::SingletonTraits::mInstance = nullptr;
		delete inst;
	}


protected:

	static State mState;
};

template<typename T, typename TCreatePolicy,
	typename TThreadPolicy = BasicThreadPolicy>
class static_singleton_impl
	: public singleton_impl<T>
{
public:
	static void create()
	{
		using traits = T::SingletonTraits;
		T*& instance = traits::mInstance;
		State& state = mState;
		if (state == State::Constructing)
		{
			// Cyclic construction was found
			ERROR_MSG( "Cyclic singleton dependency detected");
		}
		else if (state == State::Destroyed)
		{
			// Accessing a dead singleton
			ERROR_MSG( "Accessing dead singleton");
		}
		else if (state == State::Uninitialized)
		{
			// must be in uninitialized state
			ASSERT(state == State::Uninitialized);
			// set constructing state
			state = State::Constructing;
			instance = TCreatePolicy::create();
			// after instance has completed construction, register it on the
			// singleton stack
			state = State::Constructed;
			getSingleton<SingletonStack>()->registerSingleton(destroy);
		}
		else
		{
			ERROR_MSG("Invalid Singleton State.");
		}
  }
};

template< typename T>
void setSingleton(T* inst)
{
	T::SingletonTraits::mInstance = inst;
}


#endif