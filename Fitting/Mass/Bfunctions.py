import sys
from ROOT import RooRealVar, RooArgList
from ROOT import RooCBShape, RooGaussian, RooAddPdf, RooPolynomial, RooExponential, RooJohnson

sys.path.append('../../') 
from Naming.varDict import varDict
from Fitting.fitFunction import fitFunction
class BFit():
    def __init__(self, B_mass_range : list,  B_mass_err_range : list):
        var_dict = varDict()

        self.m_B = RooRealVar(var_dict["B_mass"],"m_{B} [MeV]",B_mass_range[0],B_mass_range[1])
        self.m_B_err = RooRealVar(var_dict["B_mass"]+"_err","#sigma_{mB}",B_mass_err_range[0], B_mass_err_range[1])
        self.frac = RooRealVar("frac", "frac", 0.5, 0.01, 1)

        self.B_mass_fit = {}

    def define_fit_functions(self):
        # Define the Crystal Ball function
        cb_mean = RooRealVar("B_mass_cb_mean", "Mean", 5300, 5250, 5350)
        # scale_factor = RooRealVar("scale_factor", "per-event error scale factor", 2.5, 0.1, 10)
        # sigma = RooProduct("sigma","sigma", RooArgList(scale_factor, m_B_err))
        cb_sigma = RooRealVar("B_mass_cb_sigma", "Sigma", 15, 10, 120)
        cb_alpha = RooRealVar("B_mass_cb_alpha", "Alpha", 1, 0.1, 5)
        cb_n = RooRealVar("B_mass_cb_n", "n", 1, 0.0001, 2)
        bg_exp_lambda_cb = RooRealVar("exp_lambda", "exp_lambda",  -0.01, -1, 0)
        bg_exp_cb= RooExponential("bg_exp", "bg_expo", self.m_B, bg_exp_lambda_cb)
        cb = RooCBShape("CrystalBall", "Crystal Ball",self.m_B, cb_mean, cb_sigma, cb_alpha, cb_n)
        bg_sg_exp_cb = RooAddPdf("bg_sg_exp_cb", "Superposition of Exp and Crystal Ball (bg + sg)", RooArgList(cb, bg_exp_cb), RooArgList(self.frac), True)
        self.B_mass_fit['cb'] = fitFunction(cb, RooArgList(cb_mean, cb_sigma, cb_alpha, cb_n, self.frac))

        #Define Gaussian
        # g_scale_factor = RooRealVar("gauss_scale_factor", "per-event error scale factor", 2.5, 0.1, 10)
        # g_sigma = RooProduct("gauss_sigma","sigma", RooArgList(scale_factor, m_B_err))
        g1_mean = RooRealVar("B_mass_g1_mean", "Mean 1", 5280, 5000, 5700)
        g1_sigma = RooRealVar("B_mass_g1_sigma", "Sigma 1", 50, 0.1,135)
        #bg_exp_lambda_g1 = RooRealVar("exp_lambda", "exp_lambda",  -0.01, -1, 0)
        #bg_exp_g1 = RooExponential("bg_exp", "bg_expo", m_B, bg_exp_lambda_g1)
        g1 = RooGaussian("Gaussian1", "Gauss1", self.m_B, g1_mean, g1_sigma)
        #bg_sg_exp_g1 = RooAddPdf("bg_sg_exp_gauss", "Superposition of Exp and Gaussian (bg + sg)", RooArgList(g1,bg_exp_g1), RooArgList(gg_frac), True) 
        self.B_mass_fit['g'] = fitFunction(g1, RooArgList(g1_mean, g1_sigma))

        #Define Double Gaussian
        g2_mean = RooRealVar("B_mass_g2_mean", "Mean 2", 5280, 5000, 5000)
        g2_sigma = RooRealVar("B_mass_g2_sigma", "Sigma 2", 100, 0.1, 120)
        g2 = RooGaussian("Gaussian2", "Gauss 2", self.m_B, g2_mean, g2_sigma)
        #bg_exp_lambda_dg = RooRealVar("exp_lambda", "exp_lambda",  -0.01, -1, 0)
        #bg_exp_dg = RooExponential("bg_exp", "bg_expo", m_B, bg_exp_lambda_dg)
        dg = RooAddPdf("DoubleGaussian","Double Gaussian",RooArgList(g1,g2),RooArgList(self.frac),True)
        #bg_sg_dg = RooAddPdf("bg_sg_Doublegaussian", "Superposition of Double Gaussian and Exp (bg + sg)")
        self.B_mass_fit['dg'] = fitFunction(dg, RooArgList(g1_mean, g1_sigma, g2_mean, g2_sigma, self.frac))
        
        #TODO nice naming
        #JOHNSON
        j_lambda = RooRealVar("B_mass_johnson_lambda", "lambda",  3.0481e+01, 0.4, 40)
        j_gamma = RooRealVar("B_mass_johnson_gamma", "gamma", 0, -1,1)
        j_delta = RooRealVar("B_mass_johnson_delta", "delta", 0.55, 0.05, 30)
        j_mu = RooRealVar("B_mass_johnson_mu", "mu", 5.2726e+03, 5000, 5700)
        johnson = RooJohnson("johnson_pdf", "Johnson PDF", self.m_B, j_mu, j_lambda, j_gamma, j_delta)
        #bg_exp_lambda_johnson = RooRealVar("exp_lambda", "exp_lambda",  -0.01, -1, 0)
        #bg_exp_johnson =  RooExponential("bg_exp", "bg_expo", m_B, bg_exp_lambda_johnson)
        #bg_sg_johnson_exp = RooAddPdf("bg_sg_johnson_exp", "Superposition of Johnson and Exp (bg + sg)", RooArgList(johnson, bg_exp_johnson), RooArgList(gg_frac), True)
        self.B_mass_fit['john'] = fitFunction(johnson, RooArgList(j_mu, j_lambda, j_gamma, j_delta))

        #TODO
        #GAUSS + EXP
        g_mean_3 = RooRealVar("gauss_mean3", "Mean 3", 5300, 5150, 5350)
        g_sigma_3 = RooRealVar("gauss_sigma3", "Sigma 3", 15, 10, 120)
        g_3 = RooGaussian("g3", "Gauss3", self.m_B, g_mean_3, g_sigma_3)
        sg_exp_lambda = RooRealVar("exp_lambda", "exp_lambda",  -0.01, -1, 0)
        sg_exp = RooExponential("expo", "expo", self.m_B, sg_exp_lambda)
        bg_exp_lambda_gaussian_2 = RooRealVar("exp_lambda", "exp_lambda",  -0.01, -1, 0)
        bg_exp_gaussian_2 = RooExponential("expo", "expo", self.m_B, bg_exp_lambda_gaussian_2)
        superposed_exp_gaussian = RooAddPdf("g3_exp", "Superposition of Exp and Gaussian", RooArgList(g_3, sg_exp), RooArgList(self.frac), True)
        bg_sg_superposed_exp_gaussian = RooAddPdf("bg_sg_g3_exp", "Superposition of Exp and Exp + Gaussian (bg + sg)", RooArgList(superposed_exp_gaussian, bg_exp_gaussian_2,), RooArgList(self.frac), True)
        self.B_mass_fit["expg"] = fitFunction(bg_sg_superposed_exp_gaussian, RooArgList(g_mean_3, g_sigma_3, sg_exp_lambda, bg_exp_lambda_gaussian_2, self.frac))

    def get_fit_function(self, tag:str):
        mass_fit_func = self.B_mass_fit.get(tag)
        if not mass_fit_func:
            raise ValueError(f"B mass fit function '{tag} not defined'")
        
        return mass_fit_func.f



