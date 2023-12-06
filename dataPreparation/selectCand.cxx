//rewritten according selectCand.cxx from Radek Novotny
#include "TRandom.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1D.h"
#include <iostream>
#include "TRandom3.h"
using namespace std;

class IReader{
protected:
    static std::vector<IReader*> s_readers;
   public:
    virtual void selectCand(int i)= 0;
    IReader(){
        s_readers.push_back(this);
    }
    static void ApplyAllSelect(int i){
        for(auto ptr : s_readers) ptr->selectCand(i);
    }
};

std::vector<IReader*> IReader::s_readers = std::vector<IReader*>();
template <class T>
class VectorToScalar : IReader{
public:
    std::vector<T> *vect;
    T scal;
public:
    VectorToScalar(TString name, TTree* allcand, TTree* outtree) : IReader() {
        vect=nullptr;
        allcand->SetBranchStatus(name, true);
        allcand->SetBranchAddress(name, &vect);
        outtree->Branch(name, &scal);
    }
    virtual void selectCand(int i) override {
        scal = vect->at(i);
    }
    T at(int i) {  selectCand(i); return scal; }
    size_t size() const {return vect->size(); }
};

template <class T>
class ScalarToScalar : IReader {
    T scal;
public:
    ScalarToScalar(TString name, TChain* allcand, TTree* outtree){
        scal = 0;
        allcand->SetBranchStatus(name, true);
        allcand->SetBranchAddress(name, &scal);
        outtree->Branch(name, &scal);
    }
    virtual void selectCand(int i) override { }

};



void selectCand(TString inputFileName="ntuple-300700_part_02", TString outputFileName="sg_wide", int seed=42){ //300700_part_0 
    TString tag = "_refit";
    TString cuts = "BmumuKst_meson0_pT" + tag + " > 500 && BmumuKst_meson1_pT" + tag + " > 500"
                    "&& BmumuKst_meson0_eta" + tag + " < 2.5 && BmumuKst_meson1_eta" + tag + " < 2.5"
                    "&& abs(BmumuKst_muon0_pT" + tag + ")"+" > 3500 && abs(BmumuKst_muon1_pT" + tag + ")"+" > 3500"
                    "&& abs(BmumuKst_muon0_eta" + tag + ")"+" < 2.5 && abs(BmumuKst_muon1_eta" + tag + ")"+" < 2.5"
                    "&& BmumuKst_diMeson_pT" + tag + ">3000"
                    "&& ((BmumuKst_kaonPion_mass" + tag + " > 846 && BmumuKst_kaonPion_mass" + tag + " < 946) ||  (BmumuKst_pionKaon_mass" + tag + " > 846 && BmumuKst_pionKaon_mass" + tag + " < 946))"
                    "&& BmumuKst_diMuon_mass" + tag + "*BmumuKst_diMuon_mass" + tag + ">1000000 && BmumuKst_diMuon_mass" + tag + "*BmumuKst_diMuon_mass" + tag +"<9000000"
                    "&& ((BmumuKst_B_mass" + tag + "> 5150 && BmumuKst_B_mass" + tag + "<5700)||(BmumuKst_Bbar_mass" + tag +">5150 && BmumuKst_Bbar_mass" + tag + "<5700))"
                    "&& (BmumuKst_B_tau_invM_PVMinA0/BmumuKst_B_tau_invM_PVMinA0_err>12.5 || BmumuKst_Bbar_tau_invM_PVMinA0/BmumuKst_Bbar_tau_invM_PVMinA0_err>12.5)"
                    "&& BmumuKst_diMuon_chi2_over_nDoF<10"
                    "&& BmumuKst_chi2_over_nDoF<2";
    cout<<cuts<<endl;
    const Float_t pdgKst = 896;
    TString inputFile = "../data/" + inputFileName + ".root";
    TString outputFile = "../data/" + outputFileName + "_bestCand.root";

    std::cout << "input  file: " << inputFile   << std::endl;
    std::cout << "output file: " << outputFile  << std::endl;

    TChain *allcandt = new TChain("Nominal/BaseSelection_KStarMuMu_Selection");
    allcandt->Add(inputFile);
    if(allcandt==0) cout <<" allcandt is null " << endl;
    auto en_before = allcandt->GetEntries();

    TTree * allcandt_tree = allcandt->CopyTree(cuts);
    auto en_after = allcandt_tree->GetEntries();
    cout<<"========================="<<endl;
    cout<<"after application of cuts:"<<endl;
    cout<<"========================="<<endl;
    cout<<cuts<<endl;
    cout<<"========================="<<endl;
    cout<<"number of events changed: "<<en_before<<" -> "<<en_after<<endl;
    cout<<"========================="<<endl;
    
    
    allcandt_tree->SetBranchStatus("*", false);
    TFile *newfile = new TFile(outputFile, "RECREATE");
    TTree *tree    = new TTree("BdBestChi", "BdBestChi");
    VectorToScalar<float> B_chi2_ndof("BmumuKst_chi2_over_nDoF", allcandt_tree, tree);
    VectorToScalar<float> Kpi_mass("BmumuKst_kaonPion_mass" + tag, allcandt_tree, tree);
    VectorToScalar<float> piK_mass("BmumuKst_pionKaon_mass" + tag, allcandt_tree, tree);
    VectorToScalar<float> B_mass("BmumuKst_B_mass" + tag, allcandt_tree, tree);
    VectorToScalar<float> B_mass_err("BmumuKst_B_mass" + tag + "_err", allcandt_tree, tree);
    VectorToScalar<float> Bbar_mass("BmumuKst_Bbar_mass" + tag, allcandt_tree, tree);
    VectorToScalar<float> Bbar_mass_err("BmumuKst_Bbar_mass" + tag + "_err", allcandt_tree, tree);
    VectorToScalar<float> B_tau("BmumuKst_B_tau_invM_PVMinA0", allcandt_tree, tree);
    VectorToScalar<float> B_tau_err("BmumuKst_B_tau_invM_PVMinA0_err", allcandt_tree, tree);
    VectorToScalar<float> Bbar_tau("BmumuKst_Bbar_tau_invM_PVMinA0", allcandt_tree, tree);
    VectorToScalar<float> Bbar_tau_err("BmumuKst_Bbar_tau_invM_PVMinA0_err", allcandt_tree, tree);
    VectorToScalar<float> Bd_decay_angle_cos_thetaL("BmumuKst_Bd_decay_angle_cos_thetaL" + tag, allcandt_tree, tree);
    VectorToScalar<float> Bdbar_decay_angle_cos_thetaL("BmumuKst_Bdbar_decay_angle_cos_thetaL" + tag, allcandt_tree, tree);
    VectorToScalar<float> Bd_decay_angle_cos_thetaK("BmumuKst_Bd_decay_angle_cos_thetaK" + tag, allcandt_tree, tree);
    VectorToScalar<float> Bdbar_decay_angle_cos_thetaK("BmumuKst_Bdbar_decay_angle_cos_thetaK" + tag, allcandt_tree, tree);
    VectorToScalar<float> Bd_decay_angle_phi("BmumuKst_Bd_decay_angle_phi" + tag, allcandt_tree, tree);
    VectorToScalar<float> Bdbar_decay_angle_phi("BmumuKst_Bdbar_decay_angle_phi" + tag, allcandt_tree, tree); 
    // VectorToScalar<float> B_eta("BmumuKst_B_eta", allcandt_tree, tree);

    VectorToScalar<float> muon0_eta("BmumuKst_muon0_eta" + tag, allcandt_tree, tree);
    VectorToScalar<float> muon0_pT("BmumuKst_muon0_pT" + tag, allcandt_tree, tree);
    VectorToScalar<float> muon1_eta("BmumuKst_muon1_eta" + tag, allcandt_tree, tree);
    VectorToScalar<float> muon1_pT("BmumuKst_muon1_pT" + tag, allcandt_tree, tree);
    VectorToScalar<float> diMuon_mass("BmumuKst_diMuon_mass" + tag, allcandt_tree, tree);
    VectorToScalar<float> diMuon_chi2_ndof("BmumuKst_diMuon_chi2_over_nDoF", allcandt_tree, tree);

    VectorToScalar<float> meson0_eta("BmumuKst_meson0_eta" + tag, allcandt_tree, tree);
    VectorToScalar<float> meson0_pT("BmumuKst_meson0_pT" + tag, allcandt_tree, tree);
    VectorToScalar<float> meson1_eta("BmumuKst_meson1_eta" + tag, allcandt_tree, tree);
    VectorToScalar<float> meson1_pT("BmumuKst_meson1_pT" + tag, allcandt_tree, tree);
    VectorToScalar<float> diMeson_pT("BmumuKst_diMeson_pT" + tag, allcandt_tree, tree);

    VectorToScalar<UInt_t> isTrueNonresBd("BmumuKst_isTrueNonresBd", allcandt_tree, tree);

    int nCandidates;
    int nCandidatesPassed;
    int Bd; //bd = 1 or bdbar = 0
    float selected_B_mass;
    float selected_Kst_mass;
    tree->Branch("nCandidates", &nCandidates);
    tree->Branch("nCandidatesPassed", &nCandidatesPassed);
    tree->Branch("Bd", &Bd);
    tree->Branch("B_mass", &selected_B_mass);
    tree->Branch("B_mass", &selected_B_mass);
    tree->Branch("B_tau", &selected_B_mass);
    tree->Branch("Kst_mass", &selected_Kst_mass);

    int eventsWithoutPass = 0;

    
    
//     VectorToScalar<float> B_mass_err("B_mass_err", allcandt, tree);
    
    
//     // ScalarToScalar<UInt_t>    run_number("run_number", allcandt, tree);
//     // ScalarToScalar<UInt_t>    lumi_block("lumi_block", allcandt, tree);
//     // ScalarToScalar<ULong64_t> evt_number("evt_number", allcandt, tree);
    
//     VectorToScalar<float> B_chi2_ndof("B_chi2_ndof", allcandt, tree);
//     VectorToScalar<float> B_mass("B_mass", allcandt, tree);
// //     VectorToScalar<float> B_mass_err("B_mass_err", allcandt, tree);
// //     VectorToScalar<float> B_tau_MinA0("B_tau_MinA0", allcandt, tree);
// //     VectorToScalar<float> B_tau_MinA0_err("B_tau_MinA0_err", allcandt, tree);
// //     VectorToScalar<float> B_pT("B_pT", allcandt, tree);
// // //    VectorToScalar<float> Bd_TrueLifetime("Bd_TrueLifetime", allcandt, tree);
    
// //     ScalarToScalar<bool> pass_GRL("pass_GRL", allcandt, tree);
// //     VectorToScalar<bool> inclusiveTrueBd("inclusiveTrueBd", allcandt, tree);
    
    
// //     VectorToScalar<float> B_Jpsi_mass("B_Jpsi_mass", allcandt, tree);
    
// //     VectorToScalar<float> B_trk1_pT("B_trk1_pT", allcandt, tree);
// //     VectorToScalar<float> B_trk2_pT("B_trk2_pT", allcandt, tree);
// //     VectorToScalar<float> Kpi_pT("Kpi_pT", allcandt, tree);
//     VectorToScalar<float> Kpi_mass("Kpi_mass", allcandt, tree);
    

    // TH1* plot = new TH1I("candN", "candN", 15, 0, 15); 
    // TH1* plotch = new TH1I("candNch", "candNch", 15, 0, 15);

    // TRandom3 *rand = new TRandom3(seed); 
    double chi2Min;
    int chi2MinIndex;
    for(Long64_t i = 0; i< en_after; i++){
        if (i % 100000 == 0)    std::cout<<"\r      " <<i<<" / "<<en_after<< std::flush;
        allcandt_tree->GetEntry(i);
        // std::cout << i <<" ";
        chi2Min = 100;
        chi2MinIndex = -10;
        nCandidates = B_mass.size();
        nCandidatesPassed = 0;
        if(nCandidates==0){
            std::cout << "Error - event has no Bmass " << i << std::endl;
            continue;
        }
        for(int j = 0; j<nCandidates;j++){
            // std::cout << j;
            if (B_chi2_ndof.at(j) >= 2) continue;
            if ((B_mass.at(j) <= 4900 || B_mass.at(j) >= 5900) && (Bbar_mass.at(j) <= 4900 || Bbar_mass.at(j) >= 5900)) continue;  //TODO teraz to je ze ak obe su mimo
            // if ((Kpi_mass.at(j) <= 846 || Kpi_mass.at(j) >= 946) && (piK_mass.at(j) <= 846 || piK_mass.at(j) >= 946)) continue;  //TODO teraz to je ze ak obe su mimo
            // if (diMuon_mass.at(j)*diMuon_mass.at(j)<=40000 || diMuon_mass.at(j)*diMuon_mass.at(j)>=6000000) continue;  //TODO phi veto
            // if (B_tau.at(j)/B_tau_err.at(j) <= 12.75) continue; //TODO 12.75?
            // if (diMeson_pT.at(j) <= 3000) continue;
            // if (muon0_pT.at(j) <= 3500 || muon1_pT.at(j) <= 3500) continue;
            // if (muon0_eta.at(j) >= 2.5 || muon1_eta.at(j) >= 2.5) continue;
            //if (meson0_eta.at(j) >= 2.5 || meson1_eta.at(j) >= 2.5) continue;
            // if (B_eta.at(j) >= 2.5 ) continue; //TODO
            //TODO Kpi_eta.at(j) >= 2.5  && piK_eta.at(j) >= 2.5
            //TODO dimuon? .at(j) >= 2.5
            // if (meson0_pT.at(j) <= 500 || meson1_pT.at(j) <=500) continue;
            // if (diMuon_chi2_ndof.at(j) >= 10) continue;
            //TODO costheta <=0.999
            // TODO radiative   (B_mass.at(j) - pdgB) - (diMuon_mass.at(j) - mcc / cc = Jpsi psi2s
            nCandidatesPassed++;
            if(chi2Min>B_chi2_ndof.at(j)){
                chi2Min = B_chi2_ndof.at(j);
                chi2MinIndex = j;
            }
        }
        if(chi2MinIndex>-1){
            // plot->Fill(B_mass.size());
            // plotch->Fill(chi2MinIndex);
            //select B flavour from Kst hypothesis
            if (abs(Kpi_mass.at(chi2MinIndex) - pdgKst)<= abs(piK_mass.at(chi2MinIndex) - pdgKst)){ //Bd -> Kst -> K+ pi-
                Bd = 1;
                selected_B_mass = B_mass.at(chi2MinIndex);
                selected_Kst_mass = Kpi_mass.at(chi2MinIndex);
            } else {//Bd_bar -> Kst_bar -> K- pi+
                Bd = 0;
                selected_B_mass = Bbar_mass.at(chi2MinIndex);
                selected_Kst_mass = piK_mass.at(chi2MinIndex);
            }
            IReader::ApplyAllSelect(chi2MinIndex);
            tree->Fill();
        } else eventsWithoutPass++;
        // std::cout << std::endl;
        
    }
//TODO clone trigtree
//     truthtree->CloneTree(-1, "fast");
    auto en_new = tree->GetEntries();
    cout<<"============================"<<endl;
    cout<<"created tree with "<< en_new <<" events"<<endl;
    cout<<"events without passed candidate "<<eventsWithoutPass  <<endl;
    cout<<"============================"<<endl;
    newfile->Write();
    newfile->Close();
    
}
