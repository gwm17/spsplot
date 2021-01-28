/*
    
    ReactionCreationFrame.h
    Wrapper class on a TGTransientFrame (temporary frame assoc. with a main frame)
    User specifies reactants, a Reaction is created and sent as a signal back to the main
    frame.

    Written by G.W. McCann Sep. 2020

*/

#ifndef REACTIONCREATIONFRAME_H
#define REACTIONCREATIONFRAME_H

#include "SPSPlotMainFrame.h"
#include "Reaction.h"
#include <TGNumberEntry.h>
#include <TQObject.h>
#include <RQ_OBJECT.h>

class ReactionCreationFrame {

  RQ_OBJECT("ReactionCreationFrame"); //ROOT wrapping into gui environment
  public:
    ReactionCreationFrame(const TGWindow *p, const TGWindow *main, UInt_t w, UInt_t h, SPSPlotMainFrame *parent);
    virtual ~ReactionCreationFrame();
    void CloseWindow();
    void DoOk();
    void DoCancel();
    void SendReaction(Reaction* reaction); // *SIGNAL*
    ClassDef(ReactionCreationFrame, 0); //ROOT requirements

  private:
    Reaction rxn;
    TGTransientFrame *fMain;
    TGTextButton *fOkButton, *fCancelButton;
    TGNumberEntryField *fATField, *fZTField, *fAEField, *fZEField, *fAPField,
                       *fZPField;
};

#endif
