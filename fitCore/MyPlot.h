#ifndef MYPLOT
#define MYPLOT

#include "RooDataSet.h"
#include "RooAbsData.h"
#include "RooAbsPdf.h"
#include "RooPlot.h"
#include "RooHist.h"

#include "TString.h"
#include "TLine.h"
#include "TLatex.h"
#include "TLegend.h"

#include "./atlasstyle-00-04-02/AtlasStyle.C"
#include "./atlasstyle-00-04-02/AtlasUtils.C"
#include "./atlasstyle-00-04-02/AtlasLabels.C"

class MyPlot{
public:
    MyPlot(RooRealVar *var, int nBins = 100, TString outDir = "./out/", TString tagATLAS = "Internal");

    int nBins;
    TString name;
    TString outDir;
    TString tagATLAS;
    //TODO atlas lumi and shit

    RooRealVar *var;
    TCanvas *c;
    TPad* pads[2];
        
    RooPlot * plotVarAndPull(RooAbsData* data, RooAbsPdf*fit, int nParams, TString outName);
    RooPlot * plotVarAndResid(RooAbsData* data, RooAbsPdf*fit, TString outName, bool useWeight, int nBins, TString components[]);
    RooHist * getResid(RooAbsData* data, RooAbsPdf*fit, bool useWeight, int nBins);
    
};
#endif
