
#ifndef GeometryGeDet_h
#define GeometryGeDet_h 1

#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"



/// Geometry Class for Ideal Ge-Det (coaxial)
///
/// It builds:
/// - Ge-Det Crystall
/// - Detector Shielding
/// 
/// Messenger-Funktion over generell DetectorConstruction-Class!


class GeometryGeDet
{
	public:
		enum DetType { Comet, Iktp_nType, Iktp_pType, CdZnTe, Test };
		GeometryGeDet(DetType type, G4bool overlap_check);
		virtual ~GeometryGeDet();


	public:
		bool CreateGeometry(G4LogicalVolume * logicWorld);
		void SetDefaults();

		// get methods
		G4LogicalVolume* GetLogical()	{ return fLogicDetVolume; }
		G4LogicalVolume* GetActiveLogical() { return fLogicActiveRegion; };
		G4double GetDetVolumeLength()	{ return fDetVolumeLength; }
		G4double GetDetVolumeRmax()		{ return fDetVolumeRmax; }
		G4double GetCrystalRmax()			{ return fCrystalRadius; }

		// set methods (used by messenger, see DetectorConstruction.cc)
		void SetOverlapCheck(bool value) { fCheckOverlaps = value; }

	private:
		DetType		fType;
		G4bool		fCheckOverlaps;

		// Logical Volume in wich the whole detector is positioned
		G4LogicalVolume*	fLogicDetVolume;
		// Logical Volume of the active region
		G4LogicalVolume*	fLogicActiveRegion;

		// ---> Variables with getters...
		G4double	fDetVolumeLength;
		G4double	fDetVolumeRmax;

		// Member-Variables (Sizes, create set-/getter if needed)
		G4double fCrystalRadius;
		G4double fCrystalLength;
		G4double fCornerRadius;

		G4bool fCoreSphereOn;
		G4double fCoreDepth;
		G4double fCoreRadius;
		
		G4bool fDeadLayerConsidered;			// option for enabling dead-layer
		G4double fOuterDeadLayerThickness;
		G4double fCoreDeadLayerThickness;

		G4bool fExcessSideDeadLayerConsidered;	// option for more dead layer at side compared to top
		G4double fExcessSideDeadLayerThickness;

		G4bool fCutInHalf; // Debug Option 

		G4double fDistanceTopIS_OS;

		// IS = Inner shield (insert bools for Shield-Disabling if needed)
		G4bool	 fISConsidered;
		G4double fIS_Side_Thickness;
		G4double fIS_Side_Radius;
		G4double fIS_Side_Length;

		G4double fIS_Bot_Thickness;
		G4double fIS_Bot_InnerRadius;
		G4double fIS_Bot_OuterRadius;

		G4double fIS_Top_Thickness;
		G4double fIS_Top_OuterRadius;

		// OS = Outer shield (insert bools for Shield-Disabling if needed)	
		G4bool	 fOSConsidered;
		G4double fOS_Side_Thickness;
		G4double fOS_Side_Radius;	// fOS_Side_Radius > fIS_Side_Radius + fIS_Side_Thickness
		G4double fOS_Side_Length;

		G4double fOS_Top_Thickness;
		G4double fOS_Top_OuterRadius;

		// Material pointers:
		G4Material*	fDetVolumeMaterial;
		G4Material*	fCrystalMaterial;
		G4Material* fCrystalDeadLayerMaterial;
		G4Material*	fIS_SideMaterial;
		G4Material*	fIS_BotMaterial;
		G4Material*	fIS_TopMaterial;
		G4Material*	fOS_SideMaterial;
		G4Material*	fOS_TopMaterial;



};

#endif