#ifndef AndiDETECTOR_HH
#define ANdiDETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "g4root.hh"
#include "MuStoppedRunAction.hh"
#include "MuStoppedTrackingAction.hh"

class AndiHPGeSensitiveDetector : public G4VSensitiveDetector
{
public:
    //AndiHPGeSensitiveDetector(G4String,MuStoppedRunAction * run);
    AndiHPGeSensitiveDetector(G4String,MuStoppedRunAction * run,MuStoppedTrackingAction * track);
    ~AndiHPGeSensitiveDetector();

private:
    virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
    MuStoppedRunAction * fRunAction;
    MuStoppedTrackingAction * fTrackingAction;
};



#endif
