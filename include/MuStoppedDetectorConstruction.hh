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
/// \file MuStoppedDetectorConstruction.hh
/// \brief Definition of the MuStoppedDetectorConstruction class

#ifndef MuStoppedDetectorConstruction_h
#define MuStoppedDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4Material.hh"

#include "MySensitiveDetector.hh"

#include "G4GenericMessenger.hh"


class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction class to define materials and geometry.

class MuStoppedDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    MuStoppedDetectorConstruction();
    virtual ~MuStoppedDetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    
    G4LogicalVolume* GetTheLogicalVolumeWithEnergyRecord() const { return fTheLogicalVolumeWithEnergyRecord; }

    void SetRunAction(MuStoppedRunAction* pRunAction);
    void SetTrackingAction(MuStoppedTrackingAction* pTrackingAction);

  protected:
    G4LogicalVolume*  fTheLogicalVolumeWithEnergyRecord;

    G4double BlockLength;        
    G4double BlockWidth; 
    G4double BlockHeight;

    G4double ConcreteLength;
    G4double ConcretekWidth;
    G4double ConcreteHeight;

    G4String TargetMaterial;

    G4Box * solidWorld;
    G4Box * solidConcrete;
    G4Box * solidTarget;

    G4LogicalVolume * logicalWorld;
    G4LogicalVolume * logicalConcrete;
    //G4LogicalVolume * logical_block_target;

    G4VPhysicalVolume * physWorld;
    G4VPhysicalVolume * physConcrete;
    //G4VPhysicalVolume * phys_block_Target;

    G4Material * worldMat;
    G4Material * ConcreteMat;  
    G4Material * Target_material;

    MuStoppedRunAction* runAction;
    MuStoppedTrackingAction* trackingAction;
    MySensitiveDetector* senDet;

  private:

    void DefineMaterials();
    virtual void ConstructSDandField();
       
    G4GenericMessenger *fMesesenger;


};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

