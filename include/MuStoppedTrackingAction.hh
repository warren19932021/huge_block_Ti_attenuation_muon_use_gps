#ifndef MuStoppedTrackingAction_h
#define MuStoppedTrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"
//------add g4root.hh, then G4AnalysisManager can be recognised!----
#include "g4root.hh"

#include "G4GenericMessenger.hh"

#include "G4RunManager.hh"

class MuStoppedTrackingAction : public G4UserTrackingAction {

  public:  

   MuStoppedTrackingAction();
   ~MuStoppedTrackingAction() {};
   
    virtual void PreUserTrackingAction(const G4Track*);   
    virtual void PostUserTrackingAction(const G4Track*);
    G4int GetFlagForRealSignal() const { return FlagForRealSignal; }

  private:


//------- If FlagForRealSignal = 1, it means Muon is captured and also produced real signal gamma,------  
//------- Then in this Event, all the following track will marked with FlagForRealSignal = 1----
//------- sometimes even cptured, but the daughter is not the one we want.---------------------- 

  G4int FlagForRealSignal;

  G4GenericMessenger *fMesesengerInTrackingAction;
  G4String SecondaryNameContains;

};

#endif
