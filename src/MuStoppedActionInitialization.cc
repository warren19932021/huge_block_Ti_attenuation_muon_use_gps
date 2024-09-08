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
/// \file MuStoppedActionInitialization.cc
/// \brief Implementation of the MuStoppedActionInitialization class

#include "MuStoppedActionInitialization.hh"
#include "MuStoppedPrimaryGeneratorAction.hh"
//#include "generator.hh"
#include "MuStoppedRunAction.hh"
#include "MuStoppedEventAction.hh"
#include "MuStoppedTrackingAction.hh"
#include "MuStoppedSteppingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuStoppedActionInitialization::MuStoppedActionInitialization(MuStoppedDetectorConstruction* pDetectorConstruction)
 : G4VUserActionInitialization()
{ 
  detCon = pDetectorConstruction;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuStoppedActionInitialization::~MuStoppedActionInitialization()
{}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuStoppedActionInitialization::Build() const
{
  MuStoppedPrimaryGeneratorAction* MyPrimaryGeneratorAction = new MuStoppedPrimaryGeneratorAction;
  SetUserAction(MyPrimaryGeneratorAction);

  //MyPrimaryGenerator * genetator = new MyPrimaryGenerator();
  //MyPrimaryGenerator * genetator = new MyPrimaryGenerator;
  //SetUserAction(genetator);

  MuStoppedRunAction* runAction = new MuStoppedRunAction;
  SetUserAction(runAction);
  detCon->SetRunAction(runAction);
  
  MuStoppedEventAction* eventAction = new MuStoppedEventAction(runAction,MyPrimaryGeneratorAction);
  //MuStoppedEventAction* eventAction = new MuStoppedEventAction(runAction,genetator);
  SetUserAction(eventAction);

  MuStoppedTrackingAction * trackAction = new MuStoppedTrackingAction();
  SetUserAction(trackAction);
  detCon->SetTrackingAction(trackAction);
  
  SetUserAction(new MuStoppedSteppingAction(eventAction));
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
