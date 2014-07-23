// $Id: writeWeights.C,v 1.3 2011/02/04 10:35:44 mschrode Exp $

#include "TChain.h"
#include "TBranch.h"
#include "TChainElement.h"
#include "TFile.h"
#include "TString.h"

#include <iostream>

void writeWeightsToTree(const char* path, float lumi, int nev = -1, double ptHatExpo = 0., double ptHatNorm = 1.) {

  TString treename = "GammaJetTree";
  TChain* c = new TChain(treename,treename);
  c->Add(path);
  
  Float_t weight;
  Float_t xsec;
  TBranch* bCrossSection;

  c->SetBranchAddress("CrossSection", &xsec,&bCrossSection);

  Long64_t nentries = c->GetEntries();
  if(nev < 0) nev = nentries;

  bCrossSection->GetEntry(0);
  Float_t newweight = (xsec * lumi) / nev;  

  // ------------------------------------------------------------------------------------------------------------------------------
  std::cout << path << " and n events:" << nentries <<'\n';
  std::cout << "gen events:" << nev << " x-sec:" << xsec << " pb\n";
  std::cout << "new weight:" << newweight;
  if( ptHatExpo < 0. ) std::cout << " * ptHat^{" << ptHatExpo << "}";
  std::cout << '\n';
  // ------------------------------------------------------------------------------------------------------------------------------
  
  TObjArray *fileElements=c->GetListOfFiles();
  TIter next(fileElements);
  TChainElement *chEl=0;
 

  while (( chEl=(TChainElement*)next() )) {
    
    TFile f(chEl->GetTitle(),"update");
    TTree* djt = (TTree*)f.Get(treename);
    djt->SetBranchStatus("*",1);

    TBranch* br = (TBranch*)  djt->GetListOfBranches()->FindObject("NewWeight");    
    if(br) djt->SetBranchStatus("NewWeight", 0);
    else cout<<endl<<"NewWeight does not yet exist in the Tree!"<<endl<<endl;

    TTree *ndjt = djt->CloneTree(-1,"fast");
    delete djt;
    TBranch* bWeight = ndjt->Branch("NewWeight", &weight, "NewWeight/F");
    weight = newweight;
    Float_t ptHat = 0.;
    ndjt->SetBranchAddress("GenEvtScale",&ptHat);
    //read the number of entries in the t3
    nentries = ndjt->GetEntries();
    for (Long64_t i = 0; i < nentries; i++){

      if(i%1000000 == 0) cout<<"entry: "<<i<<endl;

      if( ptHatExpo < 0. ) {
	ndjt->GetEntry(i);
	weight = newweight*pow(static_cast<double>(ptHat/ptHatNorm),ptHatExpo);
	
      }
      bWeight->Fill();
    
    }
    // save only the new version of the tree
    //f.Delete(treename +";1");
    ndjt->Write("", TObject::kOverwrite);
    f.Close();
  }

  delete c;
}


 void writeWeights() {
  writeWeightsToTree("/scratch/hh/dust/naf/cms/user/telenz/mc/pythia_flat_535_START53_V22_NoNu/MCPhoton2012/ak5PFCHS.root",19790,9699664,-4.5,15.);
  writeWeightsToTree("/scratch/hh/dust/naf/cms/user/telenz/mc/pythia_flat_535_START53_V22_NoNu/MCPhoton2012/ak7PFCHS.root",19790,9699664,-4.5,15.);
 }

/*
void writeWeights() {
  writeWeightsToTree("/scratch/hh/dust/naf/cms/user/telenz/mc/qcd_pythia_flat_535_START53_V20/OnlyTightPhotons/ak5PFCHS_QCD_Pt_20_30_EMEnriched.root",10.,35040695);
  writeWeightsToTree("/scratch/hh/dust/naf/cms/user/telenz/mc/qcd_pythia_flat_535_START53_V20/OnlyTightPhotons/ak5PFCHS_QCD_Pt_30_80_EMEnriched.root",10.,33088888);
  writeWeightsToTree("/scratch/hh/dust/naf/cms/user/telenz/mc/qcd_pythia_flat_535_START53_V20/OnlyTightPhotons/ak5PFCHS_QCD_Pt_80_170_EMEnriched.root",10.,34542763);
  writeWeightsToTree("/scratch/hh/dust/naf/cms/user/telenz/mc/qcd_pythia_flat_535_START53_V20/OnlyTightPhotons/ak5PFCHS_QCD_Pt_170_250_EMEnriched.root",10.,31697066);
  writeWeightsToTree("/scratch/hh/dust/naf/cms/user/telenz/mc/qcd_pythia_flat_535_START53_V20/OnlyTightPhotons/ak5PFCHS_QCD_Pt_250_350_EMEnriched.root",10.,34611322);
  writeWeightsToTree("/scratch/hh/dust/naf/cms/user/telenz/mc/qcd_pythia_flat_535_START53_V20/OnlyTightPhotons/ak5PFCHS_QCD_Pt_350_EMEnriched.root",10.,34080562);
}
*/

/*
  void writeWeights() {
  writeWeightsToTree("/scratch/hh/dust/naf/cms/user/telenz/mc/qcd_pythia_flat_535_START53_V20/MCPhoton2012_QCD_Pt_20_30_EMEnriched/ak5FastPF.root",10.,35040695);
  writeWeightsToTree("/scratch/hh/dust/naf/cms/user/telenz/mc/qcd_pythia_flat_535_START53_V20/MCPhoton2012_QCD_Pt_20_30_EMEnriched/ak5PFCHS.root",10.,35040695);
  writeWeightsToTree("/scratch/hh/dust/naf/cms/user/telenz/mc/qcd_pythia_flat_535_START53_V20/MCPhoton2012_QCD_Pt_30_80_EMEnriched/ak5FastPF.root",10.,33088888);
  writeWeightsToTree("/scratch/hh/dust/naf/cms/user/telenz/mc/qcd_pythia_flat_535_START53_V20/MCPhoton2012_QCD_Pt_30_80_EMEnriched/ak5PFCHS.root",10.,33088888);
  writeWeightsToTree("/scratch/hh/dust/naf/cms/user/telenz/mc/qcd_pythia_flat_535_START53_V20/MCPhoton2012_QCD_Pt_80_170_EMEnriched/ak5FastPF.root",10.,34542763);
  writeWeightsToTree("/scratch/hh/dust/naf/cms/user/telenz/mc/qcd_pythia_flat_535_START53_V20/MCPhoton2012_QCD_Pt_80_170_EMEnriched/ak5PFCHS.root",10.,34542763);
  writeWeightsToTree("/scratch/hh/dust/naf/cms/user/telenz/mc/qcd_pythia_flat_535_START53_V20/MCPhoton2012_QCD_Pt_170_250_EMEnriched/ak5FastPF.root",10.,31697066);
  writeWeightsToTree("/scratch/hh/dust/naf/cms/user/telenz/mc/qcd_pythia_flat_535_START53_V20/MCPhoton2012_QCD_Pt_170_250_EMEnriched/ak5PFCHS.root",10.,31697066);
  writeWeightsToTree("/scratch/hh/dust/naf/cms/user/telenz/mc/qcd_pythia_flat_535_START53_V20/MCPhoton2012_QCD_Pt_250_350_EMEnriched/ak5FastPF.root",10.,34611322);
  writeWeightsToTree("/scratch/hh/dust/naf/cms/user/telenz/mc/qcd_pythia_flat_535_START53_V20/MCPhoton2012_QCD_Pt_250_350_EMEnriched/ak5PFCHS.root",10.,34611322);
  writeWeightsToTree("/scratch/hh/dust/naf/cms/user/telenz/mc/qcd_pythia_flat_535_START53_V20/MCPhoton2012_QCD_Pt_350_EMEnriched/ak5FastPF.root",10.,34080562);
  writeWeightsToTree("/scratch/hh/dust/naf/cms/user/telenz/mc/qcd_pythia_flat_535_START53_V20/MCPhoton2012_QCD_Pt_350_EMEnriched/ak5PFCHS.root",10.,34080562);
  }
*/
/*
 
*/

/*
  void writeWeights() {
  writeWeightsToTree("/scratch/hh/current/cms/user/telenz/mc/PhotonJetTuple2012/MCPhoton2012_15to30/ak5FastPF*.root",10.,1970745);
  writeWeightsToTree("/scratch/hh/current/cms/user/telenz/mc/PhotonJetTuple2012/MCPhoton2012_30to50/ak5FastPF*.root",10.,1993325);
  writeWeightsToTree("/scratch/hh/current/cms/user/telenz/mc/PhotonJetTuple2012/MCPhoton2012_50to80/ak5FastPF*.root",10.,1995062);
  writeWeightsToTree("/scratch/hh/current/cms/user/telenz/mc/PhotonJetTuple2012/MCPhoton2012_80to120/ak5FastPF*.root",10.,1992627);
  writeWeightsToTree("/scratch/hh/current/cms/user/telenz/mc/PhotonJetTuple2012/MCPhoton2012_120to170/ak5FastPF*.root",10.,1989603);
  writeWeightsToTree("/scratch/hh/current/cms/user/telenz/mc/PhotonJetTuple2012/MCPhoton2012_170to300/ak5FastPF*.root",10.,2000069);
  writeWeightsToTree("/scratch/hh/current/cms/user/telenz/mc/PhotonJetTuple2012/MCPhoton2012_300to470/ak5FastPF*.root",10.,2000130);
  writeWeightsToTree("/scratch/hh/current/cms/user/telenz/mc/PhotonJetTuple2012/MCPhoton2012_470to800/ak5FastPF*.root",10.,1975231);
  writeWeightsToTree("/scratch/hh/current/cms/user/telenz/mc/PhotonJetTuple2012/MCPhoton2012_800to1400/ak5FastPF*.root",10.,1973504);
  writeWeightsToTree("/scratch/hh/current/cms/user/telenz/mc/PhotonJetTuple2012/MCPhoton2012_1400to1800/ak5FastPF*.root",10.,1984890);
  writeWeightsToTree("/scratch/hh/current/cms/user/telenz/mc/PhotonJetTuple2012/MCPhoton2012_1800/ak5FastPF*.root",10.,1939122);
  }
*/

/*
  void writeWeights() {
  writeWeightsToTree("/scratch/hh/current/cms/user/telenz/mc/PhotonJetTuple2012/MCPhoton2012_15to30/ak5PFCHS.root",10.,1970745);
  writeWeightsToTree("/scratch/hh/current/cms/user/telenz/mc/PhotonJetTuple2012/MCPhoton2012_30to50/ak5PFCHS.root",10.,1993325);
  writeWeightsToTree("/scratch/hh/current/cms/user/telenz/mc/PhotonJetTuple2012/MCPhoton2012_50to80/ak5PFCHS.root",10.,1995062);
  writeWeightsToTree("/scratch/hh/current/cms/user/telenz/mc/PhotonJetTuple2012/MCPhoton2012_80to120/ak5PFCHS.root",10.,1992627);
  writeWeightsToTree("/scratch/hh/current/cms/user/telenz/mc/PhotonJetTuple2012/MCPhoton2012_120to170/ak5PFCHS.root",10.,1989603);
  writeWeightsToTree("/scratch/hh/current/cms/user/telenz/mc/PhotonJetTuple2012/MCPhoton2012_170to300/ak5PFCHS.root",10.,2000069);
  writeWeightsToTree("/scratch/hh/current/cms/user/telenz/mc/PhotonJetTuple2012/MCPhoton2012_300to470/ak5PFCHS.root",10.,2000130);
  writeWeightsToTree("/scratch/hh/current/cms/user/telenz/mc/PhotonJetTuple2012/MCPhoton2012_470to800/ak5PFCHS.root",10.,1975231);
  writeWeightsToTree("/scratch/hh/current/cms/user/telenz/mc/PhotonJetTuple2012/MCPhoton2012_800to1400/ak5PFCHS.root",10.,1973504);
  writeWeightsToTree("/scratch/hh/current/cms/user/telenz/mc/PhotonJetTuple2012/MCPhoton2012_1400to1800/ak5PFCHS.root",10.,1984890);
  writeWeightsToTree("/scratch/hh/current/cms/user/telenz/mc/PhotonJetTuple2012/MCPhoton2012_1800/ak5PFCHS.root",10.,1939122);
  }
*/

void testWeights() 
{
  TChain* c = new TChain("DiJetTree","DiJetTree");
  c->Add("/scratch/hh/current/cms/user/stadie/QCDDiJetSummer10-START36_V9_S09-v1A/Pt*ak5Calo.root");
  c->Draw("JetPt[0] >>hpt(700,0,3500)","Weight");
}
/*
void writeWeights() {
  writeWeightsToTree("/scratch/hh/dust/naf/cms/user/telenz/mc/pythia_flat_53/MCPhoton2012/ak5FastPF.root",10.,9699664,-4.5,15.);
  writeWeightsToTree("/scratch/hh/dust/naf/cms/user/telenz/mc/pythia_flat_53/MCPhoton2012/ak5PFCHS.root",10.,9699664,-4.5,15.);
}
*/



