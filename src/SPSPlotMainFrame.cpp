/*
	
	SPSPlotMainFrame.cpp
	Class which inherits from TGMainFrame ROOT class. Main class of the gui for SPSPlot. Direct interface to the SPSPlot class.
	Handles parameter updates and recieves file information from child frames. See ROOT CERN's user guide chapter on Writing a GUI
	for more details

	Written by G.W. McCann Sep. 2020

*/

#include "SPSPlotMainFrame.h"
#include <TGLabel.h>
#include <TApplication.h>
#include <iostream>
#include <string>
#include "FileViewFrame.h"
#include "ReactionCreationFrame.h"

SPSPlotMainFrame::SPSPlotMainFrame(const TGWindow *p, UInt_t w, UInt_t h) :
	TGMainFrame(p, w, h), paramFlag(false), attachFlag(false)
{

	SetCleanup(kDeepCleanup); //ensures that all child frames are deleted
	MAIN_H = h; MAIN_W = w; //size all frames relative to the main frame

	TGVerticalFrame *CanvasFrame = new TGVerticalFrame(this, w, h*0.75); //holds canvas
	TGHorizontalFrame *EditFrame = new TGHorizontalFrame(this, w, h*0.25); //holds options

	/*Hints for arranging frames*/
	TGLayoutHints *fhints = new TGLayoutHints(kLHintsCenterX|kLHintsCenterY, 5,5,5,5);
	TGLayoutHints *chints = new TGLayoutHints(kLHintsExpandX|kLHintsExpandY, 5,5,5,5);
	TGLayoutHints *ehints = new TGLayoutHints(kLHintsExpandX|kLHintsBottom, 5,5,5,5);
	TGLayoutHints *lhints = new TGLayoutHints(kLHintsTop|kLHintsLeft, 5,5,0,0);
	TGLayoutHints *mhints = new TGLayoutHints(kLHintsTop|kLHintsLeft,0,4,0,0);

	/*Generate an embedded canvas and then obtain the tcanvas for plotting*/
	TGLabel *clabel = new TGLabel(CanvasFrame, "SPSPlot");
	fECanvas = new TRootEmbeddedCanvas("ECanvas", CanvasFrame, w, h*0.95);
	fCanvas = fECanvas->GetCanvas();
	fCanvas->SetCrosshair();
	CanvasFrame->AddFrame(clabel, lhints);
	CanvasFrame->AddFrame(fECanvas, chints);

	/*Create all of the paramter options with labels*/
	TGVerticalFrame *RhoMinFrame = new TGVerticalFrame(EditFrame, w*0.16, h*0.25);
	TGLabel *rminlabel = new TGLabel(RhoMinFrame, "RhoMin (cm)");
	fRMinField = new TGNumberEntryField(RhoMinFrame, 1, 0, TGNumberEntry::kNESRealFour, TGNumberEntry::kNEANonNegative);
	fRMinField->Connect("TextChanged(const char*)","SPSPlotMainFrame",this,"HandleParameterChange()");
	RhoMinFrame->AddFrame(rminlabel, lhints);
	RhoMinFrame->AddFrame(fRMinField, fhints);

	TGVerticalFrame *RhoMaxFrame = new TGVerticalFrame(EditFrame, w*0.16, h*0.25);
	TGLabel *rmaxlabel = new TGLabel(RhoMaxFrame, "RhoMax (cm)");
	fRMaxField = new TGNumberEntryField(RhoMaxFrame, 2, 0, TGNumberEntry::kNESRealFour, TGNumberEntry::kNEANonNegative);
	fRMaxField->Connect("TextChanged(const char*)","SPSPlotMainFrame",this,"HandleParameterChange()");
	RhoMaxFrame->AddFrame(rmaxlabel, lhints);
	RhoMaxFrame->AddFrame(fRMaxField, fhints);

	TGVerticalFrame *BFrame = new TGVerticalFrame(EditFrame, w*0.16, h*0.25);
	TGLabel *blabel = new TGLabel(BFrame, "B-field (kG)");
	fBField = new TGNumberEntryField(BFrame, 3, 0, TGNumberEntry::kNESRealFour, TGNumberEntry::kNEANonNegative);
	fBField->Connect("TextChanged(const char*)","SPSPlotMainFrame",this,"HandleParameterChange()");
	BFrame->AddFrame(blabel, lhints);
	BFrame->AddFrame(fBField, fhints);

	TGVerticalFrame *BKEFrame = new TGVerticalFrame(EditFrame, w*0.16, h*0.25);
	TGLabel *bkelabel = new TGLabel(BKEFrame, "Beam KE (MeV)");
	fBKEField = new TGNumberEntryField(BKEFrame, 4, 0, TGNumberEntry::kNESRealFour, TGNumberEntry::kNEANonNegative);
	fBKEField->Connect("TextChanged(const char*)","SPSPlotMainFrame",this,"HandleParameterChange()");
	BKEFrame->AddFrame(bkelabel, lhints);
	BKEFrame->AddFrame(fBKEField, fhints);

	TGVerticalFrame *ThetaFrame = new TGVerticalFrame(EditFrame, w*0.16, h*0.25);
	TGLabel *tlabel = new TGLabel(ThetaFrame, "Theta (deg)");
	fThetaField = new TGNumberEntryField(ThetaFrame, 5, 0, TGNumberEntry::kNESRealFour, TGNumberEntry::kNEANonNegative);
	fThetaField->Connect("TextChanged(const char*)","SPSPlotMainFrame",this,"HandleParameterChange()");
	ThetaFrame->AddFrame(tlabel, lhints);
	ThetaFrame->AddFrame(fThetaField, fhints);

	/*Plotting is explicity controlled by a button*/
	fPlotButton = new TGTextButton(EditFrame, "Plot!");
	fPlotButton->SetState(kButtonDisabled);
	fPlotButton->Connect("Clicked()","SPSPlotMainFrame",this,"DoPlot()");

	EditFrame->AddFrame(RhoMinFrame, fhints);
	EditFrame->AddFrame(RhoMaxFrame, fhints);
	EditFrame->AddFrame(BKEFrame, fhints);
	EditFrame->AddFrame(ThetaFrame, fhints);
	EditFrame->AddFrame(BFrame, fhints);
	EditFrame->AddFrame(fPlotButton, fhints);

	/*Menus*/
	TGMenuBar *fMenuBar = new TGMenuBar(this, w, h*0.1, kHorizontalFrame);
	fFileMenu = new TGPopupMenu(gClient->GetRoot());
	fFileMenu->AddEntry("Load Config", M_LOAD_CONFIG);
	fFileMenu->AddEntry("Save Config", M_SAVE_CONFIG);
	fFileMenu->Connect("Activated(Int_t)","SPSPlotMainFrame",this,"HandleMenuSelection(Int_t)");
	fMenuBar->AddPopup("File", fFileMenu, mhints);
	fRxnMenu = new TGPopupMenu(gClient->GetRoot());
	fRxnMenu->AddEntry("Add Reaction", M_ADD_REACTION);
	fRxnMenu->Connect("Activated(Int_t)","SPSPlotMainFrame",this,"HandleMenuSelection(Int_t)");
	fMenuBar->AddPopup("Reaction", fRxnMenu, mhints);

	AddFrame(fMenuBar);
	AddFrame(CanvasFrame, chints);
	AddFrame(EditFrame, ehints);

	SetWindowName("SPSPlot");
	MapSubwindows();
	Resize();
	MapWindow();

}

SPSPlotMainFrame::~SPSPlotMainFrame() {
	Cleanup(); //delete children
	delete this; //get rid of window
}

/*Overload for TGMainFrame's virtual CloseWindow(); kills program on closure of the mainframe*/
void SPSPlotMainFrame::CloseWindow() {
	gApplication->Terminate();
}

/*If there is a parameter change, then the plot button will actually do something*/
void SPSPlotMainFrame::HandleParameterChange() {
	paramFlag = true;
}

/*Currently only handles the creation of a FileViewFrame and properly links it to saving or loading;
  If we add reaction options, will need adjustment
*/
void SPSPlotMainFrame::HandleMenuSelection(int id) {

	switch(id) {
		case M_SAVE_CONFIG:
			new FileViewFrame(gClient->GetRoot(), this, MAIN_W*0.5, MAIN_H*0.5, this, id);
			break;
		case M_LOAD_CONFIG:
			new FileViewFrame(gClient->GetRoot(), this, MAIN_W*0.5, MAIN_H*0.5, this, id);
			break;
		case M_ADD_REACTION:
			new ReactionCreationFrame(gClient->GetRoot(), this, MAIN_W*0.5, MAIN_H*0.75, this);
			break;
	}

}

/*Handles button click*/
void SPSPlotMainFrame::DoPlot() {

	fPlotButton->SetState(kButtonDisabled);

	if(!attachFlag) { //If no file has been attached, we cant even begin
		std::cerr<<"Unable to plot without an input file!"<<std::endl;
		return;
	} else if(!paramFlag) { //If no paramters have been changed since last plot, dont modify
		fPlotButton->SetState(kButtonUp);
		return;
	}

	//Update reactions and plot
	UpdateKineSettings(fRMinField->GetNumber(), fRMaxField->GetNumber(), fBKEField->GetNumber(), fThetaField->GetNumber(), fBField->GetNumber());
	PlotGraphs();
	fPlotButton->SetState(kButtonUp);
	paramFlag = false; //now params are same as plot params
}

/*Actual plotting function; slightly complicated to handle axis generation*/
void SPSPlotMainFrame::PlotGraphs() {
	fCanvas->cd();
	auto graphs = fPlotter.GetGraphs();
	int ngraphs = fPlotter.GetNGraphs();
	if(graphs == nullptr){
		std::cerr<<"Faliure to generate graphs! Make sure input file is formated correctly!"<<std::endl;
		return;
	}

	int firstValidIndex = 0; //keeps track of who should be making the axes
	int nDrawn = 0; //see if anyone actually gets drawn
	for(int i=0; i<ngraphs; i++) {
		/*
			ROOT throws an execption when a graph with no points is drawn. To handle this,
			check for number of points. If no points no draw, however this can defer the first graph to be plotted,
			and the first graph has to have the "A" option to generate axes properly. Thus the necessity for firstValidIndex
		*/
		if(graphs[i]->GetN() == 0) {
			if(i == firstValidIndex) 
				firstValidIndex++;
			continue;
		} else if(i==firstValidIndex) {
			graphs[i]->Draw("AP*");
			nDrawn++;
		} else {
			graphs[i]->Draw("P*");
			nDrawn++;
		}
	}
	if(nDrawn > 0) fCanvas->BuildLegend(); //If someone is drawn build a legend; if no one clear the canvas to let the user know
	else fCanvas->Clear();
	fCanvas->Modified();
	fCanvas->Update();
}

/*Feed params to the SPSPlot instance*/
void SPSPlotMainFrame::UpdateKineSettings(double rmin, double rmax, double bke, double theta, double b) {
	fPlotter.SetRhoRange(rmin, rmax);
	fPlotter.SetParameters(bke, theta, b);
}

/*Load file and set to defaults*/
void SPSPlotMainFrame::LoadConfig(const char* name) {
	std::string sname = name;
	fPlotter.AttachFile(sname);
	attachFlag = true;
	paramFlag = false;
	PlotGraphs();
	fPlotButton->SetState(kButtonUp);
	fRMinField->SetNumber(fPlotter.GetRhoMin());
	fRMaxField->SetNumber(fPlotter.GetRhoMax());
	fBKEField->SetNumber(fPlotter.GetBeamKE());
	fThetaField->SetNumber(fPlotter.GetTheta());
	fBField->SetNumber(fPlotter.GetB());
}

/*Writting out*/
void SPSPlotMainFrame::WriteConfig(const char* name) {
	std::string sname = name;
	fPlotter.SaveToFile(sname);
}

void SPSPlotMainFrame::AddReaction(Reaction* rxn) {
	fPlotter.AddReaction(*rxn);
	PlotGraphs();
}