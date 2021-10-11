//:=================------------ Engine Entry ------------=================://
#include <dcore/Launch.hpp>
#include <iostream>

int main(int argc, char *argv[]) {
	std::cout << "DCORE" << std::endl;
	dcore::launch::Launch l;
	l.Run(argc, argv);
}
