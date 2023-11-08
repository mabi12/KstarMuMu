#ifndef LIFETIME_SPLOT
#define LIFETIME_SPLOT

#include "RooAbsPdf.h"
#include "RooRealVar.h"
#include "RooStats/SPlot.h"
#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooGaussian.h"
#include "RooExponential.h"
#include "RooChebychev.h"
#include "RooAddPdf.h"
#include "RooArgSet.h"
#include "RooArgList.h"

#include "RooProdPdf.h"
#include "RooAddition.h"
#include "RooProduct.h"
#include "TCanvas.h"
#include "RooAbsPdf.h"
#include "RooFit.h"
#include "RooFitResult.h"
#include "RooWorkspace.h"
#include "RooConstVar.h"

class lifetime_splot {
public:
    //! Constructor
    lifetime_splot(RooRealVar *mass, RooRealVar *timeErr, RooRealVar *pt, RooDataSet *inputData, RooAbsPdf *massSigPDF,RooAbsPdf *massBckPDF, RooAbsPdf *massJpsiXPDF=NULL);

    //! Destructor
    ~lifetime_splot();

    void BuildTotalPDF(bool useJpsiXgenericPDF=true);
    void RunSplot();
    void RunSBS();
    void PlotAndSaveResults(TString FileName = "punzi_timeErr_splot.root", TString PlotSaveName = "punzi_timeErr.png");

    RooHistPdf *signalPunziPDF;
    RooHistPdf *backgroundPunziPDF;
    RooHistPdf *JpsiXPunziPDF;
    RooAddPdf  *TotPunziPDF;    

    RooRealVar *JpsiX_scale ;
    RooRealVar *JpsiX_offset;

    TH1 *TH_signalPunzi=NULL;
    TH1 *TH_backgroundPunzi;
    TH1 *TH_JpsiXPunzi;

    TH1 *TH_signalPunzi_timeErr;
    TH1 *TH_backgroundPunzi_timeErr;
    TH1 *TH_JpsiXPunzi_timeErr;


private:

    RooAbsPdf *m_massSigPDF;
    RooAbsPdf *m_massBckPDF;
    RooAbsPdf *m_totalPDF;
    RooAbsPdf *m_massJpsiXPDF=NULL;

    RooRealVar *m_mass;
    RooRealVar *m_timeErr;
    RooRealVar *m_pt;

    RooRealVar *m_signalYield;
    RooRealVar *m_bkgYield;
    RooRealVar *m_JpsiXYield;

    RooDataSet *m_data;
};


#endif
