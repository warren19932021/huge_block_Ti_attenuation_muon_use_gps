#include "MuStoppedTrackingAction.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4UnitsTable.hh"

MuStoppedTrackingAction::MuStoppedTrackingAction()
:G4UserTrackingAction(),
FlagForRealSignal(0),
SecondaryNameContains("Sc48")
{

   fMesesengerInTrackingAction = new G4GenericMessenger(this,"/track/","track information");   
   
   fMesesengerInTrackingAction->DeclareProperty("secondaryNameContains",SecondaryNameContains,"Secondary Name Contains");

}

void MuStoppedTrackingAction::PreUserTrackingAction(const G4Track* aTrack )
{}


void MuStoppedTrackingAction::PostUserTrackingAction(const G4Track* track)
{

    //G4cout<<"Here is MuStoppedTrackingAction::PostUserTrackingAction 1111111111 !!!!!!!!!!!!!!!!!!!!!!!!!!"<<G4endl;

    // Get the G4RunManager instance
    G4RunManager* runManager = G4RunManager::GetRunManager();
    // Get the current event being processed
    const G4Event* event = runManager->GetCurrentEvent();

    const G4Step* step = track->GetStep();
    if(!step) return;
    const G4StepPoint* postStepPoint = step->GetPostStepPoint();
    if(!postStepPoint) return;
    const G4VProcess* process = postStepPoint->GetProcessDefinedStep();
    if(!process) return;

    //G4cout<<"track->GetTrackID(): "<<track->GetTrackID()<<G4endl;
    //G4cout<<"Here is MuStoppedTrackingAction::PostUserTrackingAction 22222222222 !!!!!!!!!!!!!!!!!!!!!!!!!!"<<G4endl;
  
    if (track->GetTrackID() == 1)
    {   
        if(!process->GetProcessName()) return;

        if((process->GetProcessName()).compare("muMinusCaptureAtRest")==0) 
        {     
            //  G4cout<<"postStepPoint LogicalVolume Name: "<<postStepPoint->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName()<<G4endl;
             
              // Get the number of secondaries
              G4int nSecondaries = step->GetNumberOfSecondariesInCurrentStep();
              auto secondary  = step->GetSecondaryInCurrentStep();
            /*
              if(nSecondaries&&event)
              {
                 for (size_t i=0; i<nSecondaries;i++)
                 {
                     auto secstep = (*secondary)[i];
                     if(!secstep->GetDefinition()) return;
                     G4String secondaryName = secstep->GetDefinition()->GetParticleName();
                     G4cout<<"All secondaryName: "<<secondaryName<<G4endl;
                 }
              }
            */
              if(nSecondaries&&event)
              {
                 for (size_t i=0; i<nSecondaries;i++)
                 {
                     auto secstep = (*secondary)[i];
                     if(!secstep->GetDefinition()) return;
                     G4String secondaryName = secstep->GetDefinition()->GetParticleName();  
                     //if(secondaryName.contains("Sb124"))
                     if(secondaryName.contains(SecondaryNameContains))
                     {            
                       //G4cout<<"Contains Sb124 !!!!!!!!!!!!"<<G4endl;
                       FlagForRealSignal = 1;
                       break;
                     }
                 }
              }
      


              if(nSecondaries&& FlagForRealSignal == 1 && event)
              {
                 for (size_t i=0; i<nSecondaries;i++)
                 {
                 
                       auto secstep = (*secondary)[i];
                       G4String secondaryName = secstep->GetDefinition()->GetParticleName();
                       if (secondaryName.compare("gamma")==0)
                       {
                         G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
                         analysisManager->FillNtupleDColumn(3, 0,secstep->GetTotalEnergy());    
                         analysisManager->FillNtupleDColumn(3, 1,secstep->GetGlobalTime());
                         analysisManager->FillNtupleDColumn(3, 2,secstep->GetPosition().getX());      
                         analysisManager->FillNtupleDColumn(3, 3,secstep->GetPosition().getY());      
                         analysisManager->FillNtupleDColumn(3, 4,secstep->GetPosition().getZ());      
                         analysisManager->FillNtupleSColumn(3, 5,secondaryName);        
                         analysisManager->FillNtupleIColumn(3, 6,event->GetEventID());        
                         analysisManager->AddNtupleRow(3);
                      }

                      if (secondaryName.compare(SecondaryNameContains)==0)
                      //if (secondaryName.compare("Sb124")==0)
                      {           
                        G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
                        analysisManager->FillNtupleDColumn(4, 0,secstep->GetPosition().getX());      
                        analysisManager->FillNtupleDColumn(4, 1,secstep->GetPosition().getY());      
                        analysisManager->FillNtupleDColumn(4, 2,secstep->GetPosition().getZ());  
                        analysisManager->FillNtupleIColumn(4, 3,event->GetEventID());        
                        analysisManager->AddNtupleRow(4);
                      }


                 }
              }

        }
        else
        {
           FlagForRealSignal = 0;
        }

    }    

}
