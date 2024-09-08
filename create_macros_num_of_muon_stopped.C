#include <iostream>
#include <string.h>
#include <fstream>
#include <TString.h>
#include <sys/stat.h>
using namespace std;

int create_macros_num_of_muon_stopped()
{  
  bool overwrite = false;
  const int num_events = 1e8;
  const int num_files = 10;
  
  TString basedir = "/home/iktp/Geant4-Work/NewStart/MuStoppedResult";

  
  TString TargetMaterial = "G4_Te"; // Material of cup shape target, default value is G4_Te;
  int run = 0;
  
  int num_SideThickness = 11;
  int SideThickness[] = {5, 15, 20, 30, 50, 70, 80, 100, 120, 150, 180}; // Side Thickness (in mm) of cup shape target, default value is 15 mm;

  int num_FrontThickness = 11;
  int FrontThickness[] = {5, 15, 20, 30, 50, 70, 80, 100, 120, 150, 180}; // Front Thickness (in mm) of cup shape target, default value is 15 mm;

  int num_InnerHeight = 11;
  int InnerHeight[] = {15, 20, 30, 50, 70, 80, 100, 120, 150, 180, 200}; // Inner Height (in mm) of cup shape target, default value is 200 mm;



  
//  for(int nucl_i=0; nucl_i<num_nucl; nucl_i++)
  
    //create directory
    TString dir = basedir + "/" + Form("%s",TargetMaterial.Data());
          
    /*check if file already exists
    string dir_s = (string)Form("%s/log/%s.mac",dir.Data(), outfilename.Data());
    struct stat buffer;
    bool exists_test = (stat (dir_s.c_str(), &buffer) == 0);
    if(exists_test==true && overwrite==false) {cout << Form("macros in %s already exist and will not be overwritten",dir.Data()) << endl;  continue;} // skip recreating the file if it already exists, if overwrite==true, overwrite existing files
    if(exists_test==true && overwrite==true) {cout << Form("macros in %s already exist and will be overwritten",dir.Data()) << endl;} */

  TString fileName;
  cout << "Store macros in " << dir << endl;
  // first create the directories in case they don't exist
  system(Form("mkdir -p %s/log",dir.Data()));

  for(int i_SideThickness = 0; i_SideThickness < num_SideThickness; i_SideThickness++) {
    for(int i_FrontThickness = 0; i_FrontThickness < num_FrontThickness; i_FrontThickness++) {
      for(int i_InnerHeight = i_SideThickness; i_InnerHeight < num_InnerHeight; i_InnerHeight++) {
      
	    TString FilenameBase = Form("%s_SideThickness%i_FrontThickness%i_InnerHeight%i", TargetMaterial.Data(), SideThickness[i_SideThickness], FrontThickness[i_FrontThickness], InnerHeight[i_InnerHeight]); // Filename including target preferences

            fileName = Form("%s/log/%s.mac", dir.Data(), FilenameBase.Data());

            cout << "Writing macro " << fileName << endl; 

            ofstream file(fileName);

            file << "/tracking/verbose 0" << endl;

            file << "/control/execute /home/iktp/Geant4-Work/NewStart/NumberofMuonStopped/MuMinusSource.mac" << endl;

            file << "" << endl;

            file << "/detector/SideThickness " << SideThickness[i_SideThickness] << endl;

            file << "/detector/FrontThickness " << FrontThickness[i_FrontThickness] << endl;

            file << "/detector/InnerHeight " << InnerHeight[i_InnerHeight] << endl;

            file << "/detector/TargetMaterial " << TargetMaterial <<endl;

            file << "/run/printProgress 10000" << endl;

            file <<"/run_action/out_file_name " <<basedir<<"/"<< TargetMaterial 
		 <<"/"
		 <<"SideThickness"<< SideThickness[i_SideThickness]
                 <<"_FrontThickness"<< FrontThickness[i_FrontThickness] 
		 <<"_InnerHeight"<<InnerHeight[i_InnerHeight]
                 <<"_"<<TargetMaterial<<".root" << endl;
 

            file << "" << endl;

            file << "/run/initialize" << endl;

            file << "/run/beamOn 100000" << endl;

            file.close(); 
      }
    }
  }
  
  return 0;
}  

