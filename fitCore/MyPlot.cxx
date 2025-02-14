#include "MyPlot.h"
using namespace RooFit;

MyPlot::MyPlot(RooRealVar *var, int nBins , TString outDir , TString tagATLAS ){
    this->name = var->GetName();
    this->var=var;
    this->nBins=nBins;
    this->outDir=outDir;
    this->tagATLAS=tagATLAS;

    this->c = new TCanvas("c_"+this->name,"",800,800);
    this->pads[0] = new TPad("pad0", "", 0., 0.25, 1., 1.0);
    this->pads[1] = new TPad("pad1", "", 0., 0., 1., 0.25);

    for (auto pad:pads)pad->Draw();
    std::cout<<"VarAndPull instance created"<<std::endl;

    SetAtlasStyle();
    gStyle->SetPadBorderMode(0);
    gStyle->SetFrameBorderMode(0);

}

RooPlot *MyPlot::plotVarAndPull(RooAbsData* data, RooAbsPdf*fit, int nParams, TString outName){
    bool useWeight = false;
    TString data_legend = "";
    //TODO debug components  //     TString components[];
    RooPlot * massFrame    = (RooPlot*)var->frame();
    
    if (useWeight) data->plotOn(massFrame,DataError(RooAbsData::SumW2), RooFit::Name("data"),Binning(nBins));
    else data->plotOn(massFrame, RooFit::Name("data"),Binning(nBins)); 
    
//     int arrSize = *(&components + 1) - components;
//     cout<<arrSize<<endl;
//     for (int i = 0; i<arrSize; ++i)fit->plotOn(massFrame,RooFit::Components(components[i]),LineColor(i+1));
    // fit->plotOn(massFrame, RooFit::Components("massBckPDF_combinatorial"),LineColor(1),LineStyle(2),Name("combinatorialBackground"));
    // fit->plotOn(massFrame, RooFit::Components("backgroundPDF_prompt"),LineColor(14),LineStyle(5),Name("prompt"));
    // fit->plotOn(massFrame,RooFit::Components("massSignalPDF"),LineColor(2),LineStyle(9),Name("signal"));
    // fit->plotOn(massFrame, RooFit::Name("fit"));
//         TLegend *leg_mass    = new TLegend(0.65,0.6,0.92,0.82);
//     leg_mass->SetBorderSize(0);  // no border
//     leg_mass->SetFillStyle(0);
//     leg_mass->SetTextFont(42);
// //     leg_mass->SetFillColor(kWhite);
// //     leg_mass->SetLineColor(kWhite);
//     leg_mass->AddEntry("data","Data", "P");
//     leg_mass->AddEntry("fit","Total Fit","L");
//     leg_mass->AddEntry("signal","Signal", "L");
//     leg_mass->AddEntry("prompt","Prompt Background", "L");
//     leg_mass->AddEntry("combinatorialBackground","Combinatorial Background", "L");
//     leg_mass->Draw();
    // if(data_legend!="")     myMarkerText( 0.64, 0.935, 1, 20, data_legend,1.3);

    
    fit->plotOn(massFrame, RooFit::Name("fit"));
    
    double chi2ndof = massFrame->chiSquare(nParams);
    std::cout << "chi/ndof = " << chi2ndof << std::endl;

    RooHist* hpullMass = massFrame->pullHist(); 
    RooPlot* massFramePull = (RooPlot*)var->frame() ;
    massFramePull->addPlotable(hpullMass,"P") ;
    
    pads[0]->cd();
    massFrame->Draw();

    ATLASLabel(0.65,0.9,tagATLAS);
    TLatex *atlas2 = new TLatex(0.65,0.85,"#scale[0.7]{#sqrt{s}=13 TeV, 140 fb^{-1}}");
    atlas2->SetNDC(kTRUE);
    atlas2->SetTextFont(42);
    atlas2->SetTextColor(kBlack);
    atlas2->SetTextSize(0.06);
    atlas2->Draw();

    TLatex *chi2Text = new TLatex(0.65, 0.8, TString::Format("#scale[0.7]{#chi^{2}/ndof = %.2f}", chi2ndof));
    chi2Text->SetNDC(kTRUE);
    chi2Text->SetTextFont(42);
    chi2Text->SetTextColor(kBlack);
    chi2Text->SetTextSize(0.06);
    chi2Text->Draw();

    if(data_legend!="")     myMarkerText( 0.64, 0.935, 1, 20, data_legend,1.3);
    massFrame->GetYaxis()->SetRangeUser(0.001,1.1*massFrame->GetMaximum());
    massFramePull->GetYaxis()->SetRangeUser(-4.9,4.9);
    massFramePull->GetYaxis()->SetNdivisions(7,2,1);
        
    pads[0]->cd();
    gPad->SetLeftMargin(0.12);
    gPad->SetRightMargin(0.04);
    gPad->SetTopMargin(0.02);
    gPad->SetBottomMargin(1e-5);
    
    pads[1]->cd();
    gPad->SetTopMargin(1e-5);
    gPad->SetLeftMargin(0.12);
    gPad->SetRightMargin(0.04);
    gPad->SetBottomMargin(0.35);
    
//     Int_t font=42; // Helvetica
//     Int_t tsize=12;
//     gStyle->SetTextFont(font);
//     gStyle->SetTextSize(tsize);
//     
//     gPad->SetTickx();
    // massFramePull->     SetXTitle("m_{B} [MeV]");
    massFramePull->GetXaxis()->SetTitleOffset(1.2);
    
//     massFramePull->GetYaxis()->SetNdivisions(205);
        
    massFramePull->Draw();

    TLine * line0 = new TLine(massFramePull-> GetXaxis()->GetXmin(),  0,  massFramePull-> GetXaxis()->GetXmax(), 0);
//     line0 -> SetLineColor(3);
    line0 -> SetLineStyle(2);
    line0 -> Draw();
        std::cout<<"save:"<<std::endl;
    c->SaveAs(outDir+outName+".pdf");
    std::cout<<"succesfully plotted"<<std::endl;
    return massFrame;
}


RooPlot *MyPlot::plotVarAndResid(RooAbsData* data, RooAbsPdf*fit, TString outName, bool useWeight, int nBins, TString components[]){//TODO debug components
    
    RooPlot * massFrame    = (RooPlot*)var->frame();
    
    if (useWeight) data->plotOn(massFrame,DataError(RooAbsData::SumW2), RooFit::Name("data"),Binning(nBins), Precision(1e-6));
    else data->plotOn(massFrame, RooFit::Name("data"),RooFit::Binning(nBins), RooFit::Precision(1e-6));
    
//     int arrSize = *(&components + 1) - components;
//     cout<<arrSize<<endl;
    //TODO for (int i = 0; i<arrSize; ++i)fit->plotOn(massFrame,RooFit::Components(components[i]),LineColor(i+1));
    
    fit->plotOn(massFrame, RooFit::Name("fit"), Precision(1e-6));
    
    RooHist* hResidMass = massFrame->residHist(); 
    RooPlot* massFrameResid = (RooPlot*)var->frame() ;
    massFrameResid->addPlotable(hResidMass,"P") ;
    
    pads[0]->cd();
    massFrame->Draw();

    ATLASLabel(0.15,0.925,"Work In Progress");
//    if(data_legend!="")     myMarkerText( 0.64, 0.935, 1, 20, data_legend,1.3);

    massFrame->GetYaxis()->SetRangeUser(0.001,1.1*massFrame->GetMaximum());
    gPad->SetLeftMargin(0.12);
    gPad->SetRightMargin(0.02);
    gPad->SetTopMargin(0.02);
    gPad->SetBottomMargin(1e-5);
    pads[0]->cd();

    pads[1]->cd();
    gPad->SetTopMargin(1e-5);
    gPad->SetLeftMargin(0.12);
    gPad->SetRightMargin(0.02);
    gPad->SetBottomMargin(0.25);

    massFrameResid->SetXTitle("m_{B} [MeV]");
    massFrameResid->Draw();
//     massFramePull->GetYaxis()->SetRangeUser(-4.9,4.9);
//     massFramePull->GetYaxis()->SetNdivisions(7,2,1);
    
    TLine * line0 = new TLine(massFrameResid-> GetXaxis()->GetXmin(),  0,  massFrameResid-> GetXaxis()->GetXmax(), 0);
//     line0 -> SetLineColor(3);
    line0 -> SetLineStyle(2);
    line0 -> Draw();

    c->SaveAs(outName+".pdf"); 
    return massFrameResid;
}

RooHist *MyPlot::getResid(RooAbsData* data, RooAbsPdf*fit, bool useWeight, int nBins){
    
    RooPlot * massFrame    = (RooPlot*)var->frame();
    if (useWeight) data->plotOn(massFrame,DataError(RooAbsData::SumW2), RooFit::Name("data"),Binning(nBins));
    else data->plotOn(massFrame, RooFit::Name("data"),Binning(nBins)); 
    fit->plotOn(massFrame, RooFit::Name("fit"));
    RooHist* hResidMass = massFrame->residHist(); 
    
    delete massFrame;
    return hResidMass;
}
