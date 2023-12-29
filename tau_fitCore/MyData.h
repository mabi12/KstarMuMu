#ifndef MyDATA
#define MyDATA

#include "TChain.h"
#include "TTree.h"
#include "TFile.h"
#include "RooCategory.h"
#include "RooRealVar.h"
#include "RooDataSet.h"

class MyData{
public:
    MyData(TString datasetName, TString rooDatasetFile);
    ~MyData();

    TString datasetName;
    TString rooDatasetFile;
    
    TString cuts;
    TString customCuts = "";
    
    Double_t massPDG;
    Double_t massMin;
    Double_t massMax;
    Double_t tauMin;
    Double_t tauMax;
    Double_t tauErrMin;
    Double_t tauErrMax;
    Double_t massErrMin;
    Double_t massErrMax;
    Double_t ptMin;
    Double_t ptMax;
    Double_t wMin;
    Double_t wMax;

    TString mainTree;
    TString trigTree;

    TString triggers2015;
    TString triggers2016A;
    TString triggers2016B;
    TString triggers2016C;
    TString triggers2017;
    TString triggers2018;

    TString tauCorr2015;
    TString tauCorr2016A;
    TString tauCorr2016B;
    TString tauCorr2016C;
    TString tauCorr2017;
    TString tauCorr2018;
    TString tauCorrRun3;

    set<TString> years {"2015","2016A", "2016B", "2016C", "2017", "2018"};
    RooCategory* yearsCategory;
    void printCutsAndLimits();
    void printTriggers();
    void printLifetimeCorrections();
    
    void useDefaultCutsAndLimitsBd(bool print);
    void useDefaultTriggersBd(bool print);
    void useDefaultLifetimeCorrectionsBd(bool print);
    
    RooCategory* getYearsCategory();
    RooDataSet * readPeriod(TTree* tree, TString year, bool useTriggers, bool useLifetimeCorrections);
    RooDataSet * createData(TString path, bool useTriggers, bool useLifetimeCorrections, TString year);
    RooDataSet * readData(TString year, TString weightName);
    
    void save(RooAbsData *data , TString fileName);
    void sample(RooAbsData *data , RooArgSet * vars, int sample);
};
#endif
