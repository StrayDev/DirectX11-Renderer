#include "include/Application.h"

int main()
{
	// create the application
	auto app = Application::MakeUnique();
	
	// init bruh
	app->Init();

	// process loop
	auto quit = app->Run();

	// return error code
	return quit;
}