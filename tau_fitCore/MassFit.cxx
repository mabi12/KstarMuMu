#include "MassFit.h"
#include "TChain.h"

// MassFit::MassFit()const char *name,const char *inFilePath){
//     this->name = name;
//     this->inFilePath = inFilePath;
//     yearsCategory = new RooCategory("yearsCategory","yearsCategory");
//     yearsCategory->defineType("2015" ) ;
//     yearsCategory->defineType("2016A") ;
//     yearsCategory->defineType("2016B") ;
//     yearsCategory->defineType("2016C") ;
//     yearsCategory->defineType("2017" ) ;
//     yearsCategory->defineType("2018" ) ;
// }


RooAbsPdf* MassFit::getMassPDF(TString sgDesc, TString bgDesc,RooRealVar *mass){
    if(sgDesc == "g"){
        mass_mean = new RooRealVar ("a_mass_mean", "Mean of Gaussian", 5282,5274, 5285);
        mass_sigma1    = new RooRealVar ("massSignal_sigma1", "Sigma1 of DoubleGaussian", 30, 20, 35);
        massSignalPDF   = new RooGaussian("massSignalPDF", "Mass Signal Gaussian 1", *mass, *mass_mean, *mass_sigma1);
    }else if(sgDesc == "dg"){
        mass_mean = new RooRealVar ("mass_mean", "Mean of Gaussian", 5279,5200, 5350);
        mass_sigma1    = new RooRealVar ("massSignal_sigma1", "Sigma1 of DoubleGaussian", 30, 1, 50);
        mass_sigma2    = new RooRealVar ("massSignal_sigma2", "Sigma2 of DoubleGaussian",40, 1, 80);
        mass_fraction  = new RooRealVar ("massSignal_fraction", "Fraction of DoubleGaussian",0.5, 0., 1.);
        RooAbsPdf *mass_gauss1   = new RooGaussian("massSignal_gauss1", "Gaussian 1", *mass, *mass_mean, *mass_sigma1);
        RooAbsPdf *mass_gauss2   = new RooGaussian("massSignal_gauss2", "Gaussian 2", *mass, *mass_mean, *mass_sigma2);
        massSignalPDF = new RooAddPdf  ("massSignalPDF", "Mass Signal DoubleGaussian", RooArgList(*mass_gauss1, *mass_gauss2), *mass_fraction, kTRUE);
    }else if(sgDesc == "jj" || sgDesc == "jjfm"){
        mass_mean = new RooRealVar ("massSg_mean", "Mean of Gaussian", 5.2726e+03,5200, 5350);
        mass_gamma  = new RooRealVar ("massSignal_gamma", "", -3.1035e-01,-1.0,0.0);
        mass_delta  = new RooRealVar ("massSignal_delta", "", 1.3739e+00,0.3,10);
        mass_lambda = new RooRealVar ("massSignal_lambda", "", 3.0481e+01,10,100);
        
        if(sgDesc == "jjfm"){
            mass_mean2 = mass_mean;
        }else{
            mass_mean2 = new RooRealVar ("massSg_mean2", "Mean of Gaussian", 5.2726e+03,5200, 5350);
        }
        mass_gamma2  = new RooRealVar ("massSignal_gamma2", "", 1.9883); //,0.5,10.0);
        mass_delta2  = new RooRealVar ("massSignal_delta2", "", 3.2868e+00,0.5,10);
        mass_lambda2 = new RooRealVar ("massSignal_lambda2", "", 7.3394e+01,10,200);

        RooJohnson *mass_johnson1 = new  RooJohnson("massSignal_johnson1", "Johnson Signal Decription", *mass, *mass_mean, *mass_lambda, *mass_gamma, *mass_delta);
        RooJohnson *mass_johnson2 = new  RooJohnson("massSignal_johnson2", "Johnson Signal Decription", *mass, *mass_mean2, *mass_lambda2, *mass_gamma2, *mass_delta2);
        mass_fraction  = new RooRealVar ("massSignal_fraction", "Fraction of two Johnson functions",9.4603e-01, 0., 1.);

        massSignalPDF = new RooAddPdf("massSignalPDF","Double Johnson common mean",RooArgList(*mass_johnson1,*mass_johnson2),*mass_fraction, kTRUE);

    } else if(sgDesc == "j" ){
        mass_mean = new RooRealVar ("massSg_mean", "Mean of Gaussian", 5.2726e+03,5200, 5350);
        mass_gamma  = new RooRealVar ("massSignal_gamma", "", -3.1035e-01,-1.0,0.0);
        mass_delta  = new RooRealVar ("massSignal_delta", "", 1.3739e+00,0.3,10);
        mass_lambda = new RooRealVar ("massSignal_lambda", "", 3.0481e+01,10,100);
        
        massSignalPDF = new  RooJohnson("massSignalPDF", "Johnson Signal Decription", *mass, *mass_mean, *mass_lambda, *mass_gamma, *mass_delta);
       

    }else{
        cout << "Wrong signal description for mass:"<< sgDesc <<endl;
//         throw invalid_argument("Wrong  signal description for mass: " + sgDesc);

    }
    RooPolynomial* mass_const;
    RooAbsPdf *massBckPDF_combinatorial;
    if(bgDesc == "exp"){
        mass_slope1     = new RooRealVar    ("mass_slope1", "Slope of Exponential1",-1.4308e-03, -0.1, -1e-8);
        massBckPDF = new RooExponential("massBckPDF", "mass_exponential1", *mass, *mass_slope1);

    }else if(bgDesc == "expexp"){
        mass_slope1     = new RooRealVar    ("mass_slope1", "Slope of Exponential1",-6.83178e-04, -0.1, -1e-8);
        mass_slope2     = new RooRealVar    ("mass_slope2", "Slope of Exponential2",-9.83178e-04, -0.1, -1e-8);
        RooExponential *mass_exponential1 = new RooExponential("mass_exponential1", "mass_exponential1", *mass, *mass_slope1);
        RooExponential *mass_exponential2 = new RooExponential("mass_exponential2", "mass_exponential2", *mass, *mass_slope2);
        massBck_fraction  = new RooRealVar ("massBck_fraction", "Fraction of Double Exponential",0.5, 0., 1.);
    
        massBckPDF = new RooAddPdf  ("massBckPDF", "Mass Background Double Exponential", RooArgList(*mass_exponential1, *mass_exponential2),*massBck_fraction, kTRUE);
    }else if(bgDesc == "pol0expexp"){
        mass_slope1     = new RooRealVar    ("massBck_slope1", "Slope of Exponential1",-6.83178e-04, -0.1, -1e-8);
        mass_slope2     = new RooRealVar    ("massBck_slope2", "Slope of Exponential2",-9.83178e-04, -0.1, -1e-8);
        RooExponential *mass_exponential1 = new RooExponential("mass_exponential1", "mass_exponential1", *mass, *mass_slope1);
        RooExponential *mass_exponential2 = new RooExponential("mass_exponential2", "mass_exponential2", *mass, *mass_slope2);
        massBck_fraction  = new RooRealVar ("massBck_fraction", "Fraction of DoubleGaussian",0.5, 0., 1.);
        massBck_fraction2  = new RooRealVar ("massBck_fraction2", "Fraction of DoubleGaussian",0.6, 0., 1.);
        
        mass_const = new RooPolynomial("massBck_pol","mass_pol",*mass,RooArgList());
//         massBckPDF = new RooAddPdf  ("massBckPDF", "Mass Background DoubleGaussian", RooArgList(*mass_const, *mass_exponential1, *mass_exponential2),RooArgList(*massBck_fraction,*massBck_fraction2), kTRUE);

        massBckPDF_combinatorial = new RooAddPdf  ("massBckPDF_combinatorial", "Mass Background DoubleGaussian", RooArgList(*mass_exponential1, *mass_exponential2),RooArgList(*massBck_fraction), kTRUE);

//         massBckPDF = new RooAddPdf  ("massBckPDF", "Mass Background DoubleGaussian", RooArgList(*mass_const, *massBckPDF_combinatorial), RooArgList(*massBck_fraction2), kTRUE);


    }else if(bgDesc == "pol0exp"){
        mass_slope1     = new RooRealVar    ("massBck_slope1", "Slope of Exponential1",-6.83178e-04, -0.1, -1e-8);
        RooExponential *mass_exponential1 = new RooExponential("mass_exponential1", "mass_exponential1", *mass, *mass_slope1);
        massBck_fraction  = new RooRealVar ("massBck_fraction", "Fraction of DoubleGaussian",0.5, 0., 1.);
        
        mass_const = new RooPolynomial("massBck_pol","mass_pol",*mass,RooArgList());
        massBckPDF = new RooAddPdf  ("massBckPDF", "Mass Background DoubleGaussian", RooArgList(*mass_const, *mass_exponential1),*massBck_fraction, kTRUE);

    }else if(bgDesc == "pol0exp5065"){
        mass_slope1     = new RooRealVar    ("massBck_slope1", "Slope of Exponential1",-6.83178e-04, -0.1, -1e-8);
        RooExponential *mass_exponential1 = new RooExponential("mass_exponential1", "mass_exponential1", *mass, *mass_slope1);
        massBck_fraction  = new RooRealVar ("massBck_fraction", "Fraction of DoubleGaussian",0.9, 0., 0.95);
        massBck_fraction2  = new RooRealVar ("massBck_fraction2", "Fraction of DoubleGaussian",0.1, 0.05, 0.5);
        RooRealVar *massBck_mean = new RooRealVar ("massBck_5065mean", "Mean of Gaussian5065", 5065,50600, 5070);
        RooRealVar *massBck_sigma    = new RooRealVar ("massSignal_5065sigma1", "Sigma of Gaussian5065", 5, 0.5, 10);
        RooAbsPdf *massBck_gauss   = new RooGaussian("massBck5065_gauss", "Gaussian 5065", *mass, *massBck_mean, *massBck_sigma);
        
        mass_const = new RooPolynomial("mass_pol","mass_pol",*mass,RooArgList());
        massBckPDF = new RooAddPdf  ("massBckPDF", "Mass Background DoubleGaussian", RooArgList(*mass_const, *mass_exponential1, *massBck_gauss),RooArgList(*massBck_fraction,*massBck_fraction2));
        
    }else{
        cout << "Wrong backgroundModel:"<< bgDesc <<endl;
//         return; 
    }
    //Total
    //
    RooRealVar *mass_sigFrac  = new RooRealVar ("mass_sigFrac", "mass_sigFrac",0.22, 0.1, 0.96);    //2.0426e-01
    RooAddPdf* massPDF = new RooAddPdf  ("massPDF", "massPDF", RooArgList(*massSignalPDF, *mass_const, *massBckPDF_combinatorial), RooArgList(*mass_sigFrac, *massBck_fraction2), kTRUE );
    
    return massPDF;
}
