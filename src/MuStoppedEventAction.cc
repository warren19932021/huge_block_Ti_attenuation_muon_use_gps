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
/// \file MuStoppedEventAction.cc
/// \brief Implementation of the MuStoppedEventAction class

#include "MuStoppedEventAction.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
//#include "G4AnalysisManager.hh" 


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuStoppedEventAction::MuStoppedEventAction(MuStoppedRunAction* runAction, MuStoppedPrimaryGeneratorAction* PrimaryGeneratorAction)
//MuStoppedEventAction::MuStoppedEventAction(MuStoppedRunAction* runAction, MyPrimaryGenerator* GunGenerator)
: G4UserEventAction(),
  fRunAction(runAction),
  fPrimaryGeneratorAction(PrimaryGeneratorAction),
  //fPrimaryGeneratorAction(GunGenerator),
  fEdep(0.)
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuStoppedEventAction::~MuStoppedEventAction()
{}

//....oooOO0OOooo...<.....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuStoppedEventAction::BeginOfEventAction(const G4Event* evt)
{    
  fEdep = 0.;

  //G4double Egps = fPrimaryGeneratorAction->GetSourceParticleEnergy();
  G4double Ekin = evt->GetPrimaryVertex()->GetPrimary()->GetKineticEnergy();
  
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  if(!analysisManager) G4cout<<"nullptr!!!!!!!!!!!!!!!"<<G4endl;
  else 
  {
    analysisManager->FillNtupleDColumn(1, 0,Ekin);   
    analysisManager->AddNtupleRow(1);
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuStoppedEventAction::EndOfEventAction(const G4Event* evt)
{   
  // accumulate statistics in run action
  fRunAction->AddEdep(fEdep);
  
  //G4cout<<"fEdep: "<<G4BestUnit(fEdep,"Energy")<<G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


