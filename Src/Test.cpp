
#include "Singleton.h"
#include <iostream>
#include <memory>
#include <string>

// Test a Singleton
class Disk
{
public:
	API_SINGLETON( Disk );

	Disk()
	{
		std::cout << "Creating Disk.\n";
	}

	~Disk()
	{
		std::cout << "Destroying Disk\n";
	}

};

DEFINE_SINGLETON( Disk );

class Log
{
public:
	API_SINGLETON( Log );

	Log()
	{
		std::cout << "Creating Log.\n";

		//GetSingleton< Disk >();
	}

	~Log()
	{
		std::cout << "Destroying Log.\n";
	}

};

DEFINE_SINGLETON( Log );

class Keyboard
{
public:
	API_DYNAMIC_SINGLETON( Keyboard );
	Keyboard(const char* name)
	{
		mName = name;
	}

	~Keyboard()
	{
		std::cout << "Destroying keyboard: " << mName << "\n";
	}

	void print()
	{
		std::cout << "Keyboard name: " << mName << "\n";
	}

private:

	std::string mName;


};

DEFINE_DYNAMIC_SINGLETON( Keyboard );

int main()
{
	Log* log = getSingleton< Log >();
	Disk* disk = getSingleton< Disk >();

	// Keyboards are only alive during this function
	std::unique_ptr<Keyboard > k1 = std::make_unique< Keyboard >( "Keyboard 1" );
	std::unique_ptr< Keyboard > k2 = std::make_unique< Keyboard >( "Keyboard 2" );
	setSingleton< Keyboard >( k1.get() );
	getSingleton< Keyboard >()->print();
	setSingleton< Keyboard >( k2.get() );
	getSingleton< Keyboard >()->print();

	setSingleton< Keyboard >(nullptr);

	// Expected output:
/*	
	Creating Log.
	Creating Disk.
	Keyboard name : Keyboard 1
	Keyboard name : Keybaord 2
	Destroying keyboard : Keybaord 2
	Destroying keyboard : Keyboard 1
	Destroying Disk
	Destroying Log.
*/
}