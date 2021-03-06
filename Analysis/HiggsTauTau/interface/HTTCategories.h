#ifndef ICHiggsTauTau_HiggsTauTau_HTTCategories_h
#define ICHiggsTauTau_HiggsTauTau_HTTCategories_h
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HTTPlots.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"

#include <string>

namespace ic {

class HTTCategories : public ModuleBase {

 private:
  CLASS_MEMBER(HTTCategories, std::string, ditau_label)
  CLASS_MEMBER(HTTCategories, std::string, met_label)
  CLASS_MEMBER(HTTCategories, std::string, jets_label)
  CLASS_MEMBER(HTTCategories, double, mass_shift)
  CLASS_MEMBER(HTTCategories, ic::channel, channel)
  CLASS_MEMBER(HTTCategories, ic::era, era)
  CLASS_MEMBER(HTTCategories, ic::strategy, strategy)
  CLASS_MEMBER(HTTCategories, bool, write_tree)
  CLASS_MEMBER(HTTCategories, bool, bjet_regression)
  CLASS_MEMBER(HTTCategories, bool, make_sync_ntuple)
  CLASS_MEMBER(HTTCategories, bool, is_embedded)
  CLASS_MEMBER(HTTCategories, bool, is_data)
  CLASS_MEMBER(HTTCategories, bool, systematic_shift)
  CLASS_MEMBER(HTTCategories, bool, add_Hhh_variables)
  CLASS_MEMBER(HTTCategories, bool, add_nlo_weights)
  CLASS_MEMBER(HTTCategories, std::string, sync_output_name)
  CLASS_MEMBER(HTTCategories, bool, iso_study)
  CLASS_MEMBER(HTTCategories, bool, tau_id_study)
  CLASS_MEMBER(HTTCategories, bool, optimisation_study)
  CLASS_MEMBER(HTTCategories, bool, qcd_study)
  CLASS_MEMBER(HTTCategories, bool, jetfake_study)
  CLASS_MEMBER(HTTCategories, int, kinfit_mode )
  CLASS_MEMBER(HTTCategories, fwlite::TFileService*, fs)
 
  TTree *outtree_;
  TTree *synctree_;
  TFile *lOFile;

  struct branch_var {
      double var_double;  
      float var_float;
      branch_var& operator=(double doub){
          var_double = doub;
          var_float = static_cast<float>(doub);
          return *this;
      }
      branch_var& operator*(double sf){
          var_double*=sf;
          var_float*=sf;
          return *this;
      }
  };

  // Event Properties
  branch_var wt_;
  int run_;
  unsigned long long event_;
  int lumi_;
  float rho_;
  float mc_weight_;
  float pu_weight_;
  double wt_btag_;
  double wt_ggh_pt_up_;
  double wt_ggh_pt_down_;
  double wt_tau_fake_up_;
  double wt_tau_fake_down_;
  double wt_tquark_up_;
  double wt_tquark_down_;
  double wt_zpt_up_;
  double wt_zpt_down_;
  double wt_tau_id_up_;
  double wt_tau_id_down_;
  double wt_em_qcd_;
  double wt_em_qcd_down_;
  double wt_em_qcd_up_;
  double wt_nlo_pt_;
  double nlo_pt_;
  float trigweight_1_;
  float trigweight_2_;
  double wt_trig_up_1_;
  double wt_trig_up_2_;
  double wt_trig_down_1_;
  double wt_trig_down_2_;
  float idisoweight_1_;
  float idisoweight_2_;
  float trackingweight_1_;
  float trackingweight_2_;
  float isoweight_1_;
  float isoweight_2_;
  float effweight_;
  float fakeweight_;
  float embeddedweight_;
  float signalweight_;
  bool os_;
  bool dilepton_veto_;
  bool extraelec_veto_;
  bool extramuon_veto_;
  bool minimal_extraelec_veto_;
  bool minimal_extramuon_veto_;
  bool lepton_veto_;
  unsigned n_vtx_;
  bool good_vtx_;
/*  branch_var trigger_object_pt_1;
  branch_var trigger_object_eta_1;
  branch_var trigger_object_pt_2;
  branch_var trigger_object_eta_2;
*/
 float n_pu_;
  branch_var m_sv_;
  branch_var mt_sv_;
  branch_var m_vis_;
  branch_var pt_h_;
  float eta_h_;
  float phi_h_;
  branch_var pt_tt_;
  branch_var mt_1_;
  branch_var mt_lep_;
  branch_var mt_tot_;
  branch_var pfmt_1_;
  branch_var puppimt_1_;
  float id_e_mva_nt_loose_1_ = 0;
  branch_var mt_2_;
  double mt_ll_;
  branch_var pzeta_;
  branch_var pfpzeta_;
  branch_var puppipzeta_;
  branch_var pzetavis_;
  branch_var pzetamiss_;
  branch_var pfpzetamiss_;
  branch_var puppipzetamiss_;
  double dphi_;
  double met_dphi_1_;
  double met_dphi_2_;
  double emu_csv_;
  double emu_dxy_1_;
  double emu_dxy_2_;
  branch_var d0_1_;
  branch_var d0_2_;
  branch_var dz_1_;
  branch_var dz_2_;
  branch_var pt_1_;
  branch_var pt_2_;
  branch_var eta_1_;
  branch_var eta_2_;
  branch_var phi_1_;
  branch_var phi_2_;
  double E_1_;
  double E_2_;
  int q_1_;
  int q_2_;
  int jet_flav_1_;
  int jet_flav_2_;
  bool tag_trigger_match_1_;
  bool tag_trigger_match_2_;
  bool probe_trigger_match_1_;
  bool probe_trigger_match_2_;
  branch_var iso_1_;
  branch_var iso_2_;
  double iso_1_db03_;
  double iso_1_db04_;
  double iso_1_puw03_;
  double iso_1_puw04_;
  double iso_2_puw03_;
  double iso_2_puw04_;
  double iso_1_trk03_;
  double iso_1_db03allch_;
  double iso_1_db04allch_; 
  double iso_1_ea03_;
  double iso_2_db03_;
  double iso_2_db04_;
  double iso_2_trk03_;
  double iso_2_db03allch_;
  double iso_2_db04allch_; 
  double iso_2_ea03_;
  bool antiele_1_;
  bool antimu_1_;
  bool antiele_2_;
  bool antimu_2_;
  double z_1_;
  double z_2_;
  float m_1_;
  branch_var m_2_;
  branch_var mva_1_;
  branch_var mva_2_;
  branch_var mvamet_;
  branch_var mvamet_phi_;
  branch_var pfmet_;
  float pfmet_phi_;
  branch_var puppimet_;
  float puppimet_phi_; 
  float mvametCov00_;
  float mvametCov01_;
  float mvametCov10_;
  float mvametCov11_;
  float pfmetCov00_;
  float pfmetCov01_;
  float pfmetCov10_;
  float pfmetCov11_;
  float gen_px_;
  float gen_py_;
  float vis_px_;
  float vis_py_;

  // Tau ID variables
  float l3Hits_2 = 0;
  float lagainstElectronMVA3raw_2 = 0;
  float lbyIsolationMVA2raw_2 = 0;
  float lagainstMuonLoose2_2 = 0;
  float lagainstMuonMedium2_2 = 0;
  float lagainstMuonTight2_2 = 0;
  float lbyIsolationMVA3newDMwoLTraw_2 = 0;
  float lbyIsolationMVA3oldDMwoLTraw_2 = 0;
  float lbyIsolationMVA3newDMwLTraw_2 = 0;
  float lbyIsolationMVA3oldDMwLTraw_2 = 0;
  float lchargedIsoPtSum_2 = 0;
  float lchargedRelIsoPtSum_2 = 0;
  float lneutralIsoPtSum_2 = 0;
  float lpuCorrPtSum_2 = 0;
  branch_var lPhotonPtSum_2;
  bool ldecayModeFindingOldDMs_2 = 0;

  float l3Hits_1 = 0;
  float lagainstElectronLooseMVA_1 = 0;
  float lagainstElectronMediumMVA_1 = 0;
  float lagainstElectronTightMVA_1 = 0;
  float lagainstElectronVTightMVA_1 = 0;
  float lagainstElectronVLooseMVA_1 = 0;
  float lagainstMuonLoose3_1 = 0;
  float lagainstMuonTight3_1 = 0;
  float lbyIsolationMVA3newDMwoLTraw_1 = 0;
  float lbyIsolationMVA3oldDMwoLTraw_1 = 0;
  float lbyIsolationMVA3newDMwLTraw_1 = 0;
  float lbyIsolationMVA3oldDMwLTraw_1 = 0;
  float lchargedIsoPtSum_1 = 0;
  float lchargedRelIsoPtSum_1 = 0;
  float lneutralIsoPtSum_1 = 0;
  float lpuCorrPtSum_1 = 0;
  branch_var lPhotonPtSum_1;
  bool ldecayModeFindingOldDMs_1 = 0;
  bool lbyVLooseIsolationMVA3newDMwLT_1=0;
  bool lbyVLooseIsolationMVA3newDMwLT_2=0;
  bool lbyLooseIsolationMVA3newDMwLT_1=0;
  bool lbyLooseIsolationMVA3newDMwLT_2=0;
  bool lbyMediumIsolationMVA3newDMwLT_1=0;
  bool lbyMediumIsolationMVA3newDMwLT_2=0;
  bool lbyVTightIsolationMVA3newDMwLT_1=0;
  bool lbyVTightIsolationMVA3newDMwLT_2=0;
  bool lbyTightIsolationMVA3newDMwLT_1=0;
  bool lbyTightIsolationMVA3newDMwLT_2=0;
  bool lbyLooseCombinedIsolation_1=0;
  bool lbyLooseCombinedIsolation_2=0;
  bool lbyMediumCombinedIsolation_1=0;
  bool lbyMediumCombinedIsolation_2=0;
  bool lbyTightCombinedIsolation_1=0;
  bool lbyTightCombinedIsolation_2=0;
  bool lbyLoosePileupWeightedIsolation_1=0;
  bool lbyLoosePileupWeightedIsolation_2=0;
  bool lbyMediumPileupWeightedIsolation_1=0;
  bool lbyMediumPileupWeightedIsolation_2=0;
  bool lbyTightPileupWeightedIsolation_1=0;
  bool lbyTightPileupWeightedIsolation_2=0;
  branch_var lbyIsolationMVArun2DBnewDMwLTraw_2;
  branch_var lbyIsolationMVArun2DBoldDMwLTraw_2;
  branch_var lbyIsolationMVArun2PWnewDMwLTraw_2;
  branch_var lbyIsolationMVArun2PWoldDMwLTraw_2;
  bool lbyVVTightIsolationMVArun2PWoldDMwLT_2=0;
  bool lbyVVTightIsolationMVArun2PWnewDMwLT_2=0;
  bool lbyVVTightIsolationMVArun2DBoldDMwLT_2=0;
  bool lbyVVTightIsolationMVArun2DBnewDMwLT_2=0;
  bool lbyVTightIsolationMVArun2PWoldDMwLT_2=0;
  bool lbyVTightIsolationMVArun2PWnewDMwLT_2=0;
  bool lbyVTightIsolationMVArun2DBoldDMwLT_2=0;
  bool lbyVTightIsolationMVArun2DBnewDMwLT_2=0;
  bool lbyTightIsolationMVArun2PWoldDMwLT_2=0;
  bool lbyTightIsolationMVArun2PWnewDMwLT_2=0;
  bool lbyTightIsolationMVArun2DBoldDMwLT_2=0;
  bool lbyTightIsolationMVArun2DBnewDMwLT_2=0;
  bool lbyMediumIsolationMVArun2PWoldDMwLT_2=0;
  bool lbyMediumIsolationMVArun2PWnewDMwLT_2=0;
  bool lbyMediumIsolationMVArun2DBoldDMwLT_2=0;
  bool lbyMediumIsolationMVArun2DBnewDMwLT_2=0;
  bool lbyLooseIsolationMVArun2PWoldDMwLT_2=0;
  bool lbyLooseIsolationMVArun2PWnewDMwLT_2=0;
  bool lbyLooseIsolationMVArun2DBoldDMwLT_2=0;
  bool lbyLooseIsolationMVArun2DBnewDMwLT_2=0;
  bool lbyVLooseIsolationMVArun2PWoldDMwLT_2=0;
  bool lbyVLooseIsolationMVArun2PWnewDMwLT_2=0;
  bool lbyVLooseIsolationMVArun2DBoldDMwLT_2=0;
  bool lbyVLooseIsolationMVArun2DBnewDMwLT_2=0;
  branch_var lbyIsolationMVArun2DBnewDMwLTraw_1;
  branch_var lbyIsolationMVArun2DBoldDMwLTraw_1;
  branch_var lbyIsolationMVArun2PWnewDMwLTraw_1;
  branch_var lbyIsolationMVArun2PWoldDMwLTraw_1;
  bool lbyVVTightIsolationMVArun2PWoldDMwLT_1=0;
  bool lbyVVTightIsolationMVArun2PWnewDMwLT_1=0;
  bool lbyVVTightIsolationMVArun2DBoldDMwLT_1=0;
  bool lbyVVTightIsolationMVArun2DBnewDMwLT_1=0;
  bool lbyVTightIsolationMVArun2PWoldDMwLT_1=0;
  bool lbyVTightIsolationMVArun2PWnewDMwLT_1=0;
  bool lbyVTightIsolationMVArun2DBoldDMwLT_1=0;
  bool lbyVTightIsolationMVArun2DBnewDMwLT_1=0;
  bool lbyTightIsolationMVArun2PWoldDMwLT_1=0;
  bool lbyTightIsolationMVArun2PWnewDMwLT_1=0;
  bool lbyTightIsolationMVArun2DBoldDMwLT_1=0;
  bool lbyTightIsolationMVArun2DBnewDMwLT_1=0;
  bool lbyMediumIsolationMVArun2PWoldDMwLT_1=0;
  bool lbyMediumIsolationMVArun2PWnewDMwLT_1=0;
  bool lbyMediumIsolationMVArun2DBoldDMwLT_1=0;
  bool lbyMediumIsolationMVArun2DBnewDMwLT_1=0;
  bool lbyLooseIsolationMVArun2PWoldDMwLT_1=0;
  bool lbyLooseIsolationMVArun2PWnewDMwLT_1=0;
  bool lbyLooseIsolationMVArun2DBoldDMwLT_1=0;
  bool lbyLooseIsolationMVArun2DBnewDMwLT_1=0;
  bool lbyVLooseIsolationMVArun2PWoldDMwLT_1=0;
  bool lbyVLooseIsolationMVArun2PWnewDMwLT_1=0;
  bool lbyVLooseIsolationMVArun2DBoldDMwLT_1=0;
  bool lbyVLooseIsolationMVArun2DBnewDMwLT_1=0;
  float lagainstElectronLooseMVA_2 = 0;
  float lagainstElectronMediumMVA_2 = 0;
  float lagainstElectronTightMVA_2 = 0;
  float lagainstElectronVTightMVA_2 = 0;
  float lagainstElectronVLooseMVA_2 = 0;
  float lagainstMuonLoose3_2 = 0;
  float lagainstMuonTight3_2 = 0;
    
  int tau_decay_mode_1_;
  int tau_decay_mode_2_;

  unsigned n_jets_;
  unsigned n_lowpt_jets_;
  unsigned n_bjets_;
  unsigned n_loose_bjets_;
  unsigned n_jetsingap_; // Defined if n_jets >= 2
  unsigned n_jetsingap20_; // Defined if n_jets >= 2
  branch_var jpt_1_;     // Defined if n_jets >= 1
  branch_var jpt_2_;     // Defined if n_jets >= 2
  branch_var jeta_1_;    // Defined if n_jets >= 1
  branch_var jeta_2_;    // Defined if n_jets >= 2
  float jphi_1_;    // Defined if n_jets >= 1
  float jphi_2_;    // Defined if n_jets >= 2
  float jptraw_1_; 
  float jrawf_1_;
  float jptraw_2_; 
  float jrawf_2_;
  float jptunc_1_; 
  float jptunc_2_; 
  float jmva_1_; 
  float jmva_2_; 
  float jlrm_1_; 
  float jlrm_2_; 
  float jctm_1_; 
  float jctm_2_;
  float nearjpt_1_;
  branch_var bpt_1_;     // Defined if n_bjets >= 1
  branch_var beta_1_;    // Defined if n_bjets >= 1
  float bphi_1_;    // Defined if n_bjets >= 1
  float bmva_1_;
  float brawf_1_;
  branch_var bcsv_1_; 
  branch_var bpt_2_;     // Defined if n_bjets >= 2
  branch_var beta_2_;    // Defined if n_bjets >= 2
  float bphi_2_;    // Defined if n_bjets >= 2
  float bmva_2_;
  float bcsv_2_; 
  float brawf_2_;
  double jet_csvpt_1_;     // Defined if n_jets >= 1
  double jet_csvEt_1_;     // Defined if n_jets >= 1
  double jet_csvpt_2_;     // Defined if n_jets >= 2
  double jet_csvpt_bb_;
  double jet_csv_dR_;
  double jet_csveta_1_;    // Defined if n_jets >= 1
  double jet_csveta_2_;    // Defined if n_jets >= 2
  double jet_csvbcsv_1_; 
  double jet_csvbcsv_2_; 

  int j1_dm_;

  branch_var mjj_;       // Defined if n_jets >= 2
  double mjj_h_;       // Defined if n_jets >= 2
  double mbb_h_;       // Defined if n_jets >= 2
  double mjj_tt_;       // Defined if n_jets >= 2
  double m_H_best_;       // Defined if n_jets >= 2
  double m_H_chi2_best_;       // Defined if n_jets >= 2
  double pull_balance_H_best_; //Defined if n_jets >= 2
  int convergence_H_best_; //Defined if n_jets >= 2
  double m_H_hZ_;     //Defined if n_jets >= 2
  double m_H_hZ_chi2_;  //Defined if n_jets >= 2
  double pull_balance_hZ_; //Defined if n_jets >= 2
  int convergence_hZ_; //Defined if n_jets >= 2
  double m_H_Zh_;      //Defined if n_jets >= 2
  double m_H_Zh_chi2_;   //Defined if n_jets >= 2 
  double pull_balance_Zh_; //Defined if n_jets >= 2
  int convergence_Zh_; //Defined if n_jets >= 2
  double m_H_hh_;	     //Defined if n_jets >= 2
  double m_H_hh_all_;	     //Defined if n_jets >= 2
  double m_H_hh_chi2_;   //Defined if n_jets >= 2
  double pull_balance_hh_; //Defined if n_jets >= 2
  int convergence_hh_; //Defined if n_jets >= 2
  double m_bb_;	     //Defined if n_jets >= 2
  double m_bb_chi2_;   //Defined if n_jets >= 2
  double pull_balance_bb_; //Defined if n_jets >= 2
  int convergence_bb_; //Defined if n_jets >= 2
  branch_var jdeta_;     // Defined if n_jets >= 2
  float jdphi_;
  unsigned gen_match_1_;
  unsigned gen_match_2_;
  double gen_match_1_pt_;
  double gen_match_2_pt_;
  /*double leading_lepton_match_pt_;
  double subleading_lepton_match_pt_;
  double leading_lepton_match_DR_;
  double subleading_lepton_match_DR_;*/

  double jet_csv_mjj_;       // Defined if n_jets >= 2
  double jet_csv_deta_;     // Defined if n_jets >= 2
  double jet_csv_dphi_;     // Defined if n_jets >= 2
  double jet_csv_dtheta_;     // Defined if n_jets >= 2
  double mjj_lowpt_;       // Defined if n_lowpt_jets >= 2
  double jdeta_lowpt_;     // Defined if n_lowpt_jets >= 2
  unsigned n_jetsingap_lowpt_; // Defined if n_lowpt_jets >= 2
  float lbyMediumIsolationMVArun2DBoldDMwLT_1_;
  float lbyMediumIsolationMVArun2DBnewDMwLT_1_;
  float lbyTightIsolationMVArun2DBoldDMwLT_1_;
  float lbyTightIsolationMVArun2DBnewDMwLT_1_;
  float lbyMediumIsolationMVArun2DBoldDMwLT_2_;
  float lbyMediumIsolationMVArun2DBnewDMwLT_2_;
  float lbyTightIsolationMVArun2DBoldDMwLT_2_;
  float lbyTightIsolationMVArun2DBnewDMwLT_2_;

  unsigned n_prebjets_;
  unsigned n_jets_csv_;
  unsigned n_bjets_csv_;

  double em_gf_mva_;
  double em_vbf_mva_;


 public:
  HTTCategories(std::string const& name);
  virtual ~HTTCategories();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();






};

}


#endif
