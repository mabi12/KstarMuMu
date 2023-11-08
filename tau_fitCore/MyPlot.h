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
using namespace RooFit;

class MyPlot{
public:
    MyPlot(RooRealVar *var,  int nBins, TString outDir, TString tagATLAS );
    ~MyPlot();

    int nBins;
    TString name;
    TString outDir;
    TString tagATLAS;
    //TODO atlas lumi and shit

    RooRealVar *var;
    TCanvas *c;
    TPad* pads[2];
        
    RooPlot * plotVarAndPull(RooAbsData* data, RooAbsPdf*fit, TString outName);
    //, bool useWeight, int nBins, TString components[], TString data_legend);
    RooPlot * plotVarAndPullNice(RooAbsData* data, RooAbsPdf*fit, TString outName, bool useWeight, int nBins, TString components[], TString data_legend);
    RooPlot * plotVarAndPullNice2(RooAbsData* data, RooAbsPdf*fit, TString outName, bool useWeight, int nBins, TString components[], TString data_legend);
    RooPlot * plotVarAndResid(RooAbsData* data, RooAbsPdf*fit, TString outName, bool useWeight, int nBins, TString components[]);
    RooHist * getResid(RooAbsData* data, RooAbsPdf*fit, bool useWeight, int nBins);
    
};
#endif
