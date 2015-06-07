#include "Application.h"
#include <QCoreApplication>
#include <iostream>

int main(int argc, char *argv[])
{
	QCoreApplication qtApp(argc,argv);
	Application application(&qtApp,std::cout,std::cerr);
	return (int) application.run();
}
