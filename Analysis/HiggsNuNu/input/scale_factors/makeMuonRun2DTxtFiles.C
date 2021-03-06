#include "TFile.h"
#include "TGraphAsymmErrors.h"
#include "TH2F.h"

#include <iostream>
#include <sstream>
#include <fstream>



int makeMuonRun2DTxtFiles(){//main

  TFile *muId_ = new TFile("MuonID_Z_RunCD_Reco76X_Feb15.root");
  TFile *muIso_ = new TFile("MuonIso_Z_RunCD_Reco76X_Feb15.root");

  double extraIdSyst = 0.01;
  double extraIsoSyst = 0.005;

  TH2F *hist_muon[4][3];

  muId_->cd("MC_NUM_LooseID_DEN_genTracks_PAR_pt_spliteta_bin1/efficienciesDATA/");
  hist_muon[0][0] = (TH2F*)gDirectory->Get("abseta_pt_DATA");
  muId_->cd("MC_NUM_LooseID_DEN_genTracks_PAR_pt_spliteta_bin1/efficienciesMC/");
  hist_muon[0][1] = (TH2F*)gDirectory->Get("abseta_pt_MC");
  muId_->cd("MC_NUM_LooseID_DEN_genTracks_PAR_pt_spliteta_bin1/");
  hist_muon[0][2] = (TH2F*)gDirectory->Get("abseta_pt_ratio");

  muId_->cd("MC_NUM_TightIDandIPCut_DEN_genTracks_PAR_pt_spliteta_bin1/efficienciesDATA/");
  hist_muon[1][0] = (TH2F*)gDirectory->Get("abseta_pt_DATA");
  muId_->cd("MC_NUM_TightIDandIPCut_DEN_genTracks_PAR_pt_spliteta_bin1/efficienciesMC/");
  hist_muon[1][1] = (TH2F*)gDirectory->Get("abseta_pt_MC");
  muId_->cd("MC_NUM_TightIDandIPCut_DEN_genTracks_PAR_pt_spliteta_bin1/");
  hist_muon[1][2] = (TH2F*)gDirectory->Get("abseta_pt_ratio");


  muIso_->cd("MC_NUM_LooseRelIso_DEN_LooseID_PAR_pt_spliteta_bin1/efficienciesDATA/");
  hist_muon[2][0] = (TH2F*)gDirectory->Get("abseta_pt_DATA");
  muIso_->cd("MC_NUM_LooseRelIso_DEN_LooseID_PAR_pt_spliteta_bin1/efficienciesMC/");
  hist_muon[2][1] = (TH2F*)gDirectory->Get("abseta_pt_MC");
  muIso_->cd("MC_NUM_LooseRelIso_DEN_LooseID_PAR_pt_spliteta_bin1");
  hist_muon[2][2] = (TH2F*)gDirectory->Get("abseta_pt_ratio");

  muIso_->cd("MC_NUM_TightRelIso_DEN_TightID_PAR_pt_spliteta_bin1/efficienciesDATA/");
  hist_muon[3][0] = (TH2F*)gDirectory->Get("abseta_pt_DATA");
  muIso_->cd("MC_NUM_TightRelIso_DEN_TightID_PAR_pt_spliteta_bin1/efficienciesMC/");
  hist_muon[3][1] = (TH2F*)gDirectory->Get("abseta_pt_MC");
  muIso_->cd("MC_NUM_TightRelIso_DEN_TightID_PAR_pt_spliteta_bin1/");
  hist_muon[3][2] = (TH2F*)gDirectory->Get("abseta_pt_ratio");

  const unsigned nEta = hist_muon[0][0]->GetXaxis()->GetNbins();

  double etaMin[nEta];
  double etaMax[nEta];

  for (unsigned ie(0);ie<nEta;++ie){
    etaMin[ie] = hist_muon[0][0]->GetXaxis()->GetBinLowEdge(ie+1);
    etaMax[ie] = hist_muon[0][0]->GetXaxis()->GetBinLowEdge(ie+2);
    std::cout << "eta min " << etaMin[ie] << " max " << etaMax[ie] << std::endl;
  }

  const unsigned nPt = hist_muon[0][0]->GetYaxis()->GetNbins();

  double ptMin[nPt];
  double ptMax[nPt];

  for (unsigned ie(0);ie<nPt;++ie){
    ptMin[ie] = hist_muon[0][0]->GetYaxis()->GetBinLowEdge(ie+1);
    ptMax[ie] = hist_muon[0][0]->GetYaxis()->GetBinLowEdge(ie+2);
    std::cout << "pt min " << ptMin[ie] << " max " << ptMax[ie] << std::endl;
  }

  const unsigned nP = 4;
  std::string prefix = "Fall15_76X_";
  std::string lFileName[nP] = {"mu_loose_id","mu_tight_id","mu_loose_iso","mu_tight_iso"};
  std::string lDataType[3] = {"data_eff","mc_eff","SF"};
  
  std::ostringstream lName;
  
  for (unsigned iWP(0);iWP<nP;++iWP){//loop on WP
    
    for (unsigned iData(0);iData<3;++iData){//loop on data type: data, MC, SF
      lName.str("");
      lName << prefix << lFileName[iWP] << "_" << lDataType[iData] << ".txt";
      std::ofstream lOut(lName.str().c_str());
      
      for (unsigned iEta(0); iEta<nEta; ++iEta){//loop on eta bin
	for (unsigned iPt(0); iPt<nPt; ++iPt){//loop on pT bins
	  double val = hist_muon[iWP][iData]->GetBinContent(iEta+1,iPt+1);
	  double err = hist_muon[iWP][iData]->GetBinError(iEta+1,iPt+1);
	  if (iWP<2) err = sqrt(pow(err,2)+pow(extraIdSyst,2));
	  else err = sqrt(pow(err,2)+pow(extraIsoSyst,2));
	  lOut << ptMin[iPt] << " " << ptMax[iPt] << " " << etaMin[iEta] << " " << etaMax[iEta] << " " << val << " " << err << " " << err << std::endl;
	}//loop on pT bins
	
      }//loop on eta bin
      
      lOut.close();
    }//loop on data type
    
  }//loop on WP
  
  
  return 1;

}//
