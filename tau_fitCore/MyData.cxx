#include "MyData.h"

MyData::MyData(TString datasetName,TString rooDatasetFile=""){
    this->datasetName = datasetName;

    if (rooDatasetFile!="") this->rooDatasetFile = rooDatasetFile;
    
    yearsCategory = new RooCategory("yearsCategory","yearsCategory");
    yearsCategory->defineType("2015" ) ;
    yearsCategory->defineType("2016A") ;
    yearsCategory->defineType("2016B") ;
    yearsCategory->defineType("2016C") ;
    yearsCategory->defineType("2017" ) ;
    yearsCategory->defineType("2018" ) ;
    yearsCategory->defineType("run3" ) ;
}

void MyData::printCutsAndLimits(){
    std::cout<<"============  cuts  ============="<<std::endl;
    std::cout<<cuts<<std::endl;
    if(customCuts!="") std::cout<<"========= custom cuts  =========="<<std::endl<<customCuts<<std::endl;
    std::cout<<"================================="<<std::endl<<std::endl;
    
    std::cout<<"============ limits ============="<<std::endl;
    std::cout<<"mass   PDG val"<<" "<<massPDG<<std::endl;
    std::cout<<"mass    limits"<<" "<<massMin<<" "<<massMax<<std::endl;
    std::cout<<"massErr limits"<<" "<<massErrMin<<" "<<massErrMax<<std::endl;
    std::cout<<"tau     limits"<<" "<<tauMin<<" "<<tauMax<<std::endl;
    std::cout<<"tauErr  limits"<<" "<<tauErrMin<<" "<<tauErrMax<<std::endl;
    std::cout<<"pt      limits"<<" "<<ptMin<<" "<<ptMax<<std::endl;
    std::cout<<"w       limits"<<" "<<wMin<<" "<<wMax<<std::endl;
    std::cout<<"================================="<<std::endl<<std::endl;
    
}

void MyData::useDefaultCutsAndLimitsBd(bool print=false){
    cuts =  "(";
//     cuts +=  "( ( abs(B_mu1_eta)<=1.05 && abs(B_mu2_eta)<=1.05 ) && ( B_Jpsi_mass>2959-100 && B_Jpsi_mass<3229+100 ) )";  //BB
//             + "||";
    cuts += "( ( abs(B_mu1_eta)>1.05 && abs(B_mu1_eta)<=2.5 && abs(B_mu2_eta)>1.05 && abs(B_mu2_eta)<=2.5 ) && ( B_Jpsi_mass>2852-100 && B_Jpsi_mass<3332+100 ) )"; //EE
//             + "||";
//     cuts += "( ( ( abs(B_mu1_eta)<=1.05 && abs(B_mu2_eta)>1.05 && abs(B_mu2_eta)<=2.5 ) || ( abs(B_mu1_eta)>1.05 && abs(B_mu1_eta)<=2.5 && abs(B_mu2_eta)<=1.05 ) ) && ( B_Jpsi_mass>2913-100 && B_Jpsi_mass<3273+100 ) ) "; //EB
    cuts += ")";
    
    massPDG = 5279.65;
    massMin = 5000;
    massMax = 5650;
    tauMin = -2;
    tauMax = 23;
    tauErrMin = .01;
    tauErrMax = .25;
    massErrMin = 0.1;
    massErrMax = 150;
    ptMin = 10;
    ptMax = 150;
    wMin = 0;
    wMax = 100;
    if (print) printCutsAndLimits();
}

void MyData::printTriggers(){
    std::cout<<"============ triggers ==========="<<std::endl;
    std::cout<<"2015  "<<std::endl<<triggers2015<<std::endl;
    std::cout<<"2016A "<<std::endl<<triggers2016A<<std::endl;
    std::cout<<"2016B "<<std::endl<<triggers2016B<<std::endl;
    std::cout<<"2016C "<<std::endl<<triggers2016C<<std::endl;
    std::cout<<"2017  "<<std::endl<<triggers2017<<std::endl;
    std::cout<<"2018  "<<std::endl<<triggers2018<<std::endl;
    std::cout<<"================================"<<std::endl<<std::endl;
}

void MyData::useDefaultTriggersBd(bool print=false){
    triggers2015 = "(HLT_2mu4_bJpsimumu_noL2 == 1 || HLT_mu6_mu4_bBmumuxv2 == 1 || HLT_mu18_2mu0noL1_JpsimumuFS == 1)";
    triggers2016A= "((HLT_2mu6_bBmumuxv2 == 1) || (HLT_mu6_mu4_bBmumuxv2 == 1) || (HLT_mu10_mu6_bBmumuxv2 == 1))";
    triggers2016B= "((HLT_2mu6_bBmumuxv2 == 0) && (HLT_mu6_mu4_bBmumuxv2 == 0) && (HLT_mu10_mu6_bBmumuxv2 == 0)) && (HLT_mu20_2mu0noL1_JpsimumuFS == 1 || HLT_mu10_mu6_bJpsimumu == 1 || HLT_mu6_mu4_bJpsimumu == 1)";
    triggers2016C= "(HLT_2mu6_bJpsimumu_delayed == 1 || HLT_2mu6_bBmumuxv2_delayed == 1 || HLT_mu6_mu4_bBmumuxv2_delayed==1 || HLT_2mu4_bJpsimumu_L1BPH_2M8_2MU4 == 1 || HLT_mu6_mu4_bJpsimumu_delayed == 1 || HLT_2mu6_bBmumuxv2 == 1 || HLT_mu6_mu4_bJpsimumu == 1 || HLT_mu20_2mu4_JpsimumuL2 == 1)";
    triggers2017=  "(HLT_2mu6_bJpsimumu_L1BPH_2M9_2MU6_BPH_2DR15_2MU6 == 1 || HLT_2mu6_bBmumuxv2_L1LFV_MU6 == 1 || HLT_mu11_mu6_bDimu == 1 || HLT_mu11_mu6_bBmumuxv2 ==1 || HLT_mu20_2mu2noL1_JpsimumuFS == 1 || HLT_mu6_mu4_bBmumuxv2 == 1 || HLT_2mu14 == 1 || HLT_mu50 == 1)";
    triggers2018=  "(HLT_2mu6_bJpsimumu_L1BPH_2M9_2MU6_BPH_2DR15_2MU6 == 1 || HLT_2mu6_bBmumuxv2_L1LFV_MU6 == 1 || HLT_mu11_mu6_bDimu == 1 || HLT_mu11_mu6_bBmumuxv2 == 1 || HLT_mu20_2mu2noL1_JpsimumuFS == 1 || HLT_mu6_mu4_bBmumuxv2 == 1 || HLT_2mu14 == 1 || HLT_mu50 == 1 || HLT_mu22_mu8noL1 == 1 || HLT_mu20_2mu4_JpsimumuL2 == 1)";
    if (print) printTriggers();
}

void MyData::printLifetimeCorrections(){
    std::cout<<"========== tau weights =========="<<std::endl;
    std::cout<<"2015  "<<std::endl<<tauCorr2015<<std::endl;
    std::cout<<"2016A "<<std::endl<<tauCorr2016A<<std::endl;
    std::cout<<"2016B "<<std::endl<<tauCorr2016B<<std::endl;
    std::cout<<"2016C "<<std::endl<<tauCorr2016C<<std::endl;
    std::cout<<"2017  "<<std::endl<<tauCorr2017<<std::endl;
    std::cout<<"2018  "<<std::endl<<tauCorr2018<<std::endl;
    std::cout<<"================================"<<std::endl<<std::endl;
}

void MyData::useDefaultLifetimeCorrectionsBd(bool print=false){
    //older config
    //tauCorr2015 = "(1./(0.111627*(1-0.604399*(TMath::Erf((@0-28.902776)/14.511842)+1))))/8.984731";
    //tauCorr2016A= "(1./(0.092605*(1-0.393762*(TMath::Erf((@0-9.786387)/7.386000)+1))))/11.064105";
    //tauCorr2016B= "(1./(0.028958*(1-0.447239*(TMath::Erf((@0--35.324645)/8.216771)+1))))/327.258261";
    //tauCorr2016C= "(1./(0.102441*(1-0.639164*(TMath::Erf((@0-29.658980)/15.557181)+1))))/9.805719";
    //tauCorr2017 = "(1./(0.097549*(1-0.800386*(TMath::Erf((@0-32.691955)/19.259848)+1))))/10.387425";
    //tauCorr2018 = "(1./(0.096281*(1-0.499653*(TMath::Erf((@0-25.367223)/13.913118)+1))))/10.438052";


    tauCorr2015 = "(1./(0.397365*(1-0.103693*(TMath::Erf((@0-19.255467)/9.637173)+1))))/2.517812";
    tauCorr2016A= "(1./(0.092605*(1-0.393762*(TMath::Erf((@0-9.786387)/7.386000)+1))))/11.064105";
    tauCorr2016B= "(1./(0.050004*(1-0.406059*(TMath::Erf((@0--13.049108)/2.998047)+1))))/106.442527";
    tauCorr2016C= "(1./(0.364791*(1-0.111919*(TMath::Erf((@0-18.963050)/10.014288)+1))))/2.743572";
    tauCorr2017= "(1./(0.347183*(1-0.711833*(TMath::Erf((@0-40.179907)/24.283921)+1))))/2.920417";
    tauCorr2018 = "(1./(0.343664*(1-0.276886*(TMath::Erf((@0-26.416922)/16.111214)+1))))/2.926348";


    tauCorrRun3 = "(1./(0.059553*(0.387097*(TMath::Erf((@0+7.066938)/1.750629)+5))))/7.229732 + (1./(0.138681*(1-0.454835*(TMath::Erf((@0-13.921771)/5.289115)+1))))/7.229732";
    if (print) printLifetimeCorrections();
}

RooCategory* MyData::getYearsCategory(){
    return this->yearsCategory;
}

RooDataSet * MyData::readPeriod(TTree* tree, TString year, bool useTriggers, bool useLifetimeCorrections){
    cout << "Loading "<< year << " ..." << std::endl;
    
    RooRealVar *mass = new RooRealVar("B_mass", "B_mass", massMin, massMax);
    RooRealVar *time = new RooRealVar("B_tau_MinA0", "B_tau_MinA0", tauMin, tauMax);
    RooRealVar *massErr = new RooRealVar("B_mass_err", "B_mass_err", massErrMin, massErrMax);
    RooRealVar *timeErr = new RooRealVar("B_tau_MinA0_err", "B_tau_MinA0_err", tauErrMin, tauErrMax);
    RooRealVar *pt = new RooRealVar("B_pT", "B_pT", ptMin, ptMax);
//added jpsi mass
    RooRealVar *jpsi_mass = new RooRealVar("B_Jpsi_mass", "B_Jpsi_mass", 2852-100, 3332+100);

    TString newCuts = cuts;
    newCuts += Form("&& (B_tau_MinA0>%f && B_tau_MinA0<%f)",tauMin,tauMax);
    newCuts += Form("&& (B_tau_MinA0_err>%f && B_tau_MinA0_err<%f)",tauErrMin,tauErrMax);
    newCuts += Form("&& (B_mass>%f && B_mass<%f)",massMin,massMax);
    newCuts += Form("&& (B_mass_err>%f && B_mass_err<%f)",massErrMin,massErrMax);
    newCuts += Form("&& (B_pT>%f && B_pT<%f)",ptMin*1000.0,ptMax*1000.0);
    newCuts += "&& (B_mu1_pT>4000 && B_mu2_pT>4000)";
    //


    if(year != "run3") newCuts += "&& pass_GRL";
    else std::cout<<"no good run list for run3"<<std::endl;
    
    if(customCuts!=""){
        newCuts += "&&"+customCuts;
    }

    if(year == "2015"){
        newCuts += "&& (run_number<284700)";
        yearsCategory->setLabel("2015");
    }  else if(year == "2016A"){
        newCuts += "&& (run_number<302737 && run_number >= 296400)";
        yearsCategory->setLabel("2016A");
    } else if(year == "2016B"){
        newCuts += " && (run_number<302737 && run_number >= 296400)";
        yearsCategory->setLabel("2016B");
    } else if(year == "2016C"){
         newCuts += " && (run_number>=302737 && run_number<=311481)";
         yearsCategory->setLabel("2016C");
    } else if(year == "2017"){
            newCuts += "&& (run_number>=324320 && run_number < 341649)";
            yearsCategory->setLabel("2017");
    }  else if(year == "2018"){
            newCuts += "&& (run_number>=348197)";
            yearsCategory->setLabel("2018");
    }  else if(year == "run3"){
        yearsCategory->setLabel("run3");
    }
    if(useTriggers){
        if(year == "2015"){
            newCuts += "&& "+this->triggers2015;
        } else if(year == "2016A"){
             newCuts += "&& "+this->triggers2016A;
        } else if(year == "2016B"){
             newCuts += "&& "+this->triggers2016B;
        } else if(year == "2016C"){
             newCuts += " && "+this->triggers2016C;
        } else if(year == "2017"){
          newCuts += "&& "+this->triggers2017;
        }  else if(year == "2018"){
           newCuts += "&& "+this->triggers2018;
        }else if(year == "run3"){
            std::cout<<"no trigger list for run3"<<std::endl;
            //newCuts += "&& "+this->triggers2018;
        }
    }
    cout << "Cuts used: "<< newCuts <<std::endl;

    Long64_t nEvents = tree->Draw("1", newCuts, "goff");
    tree->SetEstimate(nEvents);
    tree->Draw("B_mass:B_tau_MinA0:B_mass_err:B_tau_MinA0_err:B_pT/1000:B_Jpsi_mass", newCuts, "para goff");

    RooAbsData::setDefaultStorageType(RooAbsData::Tree);
    RooArgSet*  vars = new RooArgSet(*mass, *time, *massErr, *timeErr, *pt, *jpsi_mass, *yearsCategory);
    RooDataSet* data = new RooDataSet("data", newCuts, *vars);
    TIterator *vars_it = vars->createIterator();
    for ( Long64_t i = 0; i < nEvents; i++ ) {
        vars_it->Reset();
        Int_t j = 0;
        for ( auto var = (RooRealVar*)vars_it->Next(); var != NULL ; var = (RooRealVar*)vars_it->Next() ) {
            if(j!=vars->getSize()-1)var->setVal(tree->GetVal(j)[i]); //             cout << j<<" "<<var->GetName()<<" "<< tree->GetVal(j)[i] <<" " ;
            j++;
        }

        data->add(*vars);
    }
        data->Print("v");

//add weights
    if (useLifetimeCorrections){
        TString tauWeight;
        if(year == "2015"){ tauWeight = this->tauCorr2015;
            } else if(year == "2016A"){tauWeight = this->tauCorr2016A;
            } else if(year == "2016B"){tauWeight = this->tauCorr2016B;
            } else if(year == "2016C"){tauWeight = this->tauCorr2016C;
            } else if(year == "2017"){tauWeight = this->tauCorr2017;
            }  else if(year == "2018"){tauWeight = this->tauCorr2018;
            }  else if(year == "run3"){tauWeight = this->tauCorrRun3;
            }

            RooFormulaVar wFunc("lifetimeCorrection","lifetimeCorrection",tauWeight,*time) ;
            RooRealVar* w = (RooRealVar*) data->addColumn(wFunc) ;
    }    
    //cout << year<<": Total number of events:" <<data->numEntries() <<std::endl;
    data->Print("v");
    return data;
}

RooDataSet * MyData::createData(TString treePath , bool useTriggers, bool useLifetimeCorrections, TString year =""){
    TChain* tree = new TChain(this->mainTree);
    TChain* treeTrig = new TChain(this->trigTree);
    tree->Add(treePath);
    treeTrig->Add(treePath);
    tree->AddFriend(treeTrig);
    
    RooAbsData::setDefaultStorageType(RooAbsData::Tree);
    RooDataSet* data = NULL;
    RooDataSet* dataTmp[6];
    if (useTriggers) cout << "Used trigger selection "<<std::endl;
    if (year==""){
        dataTmp[0]=readPeriod(tree, "2015",  useTriggers, useLifetimeCorrections);
        dataTmp[1]=readPeriod(tree, "2016A", useTriggers, useLifetimeCorrections);
        dataTmp[2]=readPeriod(tree, "2016B", useTriggers, useLifetimeCorrections);
        dataTmp[3]=readPeriod(tree, "2016C", useTriggers, useLifetimeCorrections);
        dataTmp[4]=readPeriod(tree, "2017",  useTriggers, useLifetimeCorrections);
        dataTmp[5]=readPeriod(tree, "2018",  useTriggers, useLifetimeCorrections);
    
        data = dataTmp[0];
        data->append(*dataTmp[1]);
        data->append(*dataTmp[2]);
        data->append(*dataTmp[3]);
        data->append(*dataTmp[4]);
        data->append(*dataTmp[5]);
        
    } else{
        data = readPeriod(tree, year, useTriggers,useLifetimeCorrections);
    }
    
    TFile* f = new TFile(this->rooDatasetFile,"RECREATE") ;
    data->Write();
    f->Close();

   return data;
}

RooDataSet* MyData::readData(TString year="",TString weightName=""){
    TFile* f = new TFile(this->rooDatasetFile,"READ");
    RooDataSet* dataTmp = (RooDataSet*)f->Get(this->datasetName);
    RooDataSet* data = NULL;
    RooDataSet* wdata = NULL;
//     data = dataTmp;
    if(this->years.find(year) != this->years.end()){
        data= (RooDataSet*) dataTmp->reduce(Form("yearsCategory==yearsCategory::%s",year.Data())) ;
    }else if(year == "2016"){
        data= (RooDataSet*) dataTmp->reduce("yearsCategory==yearsCategory::2016A || yearsCategory==yearsCategory::2016C") ;
    }else if(year == "run3"){
        data= dataTmp ;
    }
    else{ data = dataTmp;
//         data = (RooDataSet*) dataTmp->reduce("yearsCategory==yearsCategory::2015 || yearsCategory==yearsCategory::2016A || yearsCategory==yearsCategory::2016C || yearsCategory==yearsCategory::2017 || yearsCategory==yearsCategory::2018") ;
    }
//     delete dataTmp;
//    f->Close();
    if (weightName=="") {
        std::cout<<"loaded dataset "<<data->GetName()<<", "<<"no weights applied"<<std::endl;
    }else{
        std::cout<<"loaded dataset "<<data->GetName()<<", "<<weightName<<" weight applied"<<std::endl;
        wdata = new RooDataSet(data->GetName(), data->GetTitle(), data,*data->get(), 0, weightName) ;
        cout<<"data: "<<data->sumEntries()<<" ,wdata: " <<wdata->isWeighted()<<" " <<wdata->sumEntries()<<endl;
        wdata->printValue(std::cout);
        data = wdata;
    }
    
//     data->Print("v");
    
    return data;
}

void MyData::save(RooAbsData *data , TString fileName =""){
    if (fileName=="") fileName = (TString)this->rooDatasetFile;
    TFile* f = new TFile(fileName,"RECREATE") ;
    data->Write();
    f->Close();
    
    data->Print("v");    
    int N = data->numEntries();
    std::cout<<N<<std::endl;
}

void MyData::sample(RooAbsData *data, RooArgSet *vars, int sample){
    //rewrite data with sampled dataset
    if (sample > 0) {

        Long64_t nOrig = data->numEntries();
        TString title = data->GetTitle(); //TODO " Random sample ( " + sample + " / " + nOrig + " )";
        RooDataSet *newDataset = new RooDataSet("dataRandomSample", title, *vars);
        TRandom3 rand(0);
        std::cout << "INFO: sampling "<<sample<<" events"<< std::endl; //TODO sample
        int printEveryNth = 1000;
        for ( Long64_t i = 0; i < sample; i++ ) {
            if (i % printEveryNth == 0)    std::cout<<"\r      " <<i<<" / "<<sample<< std::flush;
            Long64_t j = rand.Uniform( 0 , nOrig );
            newDataset->add( *(data->get( j )) );
        }
        cout << "      new random sample created "<< endl; //TODO sample
        *data=*newDataset;
        
        data->Print("v");
    }
}
