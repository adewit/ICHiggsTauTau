#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTPrint.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/interface/Tau.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include "TMVA/Reader.h"
#include "TVector3.h"


namespace ic {

  HTTPrint::HTTPrint(std::string const& name) : ModuleBase(name) {

  }

  HTTPrint::~HTTPrint() {
    ;
  }

  int HTTPrint::PreAnalysis() {
    return 0;
  }

  void HTTPrint::PrintEvent(unsigned evt) {
    events_.insert(evt);
  }

  int HTTPrint::Execute(TreeEvent *event) {

    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    std::vector<Tau*> taus = event->GetPtrVec<Tau>("taus");
    //ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 0.0, 5.0));
    //std::vector<Vertex*> const& vertices = event->GetPtrVec<Vertex>("vertices");

    // static double min_mass = 100.0;
    // std::vector<GenParticle*> parts = event->GetPtrVec<GenParticle>("genParticles");
    // for (unsigned i = 0; i < parts.size(); ++i) {
    //   if (parts[i]->pdgid() == 23) {
    //     if (parts[i]->vector().M() < min_mass) {
    //       min_mass = parts[i]->vector().M();
    //       std::cout << min_mass << std::endl;
    //     }
    //   }
    // }

    if (events_.find(eventInfo->event()) != events_.end()) {
      std::vector<Muon*> const& muons = event->GetPtrVec<Muon>("muonsPFlow");
      std::cout << "-----------------------------------------" << std::endl;
    std::cout << "event: " <<  eventInfo->event() << " lumi: " << eventInfo->lumi_block() << " run: " << eventInfo->run() << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
    std::cout << "nGoodVertices: " << eventInfo->good_vertices() << std::endl;
    std::cout << "Jet Rho: " << eventInfo->jet_rho() << std::endl;
    

    for (unsigned i = 0; i < taus.size(); ++i) {
      std::cout << "Tau " << i << std::endl;
      taus[i]->Print();
      std::cout << "-decay_mode: " << taus[i]->decay_mode() << std::endl;
      std::cout << "-lead_track_dz_with_vertex: " << taus[i]->lead_dz_vertex() << std::endl;
      std::cout << "-decayModeFinding: " << taus[i]->GetTauID("decayModeFinding") << std::endl;
      std::cout << "-byIsolationMVAraw: " << taus[i]->GetTauID("byIsolationMVAraw") << std::endl;
      std::cout << "-byLooseIsolationMVA: " << taus[i]->GetTauID("byLooseIsolationMVA") << std::endl;
      std::cout << "-byLooseIsolationMVA2: " << taus[i]->GetTauID("byLooseIsolationMVA2") << std::endl;
      std::cout << "-againstElectronLoose: " << taus[i]->GetTauID("againstElectronLoose") << std::endl;
      std::cout << "-againstElectronMVA: " << taus[i]->GetTauID("againstElectronMVA") << std::endl;
      std::cout << "-againstElectronTightMVA2: " << taus[i]->GetTauID("againstElectronTightMVA2") << std::endl;
      std::cout << "-againstElectronTightMVA3: " << taus[i]->GetTauID("againstElectronTightMVA3") << std::endl;
      std::cout << "-againstMuonTight: " << taus[i]->GetTauID("againstMuonTight") << std::endl;
      std::cout << "-againstMuonTight2: " << taus[i]->GetTauID("againstMuonTight2") << std::endl;
      std::cout << "-againstMuonLoose: " << taus[i]->GetTauID("againstMuonLoose") << std::endl;
    }
    
    for (unsigned i = 0; i < muons.size(); ++i) {
      std::cout << "Muon " << i << std::endl;
      muons[i]->Print();
      std::cout << "-dxyVertex: " << muons[i]->dxy_vertex() << std::endl;
      std::cout << "-dzVertex: " << muons[i]->dz_vertex() << std::endl;
      std::cout << "-isGlobalMuon: " << muons[i]->is_global() << std::endl;
      std::cout << "-isTrackerMuon: " << muons[i]->is_tracker() << std::endl;
      std::cout << "-numberOfValidPixelHits: " << muons[i]->it_pixel_hits() << std::endl;
      std::cout << "-numberOfValidMuonHits: " << muons[i]->gt_valid_muon_hits() << std::endl;
      std::cout << "-trackerLayersWithMeasurement: " << muons[i]->it_layers_with_measurement() << std::endl;
      std::cout << "-normalizedChi2: " << muons[i]->gt_normalized_chi2() << std::endl;
      std::cout << "-numberOfMatchedStations: " << muons[i]->matched_stations() << std::endl;
      std::cout << "-dr03_tk_sum_pt: " << muons[i]->dr03_tk_sum_pt() << std::endl;
      std::cout << "-dr03_ecal_rechit_sum_et: " << muons[i]->dr03_ecal_rechit_sum_et() << std::endl;
      std::cout << "-dr03_hcal_tower_sum_et: " << muons[i]->dr03_hcal_tower_sum_et() << std::endl;
      std::cout << "-dr04_pfiso_charged_all " << muons[i]->dr04_pfiso_charged_all() << std::endl;
      std::cout << "-dr04_pfiso_charged " << muons[i]->dr04_pfiso_charged() << std::endl;
      std::cout << "-dr04_pfiso_neutral " << muons[i]->dr04_pfiso_neutral() << std::endl;
      std::cout << "-dr04_pfiso_gamma " << muons[i]->dr04_pfiso_gamma() << std::endl;
      std::cout << "-dr04_pfiso_pu " << muons[i]->dr04_pfiso_pu() << std::endl;

      double iso =  muons[i]->dr04_pfiso_charged_all() 
      + std::max(muons[i]->dr04_pfiso_neutral() + muons[i]->dr04_pfiso_gamma() - 0.5 * muons[i]->dr04_pfiso_pu(), 0.0);
      std::cout << "-dr04_pfiso_dbeta_sum: " << iso << std::endl;
      iso = iso / muons[i]->pt();
      std::cout << "-dr04_pfiso_dbeta_rel: " << iso << std::endl;
    }



    std::vector<Electron*> const& elecs = event->GetPtrVec<Electron>("electrons");

    for (unsigned i = 0; i < elecs.size(); ++i) {
      std::cout << "Elec " << i <<  std::endl;
      elecs[i]->Print();
      double iso =  (elecs[i]->dr04_pfiso_charged_all() 
      + std::max(elecs[i]->dr04_pfiso_neutral() + elecs[i]->dr04_pfiso_gamma() - 0.5 * elecs[i]->dr04_pfiso_pu(), 0.0)) / elecs[i]->pt();
      std::cout << "-rel_iso_04_dbeta: " << iso << std::endl;
      std::cout << "-id_mvaNonTrigV0: " << elecs[i]->GetIdIso("mvaNonTrigV0") << std::endl;
      std::cout << "-id_mvaTrigV0: " << elecs[i]->GetIdIso("mvaTrigV0") << std::endl;
      std::cout << "-dr03_tk_sum_pt: " << elecs[i]->dr03_tk_sum_pt() << std::endl;
      std::cout << "-dr03_ecal_rechit_sum_et: " << elecs[i]->dr03_ecal_rechit_sum_et() << std::endl;
      std::cout << "-dr03_hcal_tower_sum_et: " << elecs[i]->dr03_hcal_tower_sum_et() << std::endl;
    }

    for (unsigned i = 0; i < elecs.size(); ++i) {
      for (unsigned j = 0; j < muons.size(); ++j) {
        std::cout << "Elec " << i << ", Muon " << j << " DR: " <<
          ROOT::Math::VectorUtil::DeltaR(elecs[i]->vector(), muons[j]->vector()) << std::endl;
      }
    }
    for (unsigned i = 0; i < muons.size(); ++i) {
      for (unsigned j = 0; j < taus.size(); ++j) {
       std::cout << "Muon " << i << ", Tau " << j << " DR: " <<
          ROOT::Math::VectorUtil::DeltaR(muons[i]->vector(), taus[j]->vector()) << std::endl;
      }
    }
    for (unsigned i = 0; i < elecs.size(); ++i) {
      for (unsigned j = 0; j < taus.size(); ++j) {
       std::cout << "Elec " << i << ", Tau " << j << " DR: " <<
          ROOT::Math::VectorUtil::DeltaR(elecs[i]->vector(), taus[j]->vector()) << std::endl;
      }
    }
    std::map<std::size_t, Met *> const& met_map = event->GetIDMap<Met>("pfMVAMetVector");
    for (auto met_pair : met_map) {
      std::cout << "MVA MET: " << met_pair.second->pt() << std::endl;
    }

    std::vector<PFJet*> const& jets = event->GetPtrVec<PFJet>("pfJetsPFlow");
    std::vector<PFJet*> matched_jets;
    std::vector< std::pair<PFJet*, GenJet*> > matches;
    if (!eventInfo->is_data()) {
      std::vector<GenJet*> const& gen_jets = event->GetPtrVec<GenJet>("genJets");
      matches = MatchByDR(jets, gen_jets, 0.5, true, true);
      matched_jets = ExtractFirst(matches);
    }
    for (unsigned i = 0; i < jets.size(); ++i) {
      std::cout << "Jet " << i << ": " << jets[i]->vector() << std::endl;
      std::cout << "-pileup id mva: " << jets[i]->pu_id_mva_value()  << std::endl;
      std::cout << "-pileup id loose (2012): " << PUJetID(jets[i], true)  << std::endl;
      std::cout << "-pileup id loose (2011): " << PUJetID(jets[i], false)  << std::endl;
      std::cout << "-PF ID: " << PFJetID(jets[i]) << std::endl;
      std::cout << "-beta: " << jets[i]->beta() << std::endl;
      std::cout << "-beta_max: " << jets[i]->beta_max() << std::endl;
      double charged_frac = (jets[i]->charged_em_energy() + jets[i]->charged_had_energy()) / jets[i]->uncorrected_energy();
      std::cout << "-charged_frac: " << charged_frac << std::endl;
      std::cout << "-Uncorrected: " << jets[i]->GetJecFactor("Uncorrected") << std::endl;
      std::cout << "-L1FastJet: " << jets[i]->GetJecFactor("L1FastJet") << std::endl;
      std::cout << "-L2Relative: " << jets[i]->GetJecFactor("L2Relative") << std::endl;
      std::cout << "-L3Absolute: " << jets[i]->GetJecFactor("L3Absolute") << std::endl;
      std::cout << "-L2L3Residual: " << jets[i]->GetJecFactor("L2L3Residual") << std::endl;
      std::cout << "-CSV: " << jets[i]->GetBDiscriminator("combinedSecondaryVertexBJetTags") << std::endl;

      std::vector<PFJet*>::const_iterator it = std::find(matched_jets.begin(),matched_jets.end(), jets[i]);
      if (it != matched_jets.end()) {
        GenJet const* matched_genjet = NULL;
        for (unsigned j = 0; j < matches.size(); ++j) {
          if (matches[j].first == (*it)) matched_genjet = matches[j].second;
        }
        std::cout << "-has matched genjet: " << matched_genjet->vector() << std::endl;
      }
    }
    /*
    std::cout << "---Triggers" << std::endl;
    std::vector<std::string> paths = { "triggerObjectsIsoMu17LooseTau20", "triggerObjectsEle22WP90RhoLooseTau20" };
    for (auto path : paths) {
      if (!eventInfo->is_data()) {
        std::cout << "--" << path << std::endl;
        auto objs = event->GetPtrVec<TriggerObject>(path);
        for (auto cand : objs) {
          cand->Print();
          for (auto label : cand->filters()) {
            std::cout << "-" << label << std::endl;
          }
        }
      }
    }*/


  }
  return 0;

   
  }
  int HTTPrint::PostAnalysis() {
    return 0;
  }

  void HTTPrint::PrintInfo() {
    ;
  }
}