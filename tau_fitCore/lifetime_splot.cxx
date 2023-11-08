#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "lifetime_splot.h"

using namespace std;
using namespace RooFit;
using namespace RooStats;


lifetime_splot::lifetime_splot(RooRealVar *mass, RooRealVar *timeErr, RooRealVar *pt, RooDataSet *inputData, RooAbsPdf *massSigPDF,RooAbsPdf *massBckPDF, RooAbsPdf *massJpsiXPDF=NULL) {
    cout << "running constructor" << endl;
    m_massSigPDF   = massSigPDF;
    m_massBckPDF   = massBckPDF;
    if (massJpsiXPDF!=NULL) m_massJpsiXPDF = massJpsiXPDF;

    m_mass    = new RooRealVar(*mass);
    m_timeErr = timeErr;
    m_pt      = pt;

    //m_data = inputData;
    m_data = (RooDataSet*) inputData->reduce(RooArgSet(*m_mass, *m_timeErr,*m_pt));

    m_signalYield = new RooRealVar ("m_signalYield", "signalYield",3.93617e+06,                    0, m_data->numEntries());
    m_JpsiXYield  = new RooRealVar ("m_JpsiXYield" , "JpsiXYield" ,7.19527e+05,                    0, m_data->numEntries());
    m_bkgYield    = new RooRealVar ("m_bkgYield"   , "bkgYield"   ,0.6*m_data->numEntries(), 0, m_data->numEntries());

}

lifetime_splot::~lifetime_splot(){
    //delete m_data;
}

void lifetime_splot::BuildTotalPDF(bool useJpsiXgenericPDF=true){
    cout << "running BuildTotalPdf" << endl;

    JpsiX_scale  = new RooRealVar    ("JpsiX_scale", "Scale of Tanh",-2.68675e-02,-0.2,-0.01);
    JpsiX_offset = new RooRealVar    ("JpsiX_offset", "Offset of Tanh", 5.12876e+03,5121.70-25,5121.70+25);

    RooGenericPdf *JpsiXmassGenericPDF = new RooGenericPdf("JpsiXmassGenericPDF","JpsiXmassGenericPDF","1+TMath::TanH(@2*(@0-@1))",RooArgList(*m_mass,*JpsiX_offset,*JpsiX_scale));
    if (m_massJpsiXPDF==NULL)cout << "massJpsiXPDF is NULL" << endl;

    if (useJpsiXgenericPDF and !m_massJpsiXPDF) {
        m_massJpsiXPDF = JpsiXmassGenericPDF;
    }
    m_massSigPDF->Print();
    m_massBckPDF->Print();
    if(m_massJpsiXPDF)m_massJpsiXPDF->Print();
    if (m_massJpsiXPDF!=NULL) m_totalPDF = new RooAddPdf  ("masstotalPDF", "masstotalPDF", RooArgList(*m_massSigPDF, *m_massBckPDF,*m_massJpsiXPDF), RooArgSet(*m_signalYield,*m_bkgYield,*m_JpsiXYield));
    else                      m_totalPDF = new RooAddPdf  ("masstotalPDF", "masstotalPDF", RooArgList(*m_massSigPDF, *m_massBckPDF                ), RooArgSet(*m_signalYield,*m_bkgYield              ));

    return;
}

void lifetime_splot::RunSplot(){
     cout << "running RunSplot" << endl;
     RooDataHist* mass_dataHist = new RooDataHist("mass_dataHist", " mass_dataHist", *m_mass,*m_data);     
     m_totalPDF->fitTo(*mass_dataHist,Extended(),RooFit::InitialHesse(kTRUE),NumCPU(8),Timer(kTRUE));

    RooFitResult *resultSimpleMass = m_totalPDF->fitTo(*m_data,Extended(),NumCPU(8),Timer(kTRUE),Save());

     RooArgList fitargs = resultSimpleMass->floatParsFinal(); 
     TIterator* iter = fitargs.createIterator();
     for (RooAbsArg *a = (RooAbsArg *) iter->Next(); a != 0; a = (RooAbsArg *) iter->Next()) {
         RooRealVar *rrv = dynamic_cast<RooRealVar*>(a);
         if(rrv) {
             TString var_name = rrv->GetName();
             if (var_name == "m_signalYield" or var_name == "m_JpsiXYield" or var_name == "m_bkgYield") continue;
             rrv->setConstant(true);rrv->Print();
         }
     } 
     cout << "running Splot itself" << endl;

     m_signalYield->setConstant(false);
     m_bkgYield->setConstant(false);
     m_JpsiXYield->setConstant(false);

     RooMsgService::instance().setSilentMode(true);
     RooStats::SPlot* sData=NULL;
     if (m_massJpsiXPDF) sData = new RooStats::SPlot("sData","An SPlot",*m_data, m_totalPDF,RooArgList(*m_signalYield,*m_bkgYield,*m_JpsiXYield) );
     else                sData = new RooStats::SPlot("sData","An SPlot",*m_data, m_totalPDF,RooArgList(*m_signalYield,*m_bkgYield              ) );

//     RooMsgService::instance().setSilentMode(false);

    std::cout << "Check SWeights:"    << std::endl << std::endl;
    std::cout <<  "Yield of Sig is "  << m_signalYield->getVal() << ".  From sWeights it is " << sData->GetYieldFromSWeight("m_signalYield") << std::endl;
    std::cout << "Yield of Bkg is "   << m_bkgYield   ->getVal() << ".  From sWeights it is " << sData->GetYieldFromSWeight("m_bkgYield")    << std::endl << std::endl;
    std::cout << "Yield of JpsiX is " << m_JpsiXYield ->getVal() << ".  From sWeights it is " << sData->GetYieldFromSWeight("m_JpsiXYield")  << std::endl << std::endl;

    static const TString sSig  ("_sig"  );
    static const TString sBkg  ("_bkg"  );
    static const TString sJpsiX("_jpsiX");

    RooDataSet* m_dataw_sig=NULL;
    RooDataSet* m_dataw_bck=NULL;
    RooDataSet* m_dataw_jpx=NULL;
    
    m_dataw_sig   = new RooDataSet(m_data->GetName()+sSig  ,m_data->GetTitle()+sSig  ,m_data,*m_data->get(),0,"m_signalYield_sw") ;
    m_dataw_bck   = new RooDataSet(m_data->GetName()+sBkg  ,m_data->GetTitle()+sBkg  ,m_data,*m_data->get(),0,"m_bkgYield_sw") ;
    if (m_massJpsiXPDF!=NULL) m_dataw_jpx = new RooDataSet(m_data->GetName()+sJpsiX,m_data->GetTitle()+sJpsiX,m_data,*m_data->get(),0,"m_JpsiXYield_sw") ;


    TString name_hist_obs_selection = "B_tau_MinA0_err:B_pT";

    TH_signalPunzi     = m_dataw_sig->createHistogram(*m_timeErr,*m_pt);
    TH_backgroundPunzi = m_dataw_bck->createHistogram(*m_timeErr,*m_pt);
    if (m_massJpsiXPDF!=NULL)  TH_JpsiXPunzi = m_dataw_jpx->createHistogram(*m_timeErr,*m_pt);

    if (m_massJpsiXPDF!=NULL){
        for (int bin_i=1;bin_i< TH_JpsiXPunzi->GetNbinsX()+1;bin_i++){
            for (int bin_j=1;bin_j<TH_JpsiXPunzi->GetNbinsY()+1;bin_j++){
                double tested_bin_value = TH_JpsiXPunzi->GetBinContent(bin_i,bin_j);
                if (tested_bin_value<=0) TH_JpsiXPunzi->SetBinContent(bin_i,bin_j,1.e-5);
            }
        }
    }

//    TH_signalPunzi_timeErr     = m_dataw_sig->createHistogram("TH_signalPunzi_timeErr",*m_timeErr,50);
//    TH_backgroundPunzi_timeErr = m_dataw_bck->createHistogram("TH_backgroundPunzi_timeErr",*m_timeErr,50);
//    if (m_massJpsiXPDF!=NULL) TH_JpsiXPunzi_timeErr = m_dataw_jpx->createHistogram("TH_JpsiXPunzi_timeErr",m_timeErr,50);

    RooDataHist* background_dataHist = new RooDataHist("background_dataHist", "background_dataHist", *m_timeErr,*m_dataw_bck);
    backgroundPunziPDF  = new RooHistPdf ("backgroundPunziPDF" , "backgroundPunziPDF"  , *m_timeErr,*background_dataHist);

    RooDataHist* signal_dataHist = new RooDataHist(" signal_dataHist", " signal_dataHist", *m_timeErr,*m_dataw_sig);
    signalPunziPDF = new RooHistPdf("signalPunziPDF","signalPunziPDF",  *m_timeErr, *signal_dataHist);


    if (m_massJpsiXPDF!=NULL) {
        RooDataHist* jpsix_dataHist = new RooDataHist(" jpsix_dataHist", " jpsix_dataHist", *m_timeErr,*m_dataw_jpx);
        JpsiXPunziPDF = new RooHistPdf("JpsiXPunziPDF","JpsiXPunziPDF",  *m_timeErr, *signal_dataHist);

        TotPunziPDF = new RooAddPdf  ("TotPunziPDF", "total punzi pdf", RooArgList(*signalPunziPDF, *backgroundPunziPDF,*JpsiXPunziPDF),RooArgSet(*m_signalYield,*m_bkgYield,*m_JpsiXYield));
    }
    else {
        TotPunziPDF = new RooAddPdf  ("TotPunziPDF", "total punzi pdf", RooArgList(*signalPunziPDF, *backgroundPunziPDF),RooArgSet(*m_signalYield,*m_bkgYield));
    }

    TIterator* iter2 = fitargs.createIterator();
        for (RooAbsArg *a = (RooAbsArg *) iter2->Next(); a != 0; a = (RooAbsArg *) iter2->Next()) {
            RooRealVar *rrv = dynamic_cast<RooRealVar*>(a);
            if(rrv) {
                TString var_name = rrv->GetName();
                if (var_name == "m_signalYield" or var_name == "m_JpsiXYield" or var_name == "m_bkgYield") continue;
                rrv->setConstant(false);rrv->Print();
            }
        }
    delete m_dataw_sig;
    delete m_dataw_bck;
    if (m_massJpsiXPDF!=NULL) delete m_dataw_jpx;
    RooMsgService::instance().setSilentMode(false);
    return;
}

void lifetime_splot::RunSBS(){
     cout << "running RunSplot" << endl;
     RooDataHist* mass_dataHist = new RooDataHist("mass_dataHist", " mass_dataHist", *m_mass,*m_data);     
     m_totalPDF->fitTo(*mass_dataHist,Extended(),RooFit::InitialHesse(kTRUE),NumCPU(8),Timer(kTRUE));

     RooFitResult *resultSimpleMass = m_totalPDF->fitTo(*m_data,Extended(),NumCPU(8),Timer(kTRUE),Save());
     
     Double_t peakVal = 0;
     RooArgList fitargs = resultSimpleMass->floatParsFinal(); 
     TIterator* iter = fitargs.createIterator();
     for (RooAbsArg *a = (RooAbsArg *) iter->Next(); a != 0; a = (RooAbsArg *) iter->Next()) {
         RooRealVar *rrv = dynamic_cast<RooRealVar*>(a);
         if(rrv) {
            TString var_name = rrv->GetName();
            if (var_name.Contains("mean")){
                peakVal=rrv->getValV();
            }
         }
     } 

     cout << "running Sbs itself" << endl;
    cout <<"Sideband definidion"<< Form("B_mass<%f || B_mass>%f",peakVal-120, peakVal+120)<<endl;
    RooDataSet* dataSidebandBoth = (RooDataSet*)m_data->reduce(Form("B_mass<%f || B_mass>%f",peakVal-120, peakVal+120));
    dataSidebandBoth->SetName("dataSidebandBoth");

    TH_signalPunzi     = (TH1*)m_data->createHistogram("TH_signalPunzi",*m_timeErr,Binning(40,0.01, .25));
    TH_backgroundPunzi = (TH1*)dataSidebandBoth->createHistogram("TH_backgroundPunzi",*m_timeErr,Binning(40,0.01, .25));
    TH_signalPunzi->Scale(1.0/TH_signalPunzi->Integral(), "width");
    TH_backgroundPunzi->Scale(1.0/TH_backgroundPunzi->Integral(), "width");
            
    if (m_massJpsiXPDF!=NULL) {
        RooDataSet* dataSidebandLeft = (RooDataSet*)m_data->reduce(Form("B_mass<%f || B_mass>%f",peakVal-120, peakVal+120));
        dataSidebandLeft->SetName("dataSidebandLeft");

        TH_JpsiXPunzi = (TH1*)dataSidebandLeft->createHistogram("TH_JpsiXPunzi",*m_timeErr,Binning(40,0.01, .25));
        TH_JpsiXPunzi->Scale(1.0/TH_JpsiXPunzi->Integral(), "width");

        //Double_t fsig = m_signalYield->getValV()/(m_signalYield->getValV()+m_bkgYield->getValV()+m_JpsiXYield->getValV());
        Double_t fbck1 = m_bkgYield->getValV()/(m_signalYield->getValV()+m_bkgYield->getValV()+m_JpsiXYield->getValV());
        Double_t fbck2 = m_JpsiXYield->getValV()/(m_signalYield->getValV()+m_bkgYield->getValV()+m_JpsiXYield->getValV());

        TH_signalPunzi->Add(TH_backgroundPunzi,-1*fbck1);
        TH_signalPunzi->Add(TH_JpsiXPunzi,-1*fbck2);
        TH_signalPunzi->Scale(1.0/TH_signalPunzi->Integral(), "width");    
        delete dataSidebandLeft;
    }
    else{
        Double_t fsig = m_signalYield->getValV()/(m_signalYield->getValV()+m_bkgYield->getValV());
        TH_signalPunzi->Add(TH_backgroundPunzi,-1*(1-fsig));
        TH_signalPunzi->Scale(1.0/TH_signalPunzi->Integral(), "width");    
    }
    
    RooDataHist* background_dataHist = new RooDataHist("background_dataHist", "background_dataHist", *m_timeErr,TH_backgroundPunzi);
    backgroundPunziPDF  = new RooHistPdf ("backgroundPunziPDF" , "backgroundPunziPDF"  , *m_timeErr,*background_dataHist);

    RooDataHist* signal_dataHist = new RooDataHist("signal_dataHist", " signal_dataHist", *m_timeErr,TH_signalPunzi);
    signalPunziPDF = new RooHistPdf("signalPunziPDF","signalPunziPDF",  *m_timeErr, *signal_dataHist);
    
    if (m_massJpsiXPDF!=NULL) {
        RooDataHist* jpsix_dataHist = new RooDataHist(" jpsix_dataHist", " jpsix_dataHist", *m_timeErr,TH_JpsiXPunzi);
        JpsiXPunziPDF = new RooHistPdf("JpsiXPunziPDF","JpsiXPunziPDF",  *m_timeErr, *signal_dataHist);
    
        TotPunziPDF = new RooAddPdf  ("TotPunziPDF", "total punzi pdf", RooArgList(*signalPunziPDF, *backgroundPunziPDF,*JpsiXPunziPDF),RooArgSet(*m_signalYield,*m_bkgYield,*m_JpsiXYield));
    }
    else {
        TotPunziPDF = new RooAddPdf  ("TotPunziPDF", "total punzi pdf", RooArgList(*signalPunziPDF, *backgroundPunziPDF),RooArgSet(*m_signalYield,*m_bkgYield));
    }
    delete dataSidebandBoth;
    return;
}
void lifetime_splot::PlotAndSaveResults(TString FileName = "punzi_timeErr_splot.root", TString PlotSaveName = "punzi_timeErr.png"){
    RooPlot* timeErrSimpleFrame = (RooPlot*)m_timeErr->frame();
    m_data->plotOn(timeErrSimpleFrame);
    TotPunziPDF->plotOn(timeErrSimpleFrame,LineColor(kBlue));
    TotPunziPDF->plotOn(timeErrSimpleFrame,RooFit::Components("signalPunziPDF"),LineColor(kRed));
    TotPunziPDF->plotOn(timeErrSimpleFrame,RooFit::Components("backgroundPunziPDF"),LineColor(kBlack));
    if (m_massJpsiXPDF!=NULL) TotPunziPDF->plotOn(timeErrSimpleFrame,RooFit::Components("JpsiXPunziPDF"),LineColor(kGreen+1));

    RooPlot* massSimpleFrame = (RooPlot*)m_mass->frame();
    m_data->plotOn(massSimpleFrame);
    m_totalPDF->plotOn(massSimpleFrame,LineColor(kBlue));
    m_totalPDF->plotOn(massSimpleFrame,RooFit::Components(m_massSigPDF->GetName()),LineColor(kRed));
    m_totalPDF->plotOn(massSimpleFrame,RooFit::Components(m_massBckPDF->GetName()),LineColor(kBlack));
    if (m_massJpsiXPDF!=NULL) m_totalPDF->plotOn(massSimpleFrame,RooFit::Components(m_massJpsiXPDF->GetName()),LineColor(kGreen+1));

    TCanvas* cdata = new TCanvas("sPlot","sPlot demo", 1600, 1200);
    cdata->Divide(2,1);
    cdata->cd(1);
    massSimpleFrame->Draw();

    cdata->cd(2);
    timeErrSimpleFrame->Draw();

    cdata->SaveAs(PlotSaveName);

    TCanvas* c_hist = new TCanvas("c_hist","", 1600, 1200);
    c_hist->Divide(2,2);
    c_hist->cd(1);
    TH_signalPunzi->Draw("COLZ");
    c_hist->cd(2);
    TH_backgroundPunzi->Draw("COLZ");
    c_hist->cd(3);
    if (m_massJpsiXPDF)TH_JpsiXPunzi->Draw("COLZ");

    //c_hist->SaveAs("punzi_2D_hist.png");
    //c_hist->SaveAs("punzi_2D_hist.pdf");

    TFile* file_punzi = new TFile(FileName,"RECREATE") ;
    file_punzi->cd();
    TotPunziPDF       ->Write();
    signalPunziPDF    ->Write();
    backgroundPunziPDF->Write();
    if (m_massJpsiXPDF) JpsiXPunziPDF->Write();
    file_punzi->Close();
    
    return;
}

