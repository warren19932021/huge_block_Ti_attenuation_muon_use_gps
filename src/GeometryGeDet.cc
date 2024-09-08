
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "GeometryGeDet.hh"

#include "G4Tubs.hh"
#include "G4Torus.hh"
#include "G4Sphere.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4NistManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GeometryGeDet::GeometryGeDet(DetType type, G4bool overlap_check)
	: fType(type), fCheckOverlaps(overlap_check), 
	fLogicDetVolume(NULL), fLogicActiveRegion(NULL), 
	fDetVolumeLength(0), fDetVolumeRmax(0), fDetVolumeMaterial(NULL), 
	fCrystalMaterial(NULL), fCrystalDeadLayerMaterial(NULL), 
	fIS_SideMaterial(NULL), fIS_BotMaterial(NULL), fIS_TopMaterial(NULL),
	fOS_SideMaterial(NULL), fOS_TopMaterial(NULL)
{
   SetDefaults();
}



GeometryGeDet::~GeometryGeDet()
{
}



void GeometryGeDet::SetDefaults()
{
	// >>>> Materials:
	// ------------------------------------
/*
   G4NistManager * nist = G4NistManager::Instance();
   fDetVolumeMaterial = nist->FindOrBuildMaterial("G4_Ge");
   fCrystalMaterial = nist->FindOrBuildMaterial("G4_Ge");
   fCrystalDeadLayerMaterial = nist->FindOrBuildMaterial("G4_Ge");
   fIS_SideMaterial = nist->FindOrBuildMaterial("G4_Cu");
   fIS_BotMaterial = nist->FindOrBuildMaterial("G4_Cu");
   fIS_TopMaterial = nist->FindOrBuildMaterial("G4_Ge");
   fOS_SideMaterial = nist->FindOrBuildMaterial("G4_Cu");
   fOS_TopMaterial = nist->FindOrBuildMaterial("G4_Be");

*/


	// >>>> Materials:
	// ------------------------------------
	
	fDetVolumeMaterial = G4Material::GetMaterial("Vacuum");;
	
	fCrystalMaterial = G4Material::GetMaterial("NaturalGe");
	
	fCrystalDeadLayerMaterial = G4Material::GetMaterial("GeLi");
	
	// Note: DeadLayerMaterial is always the one IN THE FRONT
	
	// Borehole has other material, wich is not considered here... (same material chosen!)
	
	fIS_SideMaterial = G4Material::GetMaterial("G4_Cu");
	
	fIS_BotMaterial = G4Material::GetMaterial("G4_Cu");
	
	fIS_TopMaterial = G4Material::GetMaterial("PET");
	
	fOS_SideMaterial = G4Material::GetMaterial("G4_Cu");
	
	fOS_TopMaterial = G4Material::GetMaterial("G4_Be");



	// >>>> Sizes:
	// ------------------------------------
	fCrystalRadius = (52.5 / 2.0) * mm;
	fCrystalLength = 55.3 * mm;
	fCoreDepth = 47.8 * mm;
	fCoreRadius = (9.9 / 2.0) * mm;
	fCoreSphereOn = true;

	fCornerRadius = 8.0 * mm;

	fDeadLayerConsidered = true;
	fOuterDeadLayerThickness = 0.0003*mm;
	fCoreDeadLayerThickness = 0.7*mm;

	fExcessSideDeadLayerConsidered = false;
	fExcessSideDeadLayerThickness = 0.0 * mm;

	fCutInHalf = false;

	// IS = Inner shield
	fISConsidered = true;
	fIS_Side_Thickness = 0.8 * mm;
	fIS_Side_Radius = fCrystalRadius + 3.0 * mm;
	fIS_Side_Length = 91.0*mm;

	fIS_Bot_Thickness = 3.0 * mm;
	fIS_Bot_InnerRadius = fCoreRadius;
	fIS_Bot_OuterRadius = fIS_Side_Radius + fIS_Side_Thickness;

	fIS_Top_Thickness = 0.03 * mm;
	fIS_Top_OuterRadius = fIS_Side_Radius + fIS_Side_Thickness;

	// OS = Outer shield
	fOSConsidered = true;
	fDistanceTopIS_OS = 3.0*mm;

	fOS_Side_Thickness = 1.7 * mm;
	fOS_Side_Radius = fIS_Side_Radius + 6.0 * mm;
	fOS_Side_Length = fIS_Side_Length + fIS_Top_Thickness + fIS_Bot_Thickness + fDistanceTopIS_OS;

	fOS_Top_Thickness = 0.5 * mm;
	fOS_Top_OuterRadius = fOS_Side_Radius + fOS_Side_Thickness;
}

bool GeometryGeDet::CreateGeometry(G4LogicalVolume * logicWorld)
{
	// ---------------------------------------------- ----------------------------------------------
	// -------------------------------------- Calculate additional Sizes:
	// ---------------------------------------------- ----------------------------------------------
	G4double eps = 0.001 * cm;	// displacement to avoid overlapping surfaces! IMPORTANT!

	if (!fISConsidered)
	{
		// If no IS -> set all sizes to zero!
		fIS_Side_Thickness = 0.0 * mm;
		fIS_Side_Radius = 0.0 * mm;
		fIS_Side_Length = 0.0 * mm;

		fIS_Bot_Thickness = 0.0 * mm;
		fIS_Bot_InnerRadius = 0.0 * mm;
		fIS_Bot_OuterRadius = 0.0 * mm;

		fIS_Top_Thickness = 0.0 * mm;
		fIS_Top_OuterRadius = 0.0 * mm;
	}
	if (!fOSConsidered)
	{
		// If no OS -> set all sizes to zero!
		fDistanceTopIS_OS = 0.0 * mm;

		fOS_Side_Thickness = 0.0 * mm;
		fOS_Side_Radius = 0.0 * mm;
		fOS_Side_Length = 0.0 * mm;

		fOS_Top_Thickness = 0.0 * mm;
		fOS_Top_OuterRadius = 0.0 * mm;
	}

	// >> DetectorVolume Size
	if (fOSConsidered)
	{
		fDetVolumeRmax = fOS_Side_Radius + fOS_Side_Thickness;
	}
	else if (fISConsidered)
	{
		fDetVolumeRmax = fIS_Side_Radius + fIS_Side_Thickness;
	}
	else
	{
		fDetVolumeRmax = fCrystalRadius;
	}

	if (fOSConsidered && (fOS_Side_Length >= (fIS_Side_Length + fIS_Top_Thickness + fIS_Bot_Thickness + fDistanceTopIS_OS)))
	{
		fDetVolumeLength = fOS_Side_Length + fOS_Top_Thickness + eps;
	}
	else if(fISConsidered)
	{
		fDetVolumeLength = fIS_Side_Length + fIS_Top_Thickness + fIS_Bot_Thickness + fDistanceTopIS_OS + fOS_Top_Thickness + eps;
	}
	else
	{
		fDetVolumeLength = fCrystalLength;
	}




	// ---------------------------------------------- ----------------------------------------------
	// -------------------------------------- Construct:
	// ---------------------------------------------- ----------------------------------------------

	// -------------------------------------- DetectorVolume:
	// ------------------------------------------------------
	// Solid / Shape:
	G4Tubs* solidDetVolume = new G4Tubs(
		"DetVolume",		// name
		0,					// inner radius (pRMin)
		fDetVolumeRmax,		// outer radius (pRMax)
		0.5*fDetVolumeLength,// half hight (pDz)
		0.0*deg,			// starting-phi in rad (pSPhi)
		360.0*deg);			// angel of segment in rad (pDPhi)

	// Logic-Volume:
	fLogicDetVolume = new G4LogicalVolume(
		solidDetVolume,		// solid
		fDetVolumeMaterial,	// material
		"DetVolume");		// name

/*

	// Placement in Det-Const!
	G4double fDetAngle = 0.0;
	//G4ThreeVector positionDetVolume;
	G4ThreeVector positionDetVolume(0,0,200);
	G4ThreeVector fOffset;
	G4RotationMatrix rotMatrix;
	//rotMatrix.rotateY(-fDetAngle);
	rotMatrix.rotateX(-fDetAngle);
	new G4PVPlacement(G4Transform3D(rotMatrix, positionDetVolume + fOffset),	// rotation + pos
		fLogicDetVolume,							// logical volume
		"DetVolume",									// name
		logicWorld,										// mother volume (always Null-Ptr for world)
		false,											// no boolean operation
		0,												// copy number
		fCheckOverlaps);								// overlaps checking  
*/

	// Placement in Det-Const!


/*

     for(G4int j = 0; j < 4; j++)
      {

            G4Rotate3D rotZ(j*90.0*deg, G4ThreeVector(0,1,0));
            G4Translate3D transXScint(G4ThreeVector((0.*deg)*cm+0.*cm, 0.*cm, 10.*cm));
            G4Transform3D transformDetVolume = (rotZ)*(transXScint);

            new G4PVPlacement(transformDetVolume, 
			fLogicDetVolume,							// logical volume
   		    "DetVolume",									// name
		    logicWorld,										// mother volume (always Null-Ptr for world)
		    false,											// no boolean operation
		    0,												// copy number
		    fCheckOverlaps);	

      }


*/



     for(G4int j = 0; j < 1; j++)
     //for(G4int j = 0; j < 2; j++)
      {

            //G4Rotate3D rotZ(j*90.0*deg, G4ThreeVector(0,0,1));
            //G4Rotate3D rotZ(90.0*deg+j*180.0*deg, G4ThreeVector(1,0,0));
            G4Rotate3D rotZ(180.0*deg+j*180.0*deg, G4ThreeVector(1,0,0));
            //G4Translate3D transGedetctor(G4ThreeVector((0.*deg)*cm+0.*cm, (30.*deg)*10.*cm, 10.*cm));
            //G4Translate3D transGedetctor(G4ThreeVector(0.*cm,0.*cm, 60.*cm));
            G4Translate3D transGedetctor(G4ThreeVector(0.*cm,0.*cm, 5.*cm));
            G4Transform3D transformDetVolume = (rotZ)*(transGedetctor);
            //G4Transform3D transformDetVolume = transGedetctor;

            new G4PVPlacement(transformDetVolume, 
			fLogicDetVolume,							// logical volume
   		    "DetVolume",									// name
		    logicWorld,										// mother volume (always Null-Ptr for world)
		    false,											// no boolean operation
		    0,												// copy number
		    fCheckOverlaps);	

      }






	// -------------------------------------- Crystal:
	// -----------------------------------------------
	G4VSolid *solidNoCoreCrystal;	// Crystal without core.
	G4VSolid *solidCrystal;	// Final G4 Solid representing crystal  

	// 1) Crystal with no core; Build round edge if required
	if (fCornerRadius > 0.)
	{
		// with round edge:
		G4Tubs *mainCylinder1 = new G4Tubs("mainCylinder1", 0., fCrystalRadius, (fCrystalLength - fCornerRadius) / 2., 0 * deg, 360.0*deg);
		G4Tubs *mainCylinder2 = new G4Tubs("mainCylinder2", 0., fCrystalRadius - fCornerRadius, fCrystalLength / 2., 0 * deg, 360.0*deg);
		G4UnionSolid *mainCylinder = new G4UnionSolid("mainCylinder", mainCylinder2, mainCylinder1, 0, G4ThreeVector(0, 0, fCornerRadius / 2.));

		G4Torus *cornerTorus = new G4Torus("cornerTorus", 0., fCornerRadius, fCrystalRadius - fCornerRadius - eps, 0.*deg, 360.*deg);

		solidNoCoreCrystal = new G4UnionSolid("crystalNoCore", mainCylinder, cornerTorus, 0, G4ThreeVector(0, 0, -(fCrystalLength / 2. - fCornerRadius)));
	}
	else
	{
		// only cylinder = Tube:
		solidNoCoreCrystal = new G4Tubs("crystalNoCore", 0.*cm, fCrystalRadius, fCrystalLength / 2., 0 * deg, 360.0*deg);
	}

	// 2) remove Core from Crystal if required
	if (fCoreRadius > 0.)
	{
		// XYZ + eps is important for visualisation and avoiding SurfaceOverlaps!!!
		G4Tubs *coreCylinder = new G4Tubs("crystalCoreCylinder", 0 * cm, fCoreRadius, (fCoreDepth + eps) / 2., 0 * deg, 360.0*deg);

		if (fCoreSphereOn)
		{
			G4Sphere *coreSphere = new G4Sphere("crystalCoreSphere", 0 * cm, fCoreRadius + eps, 0 * deg, 360.0*deg, 0 * deg, 180.0*deg);
			G4VSolid *tempSolid;

			tempSolid = new G4SubtractionSolid("crystalWithCore_temp", solidNoCoreCrystal, coreSphere, 0, G4ThreeVector(0, 0, -(fCoreDepth*2.0 - fCrystalLength - eps) / 2. + fCoreRadius));
			solidCrystal = new G4SubtractionSolid("crystalWithCore", tempSolid, coreCylinder, 0, G4ThreeVector(0, 0, -(fCoreDepth - fCrystalLength - eps) / 2. + fCoreRadius));
		}
		else
		{
			solidCrystal = new G4SubtractionSolid("crystalWithCore", solidNoCoreCrystal, coreCylinder, 0, G4ThreeVector(0, 0, -(fCoreDepth - fCrystalLength - eps) / 2.));
		}
	}
	else
	{
		solidCrystal = solidNoCoreCrystal;
	}

	// Debugging Option: Cut Crytsal in half ~
	if (fCutInHalf)
	{
		G4cout << "Creating half Crystal." << G4endl;
		G4Box *bigBox = new G4Box("bigBox", fDetVolumeLength, fDetVolumeLength, fDetVolumeLength);
		solidCrystal = new G4SubtractionSolid("halfCrystal", solidCrystal, bigBox, 0, G4ThreeVector(-fDetVolumeLength, 0., 0.));
	}

	// 3) Logical+Physical Volume --> Placement in DetVolume
	G4LogicalVolume* logicCrystal;
	// if no dead-layer is Considered the whole crystal = active crystal (else active region INSIDE this crystal)
	if (fDeadLayerConsidered)
	{
		logicCrystal = new G4LogicalVolume(solidCrystal, fCrystalDeadLayerMaterial, "Crystal");
	}
	else
	{
		logicCrystal = new G4LogicalVolume(solidCrystal, fCrystalMaterial, "activeCrystal");
		fLogicActiveRegion = logicCrystal;
	}
	G4ThreeVector CrystalPos = G4ThreeVector(0, 0, -(fDetVolumeLength - fCrystalLength) / 2.0 + fDistanceTopIS_OS + fOS_Top_Thickness + fIS_Top_Thickness);
	G4VPhysicalVolume* physCrystal = new G4PVPlacement(0, CrystalPos, logicCrystal, "Crystal", fLogicDetVolume, false, 0, fCheckOverlaps);



	// -------------------------------------- Crystal - active region:
	// ---------------------------------------------------------------
	// Create an active logical volume that is placed inside logicCrystal
	if (fDeadLayerConsidered)
	{
		G4VSolid *solidActiveNoCoreCrystal;
		G4VSolid *solidActiveCrystal;

		// 1) Compute dimensions of active region solid
		G4double activeRadius;
		G4double activeCornerRadius;
		G4double activeHeight = fCrystalLength - fOuterDeadLayerThickness;
		G4double activeCoreRadius = fCoreRadius + fCoreDeadLayerThickness;
		G4double activeCoreDepth = fCoreDepth + fCoreDeadLayerThickness;

		if (fExcessSideDeadLayerConsidered)
		{
			// dead layer side > dead layer top
			activeRadius = fCrystalRadius - fOuterDeadLayerThickness - fExcessSideDeadLayerThickness;
			activeCornerRadius = fCornerRadius - fOuterDeadLayerThickness - fExcessSideDeadLayerThickness;

		}
		else
		{
			activeRadius = fCrystalRadius - fOuterDeadLayerThickness;
			activeCornerRadius = fCornerRadius - fOuterDeadLayerThickness;
		}

		// 2) Activ Region with no core; build round edge if required
		if (activeRadius > 0. && activeCornerRadius > 0.0)
		{
			G4Tubs *activeCylinder1 = new G4Tubs("activeCylinder1", 0., activeRadius, (activeHeight - activeCornerRadius) / 2., 0 * deg, 360.0*deg);
			G4Tubs *activeCylinder2 = new G4Tubs("activeCylinder2", 0., activeRadius - activeCornerRadius, activeHeight / 2., 0 * deg, 360.0*deg);
			G4UnionSolid *activeCylinder = new G4UnionSolid("activeCylinder", activeCylinder2, activeCylinder1, 0, G4ThreeVector(0, 0, activeCornerRadius / 2.));

			G4Torus *activeCornerTorus = new G4Torus("activeCornerTorus", 0., activeCornerRadius, activeRadius - activeCornerRadius - eps, 0.*deg, 360.*deg);

			solidActiveNoCoreCrystal = new G4UnionSolid("aCrystalNoCore", activeCylinder, activeCornerTorus, 0, G4ThreeVector(0, 0, -(activeHeight / 2. - activeCornerRadius)));
		}
		else
		{
			solidActiveNoCoreCrystal = new G4Tubs("aCrystalNoCore", 0.*cm, activeRadius, activeHeight / 2., 0 * deg, 360.0*deg);
		}

		// 3) remove Core from Activ Region if required
		if (activeCoreRadius > 0.)
		{
			G4Tubs *activeCoreCylinder = new G4Tubs("aCrystalCoreCylinder", 0 * cm, activeCoreRadius, (activeCoreDepth + eps) / 2., 0 * deg, 360.0*deg);

			if (fCoreSphereOn)
			{
				// "activeCoreRadius+eps" IS IMPORTANT for visualisation etc!
				G4Sphere *activeCoreSphere = new G4Sphere("aCrystalCoreSphere", 0 * cm, activeCoreRadius + eps, 0 * deg, 360.0*deg, 0 * deg, 180.0*deg);
				G4VSolid *tempSolid2;

				tempSolid2 = new G4SubtractionSolid("aCrystalWithCore_temp", solidActiveNoCoreCrystal, activeCoreSphere, 0, G4ThreeVector(0, 0, -(activeCoreDepth*2.0 - activeHeight - eps) / 2. + activeCoreRadius));
				solidActiveCrystal = new G4SubtractionSolid("aCrystalWithCore", tempSolid2, activeCoreCylinder, 0, G4ThreeVector(0, 0, -(activeCoreDepth - activeHeight - eps) / 2. + activeCoreRadius));
			}
			else
			{
				solidActiveCrystal = new G4SubtractionSolid("aCrystalWithCore", solidActiveNoCoreCrystal, activeCoreCylinder, 0, G4ThreeVector(0, 0, -(activeCoreDepth - activeHeight - eps) / 2.));
			}
		}
		else
		{
			solidActiveCrystal = solidActiveNoCoreCrystal;
		}

		// Debugging Option: Cut Crytsal in half ~
		if (fCutInHalf)
		{
			G4cout << "Creating half active-Crystal." << G4endl;
			G4Box *bigBox = new G4Box("bigBox", fDetVolumeLength, fDetVolumeLength, fDetVolumeLength);
			solidActiveCrystal = new G4SubtractionSolid("halfCrystalActive", solidActiveCrystal, bigBox, 0, G4ThreeVector(-fDetVolumeLength, 0., 0.));
		}

		// 4) Logical+Physical Volume --> Placement in Crystal
		G4LogicalVolume* logicActiveCrystal = new G4LogicalVolume(solidActiveCrystal, fCrystalMaterial, "activeCrystal", 0, 0, 0, true);
		G4VPhysicalVolume* physActiveCrystal = new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, fOuterDeadLayerThickness / 2.), logicActiveCrystal, "activeVolInCrystal", logicCrystal, false, 0, fCheckOverlaps);
		fLogicActiveRegion = logicActiveCrystal;
	}



	// -------------------------------------- Shielding:
	// -------------------------------------------------
	G4LogicalVolume* logicIS_Bot = NULL;
	G4LogicalVolume* logicIS_Side = NULL;
	G4LogicalVolume* logicIS_Top = NULL;
	G4LogicalVolume* logicOS_Side = NULL;
	G4LogicalVolume* logicOS_Top = NULL;
	if (fISConsidered)
	{
		// Inner Shield
		G4VSolid *solidIS_Side = new G4Tubs("IS_Side", fIS_Side_Radius, fIS_Side_Radius + fIS_Side_Thickness, fIS_Side_Length / 2., 0 * deg, 360.0*deg);
		// Debugging Option:
		if (fCutInHalf)
		{
			G4Box *bigBox = new G4Box("bigBox", fDetVolumeLength, fDetVolumeLength, fDetVolumeLength);
			solidIS_Side = new G4SubtractionSolid("halfCrystalActive", solidIS_Side, bigBox, 0, G4ThreeVector(-fDetVolumeLength, 0., 0.));
		}
		logicIS_Side = new G4LogicalVolume(solidIS_Side, fIS_SideMaterial, "IS_Side", 0, 0, 0, true);
		G4ThreeVector IS_SidePos = G4ThreeVector(0, 0, -(fDetVolumeLength - fIS_Side_Length) / 2.0 + fOS_Top_Thickness + fDistanceTopIS_OS + fIS_Top_Thickness);
		G4VPhysicalVolume* physIS_Side = new G4PVPlacement(0, IS_SidePos, logicIS_Side, "IS_Side", fLogicDetVolume, false, 0, fCheckOverlaps);

		G4VSolid *solidIS_Top = new G4Tubs("IS_Top", 0, fIS_Top_OuterRadius, fIS_Top_Thickness / 2., 0 * deg, 360.0*deg);
		if (fCutInHalf)
		{
			G4Box *bigBox = new G4Box("bigBox", fDetVolumeLength, fDetVolumeLength, fDetVolumeLength);
			solidIS_Top = new G4SubtractionSolid("halfCrystalActive", solidIS_Top, bigBox, 0, G4ThreeVector(-fDetVolumeLength, 0., 0.));
		}
		logicIS_Top = new G4LogicalVolume(solidIS_Top, fIS_TopMaterial, "IS_Top", 0, 0, 0, true);
		G4ThreeVector IS_TopPos = G4ThreeVector(0, 0, -fDetVolumeLength / 2.0 + fOS_Top_Thickness + fDistanceTopIS_OS + fIS_Top_Thickness / 2.0);
		G4VPhysicalVolume* physIS_Top = new G4PVPlacement(0, IS_TopPos, logicIS_Top, "IS_Top", fLogicDetVolume, false, 0, fCheckOverlaps);

		G4VSolid *solidIS_Bot = new G4Tubs("IS_Bot", fIS_Bot_InnerRadius, fIS_Bot_OuterRadius, fIS_Bot_Thickness / 2., 0 * deg, 360.0*deg);
		if (fCutInHalf)
		{
			G4Box *bigBox = new G4Box("bigBox", fDetVolumeLength, fDetVolumeLength, fDetVolumeLength);
			solidIS_Bot = new G4SubtractionSolid("halfCrystalActive", solidIS_Bot, bigBox, 0, G4ThreeVector(-fDetVolumeLength, 0., 0.));
		}
		logicIS_Bot = new G4LogicalVolume(solidIS_Bot, fIS_BotMaterial, "IS_Bot", 0, 0, 0, true);
		G4ThreeVector IS_BotPos = G4ThreeVector(0, 0, -fDetVolumeLength / 2.0 + fOS_Top_Thickness + fDistanceTopIS_OS + fIS_Top_Thickness + fIS_Side_Length + fIS_Bot_Thickness / 2.0);
		G4VPhysicalVolume* physIS_Bot = new G4PVPlacement(0, IS_BotPos, logicIS_Bot, "IS_Bot", fLogicDetVolume, false, 0, fCheckOverlaps);
	}
	if (fOSConsidered)
	{
		// Outer shield
		G4VSolid *solidOS_Side = new G4Tubs("OS_Side", fOS_Side_Radius, fOS_Side_Radius + fOS_Side_Thickness, fOS_Side_Length / 2., 0 * deg, 360.0*deg);
		if (fCutInHalf)
		{
			G4Box *bigBox = new G4Box("bigBox", fDetVolumeLength, fDetVolumeLength, fDetVolumeLength);
			solidOS_Side = new G4SubtractionSolid("halfCrystalActive", solidOS_Side, bigBox, 0, G4ThreeVector(-fDetVolumeLength, 0., 0.));
		}
		logicOS_Side = new G4LogicalVolume(solidOS_Side, fOS_SideMaterial, "OS_Side", 0, 0, 0, true);
		G4ThreeVector OS_SidePos = G4ThreeVector(0, 0, -(fDetVolumeLength - fOS_Side_Length) / 2.0 + fOS_Top_Thickness);
		G4VPhysicalVolume* physOS_Side = new G4PVPlacement(0, OS_SidePos, logicOS_Side, "OS_Side", fLogicDetVolume, false, 0, fCheckOverlaps);

		G4VSolid *solidOS_Top = new G4Tubs("OS_Top", 0, fOS_Top_OuterRadius, fOS_Top_Thickness / 2., 0 * deg, 360.0*deg);
		if (fCutInHalf)
		{
			G4Box *bigBox = new G4Box("bigBox", fDetVolumeLength, fDetVolumeLength, fDetVolumeLength);
			solidOS_Top = new G4SubtractionSolid("halfCrystalActive", solidOS_Top, bigBox, 0, G4ThreeVector(-fDetVolumeLength, 0., 0.));
		}
		logicOS_Top = new G4LogicalVolume(solidOS_Top, fOS_TopMaterial, "OS_Top", 0, 0, 0, true);
		G4ThreeVector OS_TopPos = G4ThreeVector(0, 0, (-fDetVolumeLength + fOS_Top_Thickness) / 2.0);
		G4VPhysicalVolume* physOS_Top = new G4PVPlacement(0, OS_TopPos, logicOS_Top, "OS_Top", fLogicDetVolume, false, 0, fCheckOverlaps);
	}

	// ---------------------------------------------- ----------------------------------------------
	// -------------------------------------- Viss-Attributes:
	// ---------------------------------------------- ----------------------------------------------
	G4VisAttributes *InvisibleVolVisAtt = new G4VisAttributes(false);
	G4VisAttributes *WhiteWireframeVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
	WhiteWireframeVisAtt->SetForceWireframe(true);
	G4VisAttributes *BlueVisAtt = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0));
	G4VisAttributes *RedVisAtt = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));
	G4VisAttributes *CyanVisAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0));
	//G4VisAttributes *GrayVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5));

	fLogicDetVolume->SetVisAttributes(InvisibleVolVisAtt);
	logicCrystal->SetVisAttributes(RedVisAtt);
	if (fISConsidered)
	{
		logicIS_Bot->SetVisAttributes(CyanVisAtt);
		logicIS_Side->SetVisAttributes(CyanVisAtt);
		logicIS_Top->SetVisAttributes(CyanVisAtt);
	}
	if (fOSConsidered)
	{
		logicOS_Side->SetVisAttributes(BlueVisAtt);
		logicOS_Top->SetVisAttributes(BlueVisAtt);
	}

	return true;
}
