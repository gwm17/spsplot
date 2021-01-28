/*
	main.cpp
	Entry point for SPSPlot with a gui

	GWM Sep. 2020
*/

#include <string>
#include <iostream>
#include <TApplication.h>
#include "SPSPlotMainFrame.h"

int main(int argc, char** argv) {
	TApplication app("app", &argc, argv);
	UInt_t width = 500;
  	UInt_t height = 500;
  	new SPSPlotMainFrame(gClient->GetRoot(), width, height);
 	app.Run();
 	return 0;
}
