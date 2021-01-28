/*
    
    ReactionCreationFrame.h
    Wrapper class on a TGTransientFrame (temporary frame assoc. with a main frame)
    User specifies reactants, a Reaction is created and sent as a signal back to the main
    frame.

    Written by G.W. McCann Sep. 2020

*/
#include "ReactionCreationFrame.h"
#include <TTimer.h>
#include <TGLabel.h>

ReactionCreationFrame::ReactionCreationFrame(const TGWindow *p, const TGWindow *main, UInt_t w, UInt_t h, SPSPlotMainFrame *parent) {
  fMain = new TGTransientFrame(p, main, w, h);
  fMain->SetCleanup(kDeepCleanup); //delete all child frames
  fMain->DontCallClose(); //disable close window button
  
  /*Layout orgainization hints*/
  TGLayoutHints *fhints = new TGLayoutHints(kLHintsCenterX|kLHintsCenterY,5,5,5,5);

  /*All of the reactant entry fields*/
  TGVerticalFrame *NucleiFrame = new TGVerticalFrame(fMain, w*0.5, h*0.55);
  TGLabel *ATLabel = new TGLabel(NucleiFrame, "Target A");
  fATField = new TGNumberEntryField(NucleiFrame, 1, 1, TGNumberEntry::kNESInteger, TGNumberEntry::kNEAPositive);
  TGLabel *ZTLabel = new TGLabel(NucleiFrame, "Target Z");
  fZTField = new TGNumberEntryField(NucleiFrame, 2, 1, TGNumberEntry::kNESInteger, TGNumberEntry::kNEAPositive);
  TGLabel *APLabel = new TGLabel(NucleiFrame, "Projectile A");
  fAPField = new TGNumberEntryField(NucleiFrame, 3, 1, TGNumberEntry::kNESInteger, TGNumberEntry::kNEAPositive);
  TGLabel *ZPLabel = new TGLabel(NucleiFrame, "Projectile Z");
  fZPField = new TGNumberEntryField(NucleiFrame, 4, 1, TGNumberEntry::kNESInteger, TGNumberEntry::kNEAPositive);
  TGLabel *AELabel = new TGLabel(NucleiFrame, "Ejectile A");
  fAEField = new TGNumberEntryField(NucleiFrame, 5, 1, TGNumberEntry::kNESInteger, TGNumberEntry::kNEAPositive);
  TGLabel *ZELabel = new TGLabel(NucleiFrame, "Ejectile Z");
  fZEField = new TGNumberEntryField(NucleiFrame, 6, 1, TGNumberEntry::kNESInteger, TGNumberEntry::kNEAPositive);
  NucleiFrame->AddFrame(ATLabel, fhints);
  NucleiFrame->AddFrame(fATField, fhints);
  NucleiFrame->AddFrame(ZTLabel, fhints);
  NucleiFrame->AddFrame(fZTField, fhints);
  NucleiFrame->AddFrame(APLabel, fhints);
  NucleiFrame->AddFrame(fAPField, fhints);
  NucleiFrame->AddFrame(ZPLabel, fhints);
  NucleiFrame->AddFrame(fZPField, fhints);
  NucleiFrame->AddFrame(AELabel, fhints);
  NucleiFrame->AddFrame(fAEField, fhints);
  NucleiFrame->AddFrame(ZELabel, fhints);
  NucleiFrame->AddFrame(fZEField, fhints);

  /*Ok and Cancel buttons*/
  TGHorizontalFrame *ButtonFrame = new TGHorizontalFrame(fMain, w, h*0.125);
  fOkButton = new TGTextButton(ButtonFrame, "Ok");
  fOkButton->Connect("Clicked()","ReactionCreationFrame",this,"DoOk()");
  fCancelButton = new TGTextButton(ButtonFrame, "Cancel");
  fCancelButton->Connect("Clicked()","ReactionCreationFrame",this,"DoCancel()");
  ButtonFrame->AddFrame(fOkButton, fhints);
  ButtonFrame->AddFrame(fCancelButton, fhints);
  
  fMain->AddFrame(NucleiFrame, fhints);
  fMain->AddFrame(ButtonFrame, fhints);

  /*Signal connection*/
  Connect("SendReaction(Reaction*)","SPSPlotMainFrame",parent,"AddReaction(Reaction*)");

  fMain->SetWindowName("Add Reaction");
  fMain->MapSubwindows();
  fMain->Resize();
  fMain->CenterOnParent();
  fMain->MapWindow();
}

ReactionCreationFrame::~ReactionCreationFrame() {
  fMain->Cleanup(); //delete children
  fMain->DeleteWindow();
}

void ReactionCreationFrame::CloseWindow() {
  delete this;
}

void ReactionCreationFrame::DoOk() {
  fOkButton->SetState(kButtonDisabled); //stop user from doing something dumb
  fCancelButton->SetState(kButtonDisabled);
  
  int at, zt, ae, ze, ap, zp;

  at = fATField->GetIntNumber();
  zt = fZTField->GetIntNumber();
  ap = fAPField->GetIntNumber();
  zp = fZPField->GetIntNumber();
  ae = fAEField->GetIntNumber();
  ze = fZEField->GetIntNumber();

  if((at+ap - ae) < 0) { //nucleon conservation
    std::cerr<<"Number of nucleons needs to be conserved!"<<std::endl;
    fOkButton->SetState(kButtonUp);
    fCancelButton->SetState(kButtonUp);
    return;
  }

  rxn.SetReactionData(at,zt,ap,zp,ae,ze);

  SendReaction(&rxn);

  //Wait for a breif period and then close the window; ensure no memory is 
  //deleted too quickly
  TTimer::SingleShot(150, "ReactionCreationFrame",this,"CloseWindow()");
}

void ReactionCreationFrame::DoCancel() {
  fOkButton->SetState(kButtonDisabled); //stop user from doing something dumb
  fCancelButton->SetState(kButtonDisabled);
  //Wait for a breif period and then close the window; ensure no memory is 
  //deleted too quickly
  TTimer::SingleShot(150, "ReactionCreationFrame", this, "CloseWindow()");
}

/*Signal*/
void ReactionCreationFrame::SendReaction(Reaction *reaction) {
  Emit<Reaction*>("SendReaction(Reaction*)", reaction);
}
