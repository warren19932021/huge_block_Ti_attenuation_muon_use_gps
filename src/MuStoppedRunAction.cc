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
/// \file MuStoppedRunAction.cc
/// \brief Implementation of the MuStoppedRunAction class

#include "MuStoppedRunAction.hh"
#include "MuStoppedPrimaryGeneratorAction.hh"
#include "MuStoppedDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuStoppedRunAction::MuStoppedRunAction()
: G4UserRunAction(),
  fEdep(0.),
  fEdep2(0.),
  fNumberOfMuonMuMinusCaptures(0),
  OutFileName("out.root")
{ 



   fMessenger = new G4GenericMessenger(this,"/run_action/","Run Action Messenger");
   fMessenger->DeclareProperty("out_file_name",OutFileName,"Output File Name");


  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  analysisManager->SetFirstNtupleId(1);       
  analysisManager->CreateNtuple("MuonSourceEnergy", "Muon Source Energy");
  analysisManager->CreateNtupleDColumn("E");    //column 0
  analysisManager->FinishNtuple(1);

//---------- attention! even though muon is captured, it does not mean the real signal gamma will be produced!!!-----
//---------- for example, when mu is captured by 124Sn, it will not always produce 124Sb----- 
  analysisManager->CreateNtuple("MuonCapturedPosition", "Muon Captured Position in the target");
  analysisManager->CreateNtupleDColumn("X");    //column 0
  analysisManager->CreateNtupleDColumn("Y");    //column 1
  analysisManager->CreateNtupleDColumn("Z");    //column 2
  analysisManager->CreateNtupleIColumn("evtnumber");    //column 3
  analysisManager->FinishNtuple(2);

//------ Ntuple 3 saved the secondary gammas information(position produced, primary energy,etc.) after real signal muon capture 
  analysisManager->CreateNtuple("GammaInformationWhereProduced", "Real Signal Gamma Information after muon capture where produced");
  analysisManager->CreateNtupleDColumn("E");    //column 0
  analysisManager->CreateNtupleDColumn("T");    //column 1
  analysisManager->CreateNtupleDColumn("X");    //column 2
  analysisManager->CreateNtupleDColumn("Y");    //column 3
  analysisManager->CreateNtupleDColumn("Z");    //column 4
  analysisManager->CreateNtupleSColumn("ParticleName");    //column 5
  analysisManager->CreateNtupleIColumn("evtnumber");    //column 6
  analysisManager->FinishNtuple(3);

//----------Muon capture position of Real signal----- 
  analysisManager->CreateNtuple("MuonCapturedPositionOfRealSignal", "Muon Captured Position of Real Signal in the target");
  analysisManager->CreateNtupleDColumn("X");    //column 0
  analysisManager->CreateNtupleDColumn("Y");    //column 1
  analysisManager->CreateNtupleDColumn("Z");    //column 2
  analysisManager->CreateNtupleIColumn("evtnumber");    //column 3
  analysisManager->FinishNtuple(4);

//------ Ntuple 5 saved the secondary gammas information (position produced, energy left when reached the boundary,etc.)  
//------ escaped from the target surface after real signal muon capture 
  analysisManager->CreateNtuple("GammaInformationEscaped", "Real Signal Gamma Information after muon capture when escape from surface of target");
  analysisManager->CreateNtupleDColumn("E");    //column 0
  analysisManager->CreateNtupleDColumn("T");    //column 1
  analysisManager->CreateNtupleDColumn("X");    //column 2
  analysisManager->CreateNtupleDColumn("Y");    //column 3
  analysisManager->CreateNtupleDColumn("Z");    //column 4
  analysisManager->CreateNtupleSColumn("ParticleName");    //column 5
  analysisManager->CreateNtupleIColumn("evtnumber");    //column 6
  analysisManager->FinishNtuple(5);

/*
  // add new units for dose
  
  const G4double milligray = 1.e-3*gray;
  const G4double microgray = 1.e-6*gray;
  const G4double nanogray  = 1.e-9*gray;  
  const G4double picogray  = 1.e-12*gray;
   
  new G4UnitDefinition("milligray", "milliGy" , "Dose", milligray);
  new G4UnitDefinition("microgray", "microGy" , "Dose", microgray);
  new G4UnitDefinition("nanogray" , "nanoGy"  , "Dose", nanogray);
  new G4UnitDefinition("picogray" , "picoGy"  , "Dose", picogray); 

  // Register accumulable to the accumulable manager
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->RegisterAccumulable(fEdep);
  accumulableManager->RegisterAccumulable(fEdep2); 
*/

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuStoppedRunAction::~MuStoppedRunAction()
{
      delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuStoppedRunAction::BeginOfRunAction(const G4Run* run)
{ 

  fNumberOfMuonMuMinusCaptures = 0.;

/*

  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // reset accumulables to their initial values
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Reset();


*/

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4int RunID = run->GetRunID();
  std::stringstream strRunID;
  strRunID << RunID;
  analysisManager->OpenFile(OutFileName);
  //analysisManager->OpenFile("TEST"+strRunID.str()+".root");


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuStoppedRunAction::EndOfRunAction(const G4Run* run)
{

/*

  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  // Merge accumulables 
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Merge();

  // Compute dose = total energy deposit in a run and its variance
  //
  G4double edep  = fEdep.GetValue();
  G4double edep2 = fEdep2.GetValue();
  
  G4double rms = edep2 - edep*edep/nofEvents;
  if (rms > 0.) rms = std::sqrt(rms); else rms = 0.;  



  //G4cout<<"edep: "<<G4BestUnit(edep,"Energy")<<G4endl;
  
  if(!G4RunManager::GetRunManager()->GetUserDetectorConstruction()) return;
  const MuStoppedDetectorConstruction* detectorConstruction
   = static_cast<const MuStoppedDetectorConstruction*>
     (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  if(!detectorConstruction->GetTheLogicalVolumeWithEnergyRecord()) return;   
  if(!detectorConstruction->GetTheLogicalVolumeWithEnergyRecord()->GetMass()) return;   
  G4double mass = detectorConstruction->GetTheLogicalVolumeWithEnergyRecord()->GetMass();
  G4double dose = edep/mass;
  G4double rmsDose = rms/mass;


  // Run conditions
  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
  const MuStoppedPrimaryGeneratorAction* generatorAction
   = static_cast<const MuStoppedPrimaryGeneratorAction*>
     (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  G4String runCondition;
  if (generatorAction)
  {
    const G4GeneralParticleSource* particleGPS = generatorAction->GetG4GeneralParticleSource();
    particleGPS->GetParticleDefinition()->GetParticleName();
    runCondition += particleGPS->GetParticleDefinition()->GetParticleName();
    runCondition += " of ";
    G4double particleEnergy = particleGPS->GetParticleEnergy();
    runCondition += G4BestUnit(particleEnergy,"Energy");
  }


  G4cout
     << G4endl
     << " The run consists of " << nofEvents << " "<< runCondition
     << G4endl
     << " Cumulated dose per run, in the logical volume with energy eecord : "
     << G4BestUnit(dose,"Dose") << " rms = " << G4BestUnit(rmsDose,"Dose")
     << G4endl
     << "------------------------------------------------------------"
     << G4endl
     << G4endl;

*/

  // Print
  //  

  G4cout
     << G4endl
     << "-----------------------End of Run---------------------------";

  G4cout
     << G4endl
     << " fNumberOfMuonMuMinusCaptures: "<<fNumberOfMuonMuMinusCaptures<<" in RunID: " <<run->GetRunID()
     << G4endl
     << "------------------------------------------------------------"
     << G4endl
     << G4endl;

  std::ofstream myfile("NumberOfMuonMuMinusCaptures.txt", std::ios_base::app | std::ios_base::out);
  myfile << "fNumberOfMuonMuMinusCaptures: "<<fNumberOfMuonMuMinusCaptures<<" in RunID: " <<run->GetRunID()<<G4endl;
  myfile.close();
  

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuStoppedRunAction::AddEdep(G4double edep)
{
  fEdep  += edep;
  fEdep2 += edep*edep;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuStoppedRunAction::PlusOneMuonMinusCaptureInRunAction()
{
    fNumberOfMuonMuMinusCaptures++;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
