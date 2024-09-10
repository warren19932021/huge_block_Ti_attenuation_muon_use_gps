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
/// \file MuStoppedDetectorConstruction.cc
/// \brief Implementation of the MuStoppedDetectorConstruction class

#include "MuStoppedDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "MuStoppedRunAction.hh"
#include "G4VisAttributes.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuStoppedDetectorConstruction::MuStoppedDetectorConstruction()
: G4VUserDetectorConstruction(),
  fTheLogicalVolumeWithEnergyRecord(0),
  BlockLength(15),
  BlockWidth(15),
  BlockHeight(15),
  ConcreteLength(40000),
  ConcretekWidth(40000),
  ConcreteHeight(40000),
  TargetMaterial("G4_Ti")
{
   DefineMaterials();

   fMesesenger = new G4GenericMessenger(this,"/detector/","Detector Construction");   
   
   fMesesenger->DeclareProperty("BlockLength",BlockLength,"Side Thcikness of CupTarget");
   fMesesenger->DeclareProperty("BlockWidth",BlockWidth,"Front Thcikness of CupTarget");
   fMesesenger->DeclareProperty("BlockHeight",BlockHeight,"Height of CupTarget");

   fMesesenger->DeclareProperty("ConcreteLength",ConcreteLength,"Concrete Length");
   fMesesenger->DeclareProperty("ConcreteWidth",ConcretekWidth,"Concretek Width");
   fMesesenger->DeclareProperty("ConcreteHeight",ConcreteHeight,"Concrete Height");


   fMesesenger->DeclareProperty("TargetMaterial",TargetMaterial,"Target Material");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MuStoppedDetectorConstruction::~MuStoppedDetectorConstruction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuStoppedDetectorConstruction::SetRunAction(MuStoppedRunAction* pRunAction)
{
    if (!pRunAction) G4cout << "pRunAction is nullptr" << G4endl;
    runAction = pRunAction;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuStoppedDetectorConstruction::SetTrackingAction(MuStoppedTrackingAction* pTrackingAction)
{
    if (!pTrackingAction) G4cout << "pTrackingAction is nullptr" << G4endl;
    trackingAction = pTrackingAction;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuStoppedDetectorConstruction::DefineMaterials()
{

    G4NistManager * nist = G4NistManager::Instance();
    worldMat = nist->FindOrBuildMaterial("G4_AIR");
    //ConcreteMat = nist->FindOrBuildMaterial("G4_CONCRETE");
    //ConcreteMat = nist->FindOrBuildMaterial("G4_Ti");
/*    
    G4Isotope* isotopeTe124 = new G4Isotope("Te124",52,124,123.904*g / mole);   
    G4Element* elementTe124 = new G4Element("Tellurium124","Te124",1);
    elementTe124->AddIsotope(isotopeTe124,1);   
    Target_material = new G4Material("Target_material",6.24*g/cm3,1);
    Target_material->AddElement(elementTe124,1.0);
*/



    // Define Titanium-48 isotope
    G4int Z_Ti = 22;  // Atomic number of Titanium
    G4int A_Ti48 = 48; // Atomic mass of Titanium-48
    G4double atomicMass_Ti48 = 47.9479 * g/mole;  // Atomic mass of Titanium-48
    G4Isotope* Ti48_isotope = new G4Isotope("Ti48", Z_Ti, A_Ti48, atomicMass_Ti48);

    // Define element Titanium consisting only of Ti-48 isotope
    G4Element* pureTi48 = new G4Element("PureTitanium48", "Ti48", 1); // 1 isotope
    pureTi48->AddIsotope(Ti48_isotope, 100*perCent);  // 100% Ti-48

    // Create the material out of the element Ti-48
    G4double density_Ti = 4.54 * g/cm3;  // Approximate density of Titanium
    ConcreteMat = new G4Material("Pure48Ti", density_Ti, 1);  // Single element material
    ConcreteMat->AddElement(pureTi48, 1.0);  // 100% Ti-48



    //Target_material = nist->FindOrBuildMaterial(TargetMaterial);
    
    G4cout<<"ConcreteMat: ============================    "<<ConcreteMat->GetName()<<G4endl;

}    


G4VPhysicalVolume* MuStoppedDetectorConstruction::Construct()
{  
    G4cout<<"ConcreteMat in MuStoppedDetectorConstruction::Construct: ============================    "<<ConcreteMat->GetName()<<G4endl;
    
    //---- World------
    solidWorld =  new G4Box("SolidWorld",50.0*m,50.0*m,50.0*m); 
    logicalWorld = new G4LogicalVolume(solidWorld, worldMat,"logicWorld");
    physWorld = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),logicalWorld,"physWolrd",0,false,0,true); 

    G4VisAttributes* worldVisAttributes = new G4VisAttributes(false); // tranparent
    logicalWorld->SetVisAttributes(worldVisAttributes);

    //---- Concrete------
    //solidConcrete   =  new G4Box("solidConcrete",2.0*m,2.0*m,10.0*cm); 
    solidConcrete   =  new G4Box("solidConcrete",ConcreteLength/2,ConcretekWidth/2,ConcreteHeight/2); 
    logicalConcrete = new G4LogicalVolume(solidConcrete, ConcreteMat, "logical_concrete");
    physConcrete = new G4PVPlacement(0, G4ThreeVector(0.,0.,ConcreteHeight/2), logicalConcrete, "physConcrete", logicalWorld, false, 0, true);
    //physConcrete = new G4PVPlacement(0, G4ThreeVector(0.,0.,50.0*cm), logicalConcrete, "physConcrete", logicalWorld, false, 0, true);

    G4VisAttributes* concreteVisAttributes = new G4VisAttributes(G4Color(0.6, 0.6, 0.6, 0.8)); // gray, opacity 0.8
    logicalConcrete->SetVisAttributes(concreteVisAttributes);

//   //---- solidTarget------
//    solidTarget =  new G4Box("SolidTarget",BlockLength/2,BlockWidth/2,BlockHeight/2); 
//    logical_block_target = new G4LogicalVolume(solidTarget,Target_material,"logical_block_target");
//    phys_block_Target = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),logical_block_target,"physTarget",logicalWorld,false,0,true); 

    G4cout<<"oooooo0000000000------DEBUG::Detector Setup Output------oooooo0000000000"<<G4endl;
    G4cout<<"BlockLength: "<<BlockLength<<G4endl;
    G4cout<<"BlockWidth: "<<BlockWidth<<G4endl;
    G4cout<<"BlockHeight: "<<BlockHeight<<G4endl;
    G4cout<<"TargetMaterial: "<<TargetMaterial<<G4endl;
    G4cout<<"oooooo0000000000---------------------------------------ooooooo0000000000"<<G4endl;

    // Set Block shape Target as TheLogicalVolumeWithEnergyRecord volume
    
    //fTheLogicalVolumeWithEnergyRecord = logical_block_target;
   
   
    fTheLogicalVolumeWithEnergyRecord = logicalConcrete;

    //always return the physical World
    return physWorld;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MuStoppedDetectorConstruction::ConstructSDandField()
{
    
    senDet = new MySensitiveDetector("Test",runAction,trackingAction);
    //logical_block_target->SetSensitiveDetector(senDet);
    logicalConcrete->SetSensitiveDetector(senDet);

}    