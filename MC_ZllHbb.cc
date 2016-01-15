// -*- C++ -*-
#include "MC_VHbb.h"

namespace Rivet {

  using namespace Cuts;

  class MC_ZllHbb : public Analysis {

  private:

    
  public:

    /// Constructor
    MC_ZllHbb()
    : Analysis("MC_ZllHbb")
    {    
    }

    //////////////////////////////////////////////////
    /// Book histograms and initialise projections before the run
    //////////////////////////////////////////////////
    void init() {

      // initialise projections
      FinalState fs;
      addProjection(fs, "fs");
      
      ////////////////////////////////////////////////////////////////////////////////////////////
      ///////////// START OF CHANNEL DEPENDENT PART
      ////////////////////////////////////////////////////////////////////////////////////////////
      
      // find Zee
      ZFinder zeefinder(fs, cut, PID::ELECTRON, cut_Zll_mass_min*GeV, cut_Zll_mass_max*GeV, 0.2);
      addProjection(zeefinder, "zeefinder");
      // find Zmm
      ZFinder zmmfinder(fs, cut, PID::MUON, cut_Zll_mass_min*GeV, cut_Zll_mass_max*GeV, 0.2);
      addProjection(zmmfinder, "zmmfinder");
      // // find Wenu
      // WFinder wenufinder(fs, cut, PID::ELECTRON, 0*GeV, 1e10*GeV, 0.2);
      // addProjection(wenufinder, "wenufinder");
      // // find Wmunu
      // WFinder wmunufinder(fs, cut, PID::MUON, 0*GeV, 1e10*GeV, 0.2);
      // addProjection(wmunufinder, "wmunufinder");
      // // find Znuenue
      // ZFinder znuenuefinder(fs, cut, PID::NU_E, 0*GeV, 1e10*GeV, 0.2);
      // addProjection(znuenuefinder, "znuenuefinder");
      // // find Znumunumu
      // ZFinder znumunumufinder(fs, cut, PID::NU_MU, 0*GeV, 1e10*GeV, 0.2);
      // addProjection(znumunumufinder, "znumunumufinder");
      // // find Znutaunutau
      // ZFinder znutaunutaufinder(fs, cut, PID::NU_TAU, 0*GeV, 1e10*GeV, 0.2);
      // addProjection(znutaunutaufinder, "znutaunutaufinder");
      // fish out Zee, Zmm, Znunu
      VetoedFinalState jetproinput;
      jetproinput.addVetoOnThisFinalState(zeefinder);
      jetproinput.addVetoOnThisFinalState(zmmfinder);
      // jetproinput.addVetoOnThisFinalState(znuenuefinder);
      // jetproinput.addVetoOnThisFinalState(znumunumufinder);
      // jetproinput.addVetoOnThisFinalState(znutaunutaufinder);
      
      ////////////////////////////////////////////////////////////////////////////////////////////
      ///////////// END OF CHANNEL DEPENDENT PART
      ////////////////////////////////////////////////////////////////////////////////////////////
      
      ////////////////////////////////////////////////////////
      // Unstable Higgs
      ////////////////////////////////////////////////////////
      UnstableFinalState undec_higgs(PID::HIGGS);
      addProjection(undec_higgs, "UFS");
      IdentifiedFinalState higgses(PID::HIGGS);
      jetproinput.addVetoOnThisFinalState(higgses);
      addProjection(higgses, "Higgses");
      VetoedFinalState rest(fs);
      rest.addVetoOnThisFinalState(higgses);
      addProjection(rest, "Rest");

      // make jets
      FastJets jetpro(jetproinput, FastJets::ANTIKT, 0.5);
      addProjection(jetpro, "jetpro");
      // jets for differential jet rates
      FastJets jetpro_kt(jetproinput, FastJets::KT, 0.7);
      addProjection(jetpro_kt, "jetpro_kt");

      ////////////////////////////////////////////////////////
      // MET
      ////////////////////////////////////////////////////////
      MissingMomentum met(fs);
      addProjection(met, "MET");

      initialize_Histos();
    }
    
    ////////////////////////////////////////////////////////
    // ---- HISTOS ----
    ////////////////////////////////////////////////////////
    void initialize_Histos(){

      std::vector<Histo1DPtr> dummy1;
      
      // CONTROL PLOTS ONLY IN INCLUSIVE CASE
      // all jets used for dij
      for (size_t i=0; i < njets; ++i) {
        _h_VptInc_AjetInc__log10_d.push_back(bookHisto1D("xcheck_VptInc_AjetInc__log10_d_" +to_str(i) + to_str(i+1), 100, 0.2, 4));
        _h_VptInc_AjetInc__ajets_pT.push_back(bookHisto1D("xcheck_VptInc_AjetInc__aJet_pT" +to_str(i),logspace(hist_nbins,0.1,500)));
        _h_VptInc_AjetInc__ajets_eta.push_back(bookHisto1D("xcheck_VptInc_AjetInc__aJet_eta" +to_str(i),hist_nbins,-5,5));
      }
      // selected jets
      for (size_t i=0; i < 1; ++i) {
        _h_VptInc_AjetInc__Sel_ajets_pT.push_back(bookHisto1D("xcheck_VptInc_AjetInc__Sel_aJet_pT" +to_str(i),hist_nbins,0,500));
        _h_VptInc_AjetInc__Sel_ajets_eta.push_back(bookHisto1D("xcheck_VptInc_AjetInc__Sel_aJet_eta" +to_str(i),hist_nbins,-5,5));
      }
      
      // CONTROL PLOTS ONLY IN INCLUSIVE CASE (didn't change the histo array structure in case some histos need to be "promoted")
      for (size_t ivpt=0; ivpt < 1; ivpt++){
        for (size_t iajet=0; iajet < 1; iajet++){
        
        _h__weights.push_back(dummy1);
        _h__weights[ivpt].push_back(bookHisto1D("xcheck_Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__ev_weights",3,-1.5,1.5));
        // Z properties
        _h__V_mass.push_back(dummy1);
        _h__V_mass[ivpt].push_back(bookHisto1D("xcheck_Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__V_mass",hist_nbins,60,120));
        _h__V_rap.push_back(dummy1);
        _h__V_rap[ivpt].push_back(bookHisto1D("xcheck_Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__V_rap",hist_nbins,hist_H_rap_min,hist_H_rap_max));
        _h__V_pT.push_back(dummy1);
        // ------> V_pT is NOT a crosscheck plot!!!
        _h__V_pT[ivpt].push_back(bookHisto1D("Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__V_pT",logspace(hist_nbins,hist_V_pt_min==0?0.1:hist_V_pt_min,hist_V_pt_max)));
        // higgs_candidate properties
        _h__higgs_candidate_mass.push_back(dummy1);
        _h__higgs_candidate_mass[ivpt].push_back(bookHisto1D("xcheck_Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__higgs_candidate_mass",hist_nbins,60,140));
        // H+Z properties
        _h__HV_rap.push_back(dummy1);
        _h__HV_rap[ivpt].push_back(bookHisto1D("xcheck_Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__HV_rap",hist_nbins,hist_H_rap_min,hist_H_rap_max));
        _h__HV_pT.push_back(dummy1);
        _h__HV_pT[ivpt].push_back(bookHisto1D("xcheck_Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__HV_pT",logspace(hist_nbins,1,hist_V_pt_max)));
        // higgs jets
        std::vector<std::vector<Histo1DPtr> > dummy2;
        _h__dau_eta.push_back(dummy2);
        _h__dau_pT.push_back(dummy2);
        
        _h__dau_eta[ivpt].push_back(dummy1);
        _h__dau_pT[ivpt].push_back(dummy1);
        for (size_t i=0; i < 2; ++i) {
          _h__dau_eta[ivpt][iajet].push_back(bookHisto1D("xcheck_Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__h_dau_eta" + to_str(i),hist_nbins,hist_H_rap_min,hist_H_rap_max));
          _h__dau_pT[ivpt][iajet].push_back(bookHisto1D("xcheck_Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__h_dau_pT" + to_str(i),logspace(hist_nbins,1,hist_V_pt_max)));
        }

        // angles
        _h__dR_jj.push_back(dummy1);
        _h__dR_jj[ivpt].push_back(bookHisto1D("xcheck_Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__dR_jj",hist_nbins,0,5));
        _h__dphi_jj.push_back(dummy1);
        _h__dphi_jj[ivpt].push_back(bookHisto1D("xcheck_Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__dphi_jj",hist_nbins,0,3.2));
        _h__deta_jj.push_back(dummy1);
        _h__deta_jj[ivpt].push_back(bookHisto1D("xcheck_Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__deta_jj", hist_nbins,0,5));

        _h__dR_HV.push_back(dummy1);
        _h__dR_HV[ivpt].push_back(bookHisto1D("xcheck_Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__dR_HV",hist_nbins,0,5));
        _h__dphi_HV.push_back(dummy1);
        _h__dphi_HV[ivpt].push_back(bookHisto1D("xcheck_Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__dphi_HV",hist_nbins,0,3.2));
        _h__deta_HV.push_back(dummy1);
        _h__deta_HV[ivpt].push_back(bookHisto1D("xcheck_Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__deta_HV", hist_nbins,0,5));
        
        // nJets
        _h__Sel_najets.push_back(dummy1);
        _h__Sel_najets[ivpt].push_back(bookHisto1D("xcheck_Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__Sel_najets",11,-0.5,10.5));
        _h__Sel_nbjets.push_back(dummy1);
        _h__Sel_nbjets[ivpt].push_back(bookHisto1D("xcheck_Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__Sel_nbjets",11,-0.5,10.5));
      }
    }
      
      // 0 < pT_H < 500 (a)
      // -5 < y_H < 5 (b)
      // 0 < pT_lepton < 500 (c)
      // 0 < pT_nu/nubar < 500 (d)
      // -5 < y_lepton < 5 (e)

      for (size_t ivpt=0; ivpt < Vpt_bins; ivpt++){
        for (size_t iajet=0; iajet < NJet_bins; iajet++){
          
          // higgs_candidate properties
          _h__higgs_candidate_rap.push_back(dummy1);
          _h__higgs_candidate_rap[ivpt].push_back(bookHisto1D("Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__higgs_candidate_rap",hist_nbins,hist_H_rap_min,hist_H_rap_max));
          _h__higgs_candidate_pT.push_back(dummy1);
          _h__higgs_candidate_pT[ivpt].push_back(bookHisto1D("Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__higgs_candidate_pT",logspace(hist_nbins,hist_H_pt_min==0?0.1:hist_H_pt_min,hist_H_pt_max)));
          // V lepton properties
          _h__leadlep_pT.push_back(dummy1);
          _h__leadlep_pT[ivpt].push_back(bookHisto1D("Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__leadlep_pT",logspace(hist_nbins,5,300)));
          _h__leadlep_eta.push_back(dummy1);
          _h__leadlep_eta[ivpt].push_back(bookHisto1D("Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__leadlep_eta",hist_nbins,hist_H_rap_min,hist_H_rap_max));
          _h__subleadlep_pT.push_back(dummy1);
          _h__subleadlep_pT[ivpt].push_back(bookHisto1D("Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__subleadlep_pT",logspace(hist_nbins,1,hist_V_pt_max)));
          _h__subleadlep_eta.push_back(dummy1);
          _h__subleadlep_eta[ivpt].push_back(bookHisto1D("Vpt"+ptbin_labels[ivpt]+"_Ajet"+njetbin_labels[iajet]+"__subleadlep_eta",hist_nbins,hist_H_rap_min,hist_H_rap_max));

        }
      }

    }

    //////////////////////////////////////////////////
    /// Perform the per-event analysis
    //////////////////////////////////////////////////
    void analyze(const Event& event) {
      const double weight = event.weight();

      // ////////////////////////////////////////////////////////
      // // MET
      // ////////////////////////////////////////////////////////
      // const MissingMomentum& MET_tmp = applyProjection<MissingMomentum>(event, "MET");
      // MET_4v = -MET_tmp.visibleMomentum();
      // MET = MET_4v.pT();
      // const WFinder& wenufinder = applyProjection<WFinder>(event, "wenufinder");
      // const WFinder& wmunufinder = applyProjection<WFinder>(event, "wmunufinder");
      // const Particles wlnu = wenufinder.bosons() + wmunufinder.bosons();
      // const ZFinder& znuenuefinder = applyProjection<ZFinder>(event, "znuenuefinder");
      // const ZFinder& znumunumufinder = applyProjection<ZFinder>(event, "znumunumufinder");
      // const ZFinder& znutaunutaufinder = applyProjection<ZFinder>(event, "znutaunutaufinder");
      // const Particles znunu = znuenuefinder.bosons() + znumunumufinder.bosons()+ znutaunutaufinder.bosons();

      ////////////////////////////////////////////////////////////////////////////////////////////
      ///////////// START OF CHANNEL DEPENDENT PART
      ////////////////////////////////////////////////////////////////////////////////////////////

      // apply the projections
      const ZFinder& zeefinder = applyProjection<ZFinder>(event, "zeefinder");
      const ZFinder& zmmfinder = applyProjection<ZFinder>(event, "zmmfinder");
      const Particles zll = zeefinder.bosons() + zmmfinder.bosons();
      // bool higgs_is_undecayed = false;
      bool higgs_is_undecayed = true;
      ParticleVector higgses =
      applyProjection<IdentifiedFinalState>(event, "Higgses").particles();
      // unstable higgs
      const UnstableFinalState& undec_higgs = applyProjection<UnstableFinalState>(event, "UFS");
      // jets
      const FastJets& jetpro = applyProjection<FastJets>(event,"jetpro");
      ParticleVector rest =
      applyProjection<VetoedFinalState>(event, "Rest").particles();
      
      if(higgses.size()==0)
        for (size_t i(0);i<rest.size();++i) {
        if(rest[i].pid()==25){
          // cout << " i= " << i << " PdgId= " << rest[i].pid() << endl;
          higgses.push_back(rest[i]);
          FourMomentum dummy_mom(0,0,0,0);
          // cout << "before rest["<<i<<"].momentum()= " << rest[i].momentum() << " PdgId= " << rest[i].pid() << endl;
          rest[i].setMomentum(dummy_mom);
          // cout << "after rest["<<i<<"].momentum()= " << rest[i].momentum() << " PdgId= " << rest[i].pid() << endl;
        }
      }

      // only continue if a Z has been found
      if ( zll.empty() ) vetoEvent;

      ////////////////////////////////////////////////////////////////////////////////////////////
      ///////////// END OF CHANNEL DEPENDENT PART
      ////////////////////////////////////////////////////////////////////////////////////////////

      Jets ajets, sel_ajets;
      ajets.clear();
      sel_ajets.clear();
      Jet h0, h1; 
      FourMomentum  higgs_candidate,Z, HV;
      std::vector<size_t> b_indices;
      int na = 0.0;
      int nb = 0.0;
      const Jets alljets = jetpro.jetsByPt();
      // find all jets with bs
      for (size_t i = 0; i < alljets.size(); ++i) {
        if (alljets[i].containsBottom()) {
          b_indices.push_back(i);
          if (alljets[i].pT() > cut_bjets_ptmin && abs(alljets[i].eta()) < cut_bjets_etamax) nb++;
        }
      }

      // std::cout << "higgses.size()= "<< higgses.size() << " alljets.size()= "<< alljets.size() << " b_indices.size()= "<< b_indices.size() << " undec_higgs.size()= "<< undec_higgs.size() << std::endl;
      // require at least 2 b jets or undecayed higgs
      if (b_indices.size() < 2 && undec_higgs.size() == 0 && higgses.size() == 0) vetoEvent;
      if (higgses.size()>1) vetoEvent;

      
      size_t idx0 = -1;
      size_t idx1 = -1;
      higgs_candidate = higgses[0].momentum();
      // if (b_indices.size() < 2){
        // higgs_is_undecayed = true;
        // // higgs_candidate = undec_higgs.particles()[0];
      // }else if(higgs_is_undecayed == false) {
        // // construct highest dijet pt b-jet pair if higgs is decayed
        // double max_pt = 0.0*GeV;
        // // pairwise iterate
        // for (size_t i = 0; i < b_indices.size()-1; ++i) {
          // for (size_t j = i+1; j < b_indices.size(); ++j) {
            // higgs_candidate = alljets[b_indices[i]].momentum() + alljets[b_indices[j]].momentum();
            // if (higgs_candidate.pT() > max_pt) {
              // max_pt = higgs_candidate.pT();
              // idx0 = b_indices[i];
              // idx1 = b_indices[j];
            // }
          // }
        // }

        // h0 = alljets[idx0];
        // h1 = alljets[idx1];
        // // final highest pt dijet pair
        // higgs_candidate = h0.momentum() + h1.momentum();
      // }
      
      // cout << "before veto: higgs_candidate.pT()= " << higgs_candidate.pT() << endl;
      if( ! higgs_candidate.pT()>0 ) vetoEvent;

      Z = zll[0].momentum();
      
      Particles leptons = zmmfinder.bosons().size()>0 ? zmmfinder.constituents(cmpMomByPt) : zeefinder.constituents(cmpMomByPt);
      // const FourMomentum lminus = leptons[0].charge() < 0 ? leptons[0].momentum() : leptons[1].momentum();
      // const FourMomentum lplus = leptons[0].charge() < 0 ? leptons[1].momentum() : leptons[0].momentum();      
      const FourMomentum leadlep = leptons[0].momentum();
      const FourMomentum subleadlep = leptons[1].momentum();

      // HV system
      HV = higgs_candidate + Z;
      
      // add everything else to the ajets 
      for (size_t i = 0; i < alljets.size(); ++i) {
        if (i != idx0 && i != idx1){
          ajets.push_back(alljets[i]);
          if (alljets[i].pT() > cut_addjets_ptmin && abs(alljets[i].eta()) < cut_addjets_etamax){
            na++;
            sel_ajets.push_back(alljets[i]);
          }
        }
      }
      
      //////////////////////////////////////////////////
      // fill histos
      //////////////////////////////////////////////////
      
      //////////////////////////////////////////////////
      /// Only Inclusive pT, inclusive jet
      //////////////////////////////////////////////////
      
      // Jet resolutions and integrated jet rates
      double d_ij;
      const FastJets& jetpro_kt = applyProjection<FastJets>(event,"jetpro_kt");
      const fastjet::ClusterSequence* seq = jetpro_kt.clusterSeq();
      if (seq != NULL) {
        for (size_t i = 0; i < njets; ++i) {
          // Jet resolution i -> j
          d_ij = log10(sqrt(seq->exclusive_dmerge_max(i)));

          // Fill differential jet resolution
          _h_VptInc_AjetInc__log10_d[i]->fill(d_ij, weight);

          //fill aJet pT histos
          _h_VptInc_AjetInc__ajets_pT[i]->fill(ajets[i].pT(),weight);
          _h_VptInc_AjetInc__ajets_eta[i]->fill(ajets[i].eta(),weight);
        }
      }
      for (size_t iajet=0; iajet < 1; iajet++){
        if(sel_ajets.size()>0){
          _h_VptInc_AjetInc__Sel_ajets_pT[iajet]->fill(sel_ajets[0].pT(),weight);
          _h_VptInc_AjetInc__Sel_ajets_eta[iajet]->fill(sel_ajets[0].eta(),weight);
        }
      }
      for (size_t ivpt=0; ivpt < 1; ivpt++){
        for (size_t iajet=0; iajet < 1; iajet++){
          if(!higgs_is_undecayed){
            _h__dau_eta[ivpt][iajet][0]->fill(h0.eta(),weight);
            _h__dau_eta[ivpt][iajet][1]->fill(h1.eta(),weight);
            _h__dau_pT[ivpt][iajet][0]->fill(h0.pT(),weight);
            _h__dau_pT[ivpt][iajet][1]->fill(h1.pT(),weight);
            _h__dR_jj[ivpt][iajet]->fill(deltaR(h0,h1),weight);
            _h__deta_jj[ivpt][iajet]->fill(deltaEta(h0,h1),weight);
            _h__dphi_jj[ivpt][iajet]->fill(deltaPhi(h0,h1),weight);
          }
          _h__weights[ivpt][iajet]->fill(weight/(std::abs(weight)>0?std::abs(weight):1));
          
          
          _h__V_mass[ivpt][iajet]->fill(Z.mass(),weight);
          _h__V_rap[ivpt][iajet]->fill(Z.rapidity(),weight);
          _h__V_pT[ivpt][iajet]->fill(Z.pT(),weight);
          
          _h__higgs_candidate_mass[ivpt][iajet]->fill(higgs_candidate.mass(),weight);
          
          _h__HV_rap[ivpt][iajet]->fill(HV.rapidity(),weight);
          _h__HV_pT[ivpt][iajet]->fill(HV.pT(),weight);

          _h__Sel_najets[ivpt][iajet]->fill(na,weight);
          _h__Sel_nbjets[ivpt][iajet]->fill(nb,weight);

          _h__dR_HV[ivpt][iajet]->fill(deltaR(higgs_candidate,Z),weight);
          _h__deta_HV[ivpt][iajet]->fill(deltaEta(higgs_candidate,Z),weight);
          _h__dphi_HV[ivpt][iajet]->fill(deltaPhi(higgs_candidate,Z),weight);
        }
      }
      //////////////////////////////////////////////////
      /// Loop over pT bins, jet bins
      //////////////////////////////////////////////////
      for (size_t ivpt=0; ivpt < Vpt_bins; ivpt++){
        
        // std::cout << "Z.pT()= "<< Z.pT() << std::endl;
     
        if(Z.pT() < cut_Zll_pt_low_edge_bin[ivpt] || Z.pT() > cut_Zll_pt_high_edge_bin[ivpt]) continue;
          
        for (size_t iajet=0; iajet < NJet_bins; iajet++){

          // std::cout << "sel_ajets.size()= "<< sel_ajets.size() << std::endl;
          if(sel_ajets.size() < cut_najets_low_edge_bin[iajet] || sel_ajets.size() >= cut_najets_high_edge_bin[iajet]) continue;
          
          // std::cout 
          // << "leadlep.pT()= "<< leadlep.pT() << " leadlep.eta()= "<< leadlep.eta() 
          // << " subleadlep.pT()= "<< subleadlep.pT() << " subleadlep.eta()= "<< subleadlep.eta() 
          // << " higgs_candidate.pT()= "<< higgs_candidate.pT() << " higgs_candidate.rapidity()= "<< higgs_candidate.rapidity() 
          // << std::endl;
          
          _h__leadlep_pT[ivpt][iajet]->fill(leadlep.pT(),weight);
          _h__leadlep_eta[ivpt][iajet]->fill(leadlep.eta(),weight);
          _h__subleadlep_pT[ivpt][iajet]->fill(subleadlep.pT(),weight);
          _h__subleadlep_eta[ivpt][iajet]->fill(subleadlep.eta(),weight);

          _h__higgs_candidate_rap[ivpt][iajet]->fill(higgs_candidate.rapidity(),weight);
          _h__higgs_candidate_pT[ivpt][iajet]->fill(higgs_candidate.pT(),weight);

        }
      }
    }

    /// Normalise histograms etc., after the run
    void finalize() {

      for (size_t i = 0; i < njets; ++i) {
        scale(_h_VptInc_AjetInc__log10_d[i], crossSection()/sumOfWeights());
        scale(_h_VptInc_AjetInc__ajets_pT[i], crossSection()/sumOfWeights());
        scale(_h_VptInc_AjetInc__ajets_eta[i], crossSection()/sumOfWeights());
      }
      
      scale(_h_VptInc_AjetInc__Sel_ajets_pT[0], crossSection()/sumOfWeights());
      scale(_h_VptInc_AjetInc__Sel_ajets_eta[0], crossSection()/sumOfWeights());

      for (size_t ivpt=0; ivpt < 1; ivpt++){
        for (size_t iajet=0; iajet < 1; iajet++){
          
          for (size_t i = 0; i < 2; ++i) {
            scale(_h__dau_eta[ivpt][iajet][i], crossSection()/sumOfWeights());
            scale(_h__dau_pT[ivpt][iajet][i], crossSection()/sumOfWeights());
          }
          
          scale(_h__V_mass[ivpt][iajet], crossSection()/sumOfWeights());
          scale(_h__V_rap[ivpt][iajet], crossSection()/sumOfWeights());
          scale(_h__V_pT[ivpt][iajet], crossSection()/sumOfWeights());
          scale(_h__higgs_candidate_mass[ivpt][iajet], crossSection()/sumOfWeights());
          
          scale(_h__HV_pT[ivpt][iajet], crossSection()/sumOfWeights());
          scale(_h__HV_rap[ivpt][iajet], crossSection()/sumOfWeights());

          scale(_h__dR_jj[ivpt][iajet], crossSection()/sumOfWeights());
          scale(_h__deta_jj[ivpt][iajet], crossSection()/sumOfWeights());
          scale(_h__dphi_jj[ivpt][iajet], crossSection()/sumOfWeights());

          scale(_h__dR_HV[ivpt][iajet], crossSection()/sumOfWeights());
          scale(_h__deta_HV[ivpt][iajet], crossSection()/sumOfWeights());
          scale(_h__dphi_HV[ivpt][iajet], crossSection()/sumOfWeights());

          scale(_h__Sel_najets[ivpt][iajet], crossSection()/sumOfWeights());
          scale(_h__Sel_nbjets[ivpt][iajet], crossSection()/sumOfWeights());
        }
      }
      
      for (size_t ivpt=0; ivpt < Vpt_bins; ivpt++){
        for (size_t iajet=0; iajet < NJet_bins; iajet++){

          scale(_h__higgs_candidate_rap[ivpt][iajet], crossSection()/sumOfWeights());
          scale(_h__higgs_candidate_pT[ivpt][iajet], crossSection()/sumOfWeights());
          scale(_h__leadlep_pT[ivpt][iajet], crossSection()/sumOfWeights());
          scale(_h__leadlep_eta[ivpt][iajet], crossSection()/sumOfWeights());
          scale(_h__subleadlep_pT[ivpt][iajet], crossSection()/sumOfWeights());
          scale(_h__subleadlep_eta[ivpt][iajet], crossSection()/sumOfWeights());
        }
      }
    }

  };

  DECLARE_RIVET_PLUGIN(MC_ZllHbb);

}
