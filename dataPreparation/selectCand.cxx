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
    VectorToScalar(TString name, TChain* allcand, TTree* outtree) : IReader() {
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

void selectCand(double chi=5, int pT=5000, TString inputFileName="15main.root", TString outputFileName="", int seed=42){ 
    inputFile = "../data/" + inputFileName + ".root"
    outputFile = "../data/" + outputFileName + ".root"

    std::cout << "input  file: " << inputFile   << endl;
    std::cout << "output file: " << outputFile  << endl;

    TChain *allcandt = new TChain("BdAllCandidates");
    TChain *bestcand = new TChain("BdBestChi");
    TChain* trigtree = new TChain("BdTriggers");
//     TChain* truthtree = new TChain("BdTruth");

    allcandt->Add(inputFile);
    bestcand->Add(inputFile);
    trigtree->Add(inputFile);
//    truthtree->Add(inputFile);

    if(allcandt==0) cout <<" allcandt is null " << endl;
    if(bestcand==0) cout <<" bestcand is null " << endl;
    if(trigtree==0) cout <<" trigtree is null " << endl;
//     if(truthtree==0) cout <<" trigtree is null " << endl;

    bestcand->SetBranchStatus("*", false);
    allcandt->SetBranchStatus("*", false);
   
    
    auto en = trigtree->GetEntries();
    TFile *newfile = new TFile(outputFile, "RECREATE");
    TTree *tree    = new TTree("BdBestChi", "BdBestChi");
    
    
    ScalarToScalar<UInt_t>    run_number("run_number", allcandt, tree);
    ScalarToScalar<UInt_t>    lumi_block("lumi_block", allcandt, tree);
    ScalarToScalar<ULong64_t> evt_number("evt_number", allcandt, tree);
    
    VectorToScalar<float> B_chi2_ndof("B_chi2_ndof", allcandt, tree);
    VectorToScalar<float> B_mass("B_mass", allcandt, tree);
    VectorToScalar<float> B_mass_err("B_mass_err", allcandt, tree);
    VectorToScalar<float> B_tau_MinA0("B_tau_MinA0", allcandt, tree);
    VectorToScalar<float> B_tau_MinA0_err("B_tau_MinA0_err", allcandt, tree);
    VectorToScalar<float> B_pT("B_pT", allcandt, tree);
//    VectorToScalar<float> Bd_TrueLifetime("Bd_TrueLifetime", allcandt, tree);
    
    ScalarToScalar<bool> pass_GRL("pass_GRL", allcandt, tree);
    VectorToScalar<bool> inclusiveTrueBd("inclusiveTrueBd", allcandt, tree);
    
    VectorToScalar<float> B_mu1_eta("B_mu1_eta", allcandt, tree);
    VectorToScalar<float> B_mu1_pT("B_mu1_pT", allcandt, tree);
    VectorToScalar<float> B_mu2_eta("B_mu2_eta", allcandt, tree);
    VectorToScalar<float> B_mu2_pT("B_mu2_pT", allcandt, tree);
    VectorToScalar<float> B_Jpsi_mass("B_Jpsi_mass", allcandt, tree);
    
    VectorToScalar<float> B_trk1_pT("B_trk1_pT", allcandt, tree);
    VectorToScalar<float> B_trk2_pT("B_trk2_pT", allcandt, tree);
    VectorToScalar<float> Kpi_pT("Kpi_pT", allcandt, tree);
    VectorToScalar<float> Kpi_mass("Kpi_mass", allcandt, tree);
    

    TH1* plot = new TH1I("candN", "candN", 15, 0, 15); 
    TH1* plotch = new TH1I("candNch", "candNch", 15, 0, 15);

    TRandom3 *rand = new TRandom3(seed); 
    double chi2Min;
    int chi2MinIndex;
    int vectorSize;
    for(Long64_t i = 0; i< en; i++){
        if(i%1000000==0)std::cout << i << "/" << en << std::endl;
        allcandt->GetEntry(i);
        bestcand->GetEntry(i);
        chi2Min = 100;
        chi2MinIndex = -10;
        vectorSize = B_mass.size();
        if(vectorSize==0){
            std::cout << "Error - event has no Bmass " << i << std::endl;
            continue;
        }
        for(int j = 0; j<vectorSize;j++){
            if(B_trk2_pT.at(j)<500 || B_trk1_pT.at(j)<1000 || Kpi_pT.at(j)< pT)continue;
            if(B_chi2_ndof.at(j) > chi)continue;
            if(chi2Min>B_chi2_ndof.at(j)){
                chi2Min = B_chi2_ndof.at(j);
                chi2MinIndex = j;
            }
        }
        if(chi2MinIndex>-1){
            plot->Fill(B_mass.size());
            plotch->Fill(chi2MinIndex);
            IReader::ApplyAllSelect(chi2MinIndex);
            tree->Fill();
        }
    }
    cout << "Cloning Trigger" << endl;
    trigtree->CloneTree(-1, "fast");
//     truthtree->CloneTree(-1, "fast");
    newfile->Write();
    newfile->Close();
}
