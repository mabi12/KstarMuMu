#ifndef MASSFIT
#define MASSFIT

class MassFit{
public:
//     MassFit(const char *name,const char *inFilePath);
//     ~MassFit();
    RooAbsPdf* getMassPDF(TString sgDesc, TString bgDesc,RooRealVar *mass);
    RooAbsPdf* massSignalPDF;
    RooRealVar *mass_mean, *mass_sigma1, *mass_sigma2, *mass_fraction,*mass_gamma,*mass_delta,*mass_lambda,*mass_mean2, *mass_gamma2,*mass_delta2, *mass_lambda2; 
    RooAbsPdf *massBckPDF;
    RooRealVar *mass_slope1,*mass_slope2, *massBck_fraction, *massBck_fraction2;
};
#endif
