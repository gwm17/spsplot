#include <string>
#include <iostream>
#include <TApplication.h>
#include "SPSPlot.h"

int main(int argc, char** argv) {
	if(argc != 2) {
		std::cerr<<"Incorrect number of arguments!"<<std::endl;
		return 1;
	}

	string name = argv[1];
	TApplication app("app", &argc, argv);
	SPSPlot plotter(name);
	plotter.DrawPlot();
}
