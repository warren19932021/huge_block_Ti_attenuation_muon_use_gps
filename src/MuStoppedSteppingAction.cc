//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file MuStoppedSteppingAction.cc
/// \brief Implementation of the MuStoppedSteppingAction class

#include "MuStoppedSteppingAction.hh"
#include "MuStoppedEventAction.hh"
#include "MuStoppedDetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuStoppedSteppingAction::MuStoppedSteppingAction(MuStoppedEventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fTheLogicalVolumeWithEnergyRecord(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuStoppedSteppingAction::~MuStoppedSteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuStoppedSteppingAction::UserSteppingAction(const G4Step* step)
{

  //G4cout<<"Here is MuStoppedSteppingAction::UserSteppingAction!!!!!!!!!!!! "<<G4endl;
  if (!fTheLogicalVolumeWithEnergyRecord) { 
      
    if(!G4RunManager::GetRunManager()->GetUserDetectorConstruction()) return;  

    const MuStoppedDetectorConstruction* detectorConstruction
      = static_cast<const MuStoppedDetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fTheLogicalVolumeWithEnergyRecord = detectorConstruction->GetTheLogicalVolumeWithEnergyRecord();   
  }

  // get volume of the current step
  if(!step->GetPreStepPoint()) return;
  if(!step->GetPreStepPoint()->GetTouchableHandle()) return;
  if(!step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()) return;
  if(!step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()) return;
  G4LogicalVolume* volume 
    = step->GetPreStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();

   //G4cout<<"volume name: "<<volume->GetName()<<G4endl;   
  // check if we are in the Logical Volume With Energy Record
  if (volume != fTheLogicalVolumeWithEnergyRecord) return;

  // collect energy deposited in this step
  G4double edepStep = step->GetTotalEnergyDeposit();
  fEventAction->AddEdep(edepStep);  


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

