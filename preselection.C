// ----------------------------------------------------------------------------
// --- This script can reduce the size of a TTree by certain selection cuts ---
// ----------------------------------------------------------------------------

#include "TFile.h"
#include "TChain.h"
#include "TString.h"
#include <iostream>

//-----------------------------------------------------------------------------------------------------------

void preselection(string ifileName,string ofileName){

  const int NPHOTONS = 10000;
  
  int nPhoton   = 0;
  bool tight[NPHOTONS] = {0};
  bool photonHasPixelSeed[NPHOTONS] = {0};
  bool loose[NPHOTONS] = {0};
  


  TFile * ifile = TFile::Open(ifileName.c_str());
  TTree * itree = (TTree*)ifile->Get("GammaJetTree");

 
  itree -> SetBranchAddress("NobjPhoton",&nPhoton);
  itree -> SetBranchAddress("PhotonIDTight",tight);
  itree -> SetBranchAddress("PhotonHasPixelSeed",photonHasPixelSeed);
  itree -> SetBranchAddress("PhotonIDLoose",loose);
  

  TFile * ofile = new TFile(ofileName.c_str(),"RECREATE");
  TTree * otree = itree->CloneTree(0);

  int cut1 = 0;
  int cut2 = 0;
  int cut3 = 0;
 
  cout<<"Number of Entries in Tree:  "<<itree->GetEntries()<<endl<<endl;

  for(int n =0; n<itree->GetEntries(); n++){
    
    itree->GetEntry(n);

    if(n%1000000 == 0) cout<<"event number = "<<n<<endl;

    if(nPhoton == 0){
     cut1 += 1;
    continue;
    }

    if(!tight[0]){
      cut2 +=1;
      continue;
    }

    if(photonHasPixelSeed[0]){
      cut3 +=1;
      continue;
    }

    otree->Fill();
  }
  
  cout<<"cut1 (nPhoton==0) = "<<cut1<<endl;
  cout<<"left after nPhoton cut = "<<itree->GetEntries()-cut1<<endl;
  cout<<"cut2 (non-tight Photons)= "<<cut2<<endl;
  cout<<"left = "<<itree->GetEntries()-cut1-cut2<<endl;
  cout<<"cut3 (photon with pixel seeds)= "<<cut3<<endl;
  cout<<"left = "<<itree->GetEntries()-cut1-cut2-cut3<<endl;
  /*  
      cout<<"cut1a (nMuon>0) = "<<cut1a<<endl;
      cout<<"left after nmuon cut = "<<itree->GetEntries()-cut1-cut1a<<endl;
      
      cout<<"cut3 (nJet20>1)= "<<cut3<<endl;
      cout<<"left = "<<itree->GetEntries()-cut1-cut1a-cut2-cut3<<endl;
  */
  /*
    cout<<"cut4 (HT>500)= "<<cut4<<endl;
    cout<<"left = "<<itree->GetEntries()-cut1-cut2-cut3-cut4<<endl;
    cout<<"cut4 (MHT>200)= "<<cut5<<endl;
    cout<<"left = "<<itree->GetEntries()-cut1-cut2-cut3-cut4-cut5<<endl;
  */
 
  otree->Write();
  ofile->Close();
  
}


void fullPreselection() {

  preselection("ak5PFCHS.root",  "OnlyTightPhotons/ak5PFCHS.root");
  preselection("ak7PFCHS.root",  "OnlyTightPhotons/ak7PFCHS.root");
  //preselection("ak5FastPF.root", "OnlyTightPhotons/ak5FastPF.root");
  //preselection("ak7FastPF.root", "OnlyTightPhotons/ak7FastPF.root");
}
