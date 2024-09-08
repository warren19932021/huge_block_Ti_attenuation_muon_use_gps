#include "MySensitiveDetector.hh"
#include "G4Gamma.hh"
#include "G4UnitsTable.hh"

MySensitiveDetector::MySensitiveDetector(G4String name,MuStoppedRunAction * run,MuStoppedTrackingAction * track) : G4VSensitiveDetector(name),
fRunAction(run),
fTrackingAction(track)
{
   // G4cout<<"Here is MySensitiveDetector::MySensitiveDetector constructor function !!!!!!!!!!!!!!!! "<<G4endl;
}

MySensitiveDetector::~MySensitiveDetector()
{

}

G4bool MySensitiveDetector::ProcessHits(G4Step * aStep, G4TouchableHistory * ROhist)//
{
    //G4cout<<"Here is MySensitiveDetector::ProcessHits !!!!!!!!!!!!!!!! "<<G4endl;
    // Get the G4RunManager instance
    G4RunManager* runManager = G4RunManager::GetRunManager();
    // Get the current event being processed
    const G4Event* event = runManager->GetCurrentEvent();

    G4Track * track =  aStep->GetTrack();

    G4StepPoint * postStepPoint = aStep->GetPostStepPoint();
    G4ThreeVector CapturedPosition = postStepPoint->GetPosition();
    if(!postStepPoint->GetProcessDefinedStep()) return false;
    const G4VProcess* Process   =  postStepPoint->GetProcessDefinedStep();    
    if(!Process->GetProcessName()) return false;
    std::string postStepProcessName = Process->GetProcessName();
    G4int FlagForRealSignal = fTrackingAction->GetFlagForRealSignal();

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    if(event&&track->GetTrackID()==1&&postStepProcessName.compare("muMinusCaptureAtRest")==0)
      {
         if (!fRunAction) G4cout << "MySensitiveDetector has no run action pointer!" << G4endl;
         else  fRunAction->PlusOneMuonMinusCaptureInRunAction();
         analysisManager->FillNtupleDColumn(2, 0,CapturedPosition.getX());   
         analysisManager->FillNtupleDColumn(2, 1,CapturedPosition.getY());   
         analysisManager->FillNtupleDColumn(2, 2,CapturedPosition.getZ());  
         analysisManager->FillNtupleIColumn(2, 3,event->GetEventID());        
         analysisManager->AddNtupleRow(2);
      } 
      else
      {
        if(event&&FlagForRealSignal==1) 
         {
           if(!track->GetCreatorProcess()) return false;
           if(!track->GetCreatorProcess()->GetProcessName()) return false;
           if (track->GetCreatorProcess()->GetProcessName().compare("muMinusCaptureAtRest")==0)
            { 
              if(!postStepPoint -> GetStepStatus()) return false;
              if (track->GetDefinition()==G4Gamma::GammaDefinition()&&postStepPoint -> GetStepStatus() == fGeomBoundary )
              {  
                 G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
                 analysisManager->FillNtupleDColumn(5, 0,postStepPoint->GetTotalEnergy());    
                 analysisManager->FillNtupleDColumn(5, 1,postStepPoint->GetGlobalTime());
                 analysisManager->FillNtupleDColumn(5, 2,postStepPoint->GetPosition().getX());      
                 analysisManager->FillNtupleDColumn(5, 3,postStepPoint->GetPosition().getY());      
                 analysisManager->FillNtupleDColumn(5, 4,postStepPoint->GetPosition().getZ());      
                 analysisManager->FillNtupleSColumn(5, 5,track->GetParticleDefinition()->GetParticleName());        
                 analysisManager->FillNtupleIColumn(5, 6,event->GetEventID());        
                 analysisManager->AddNtupleRow(5);
              }
            }
         }
      }

  
  return true;

}

