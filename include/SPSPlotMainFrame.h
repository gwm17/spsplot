/*
	
	SPSPlotMainFrame.h
	Class which inherits from TGMainFrame ROOT class. Main class of the gui for SPSPlot. Direct interface to the SPSPlot class.
	Handles parameter updates and recieves file information from child frames. See ROOT CERN's user guide chapter on Writing a GUI
	for more details

	Written by G.W. McCann Sep. 2020

*/

#ifndef SPSPLOTMAINFRAME_H
#define SPSPLOTMAINFRAME_H

#include <TROOT.h>
#include <TGClient.h>
#include <TGWindow.h>
#include <TGFrame.h>
#include <TGNumberEntry.h>
#include <TGButton.h>
#include <TRootEmbeddedCanvas.h>
#include <TCanvas.h>
#include <TGMenu.h>
#include "SPSPlot.h"


class SPSPlotMainFrame : public TGMainFrame {
public:
	SPSPlotMainFrame(const TGWindow *p, UInt_t w, UInt_t h);
	virtual ~SPSPlotMainFrame();
	void CloseWindow();
	void HandleParameterChange();
	void HandleMenuSelection(int id);
	void UpdateKineSettings(double rmin, double rmax, double bke, double theta, double b);
	void DoPlot();
	void PlotGraphs();
	void LoadConfig(const char* name);
	void WriteConfig(const char* name);
	void AddReaction(Reaction* rxn);
	ClassDef(SPSPlotMainFrame, 0); //ROOT requirement

	enum MenuID {
		M_LOAD_CONFIG,
		M_SAVE_CONFIG,
		M_ADD_REACTION
	};

private:
	SPSPlot fPlotter;

	TGNumberEntryField *fBField, *fThetaField, *fBKEField, *fRMinField, *fRMaxField;
	TGTextButton *fPlotButton;

	TRootEmbeddedCanvas *fECanvas;
	TCanvas *fCanvas;

	TGPopupMenu *fFileMenu, *fRxnMenu;

	bool paramFlag; //false=params unchanged, true=params changed
	bool attachFlag; //false=no file attached, true=file attached

	UInt_t MAIN_H, MAIN_W;



};



#endif