#ifndef DETECTOR_HH
#define DETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "g4root.hh"
#include "MuStoppedRunAction.hh"
#include "MuStoppedTrackingAction.hh"

class MySensitiveDetector : public G4VSensitiveDetector
{
public:
    //MySensitiveDetector(G4String,MuStoppedRunAction * run);
    MySensitiveDetector(G4String,MuStoppedRunAction * run,MuStoppedTrackingAction * track);
    ~MySensitiveDetector();

private:
    virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
    MuStoppedRunAction * fRunAction;
    MuStoppedTrackingAction * fTrackingAction;
};



#endif
