import sys
from ROOT import RooRealVar, RooArgList
from ROOT import RooCBShape, RooGaussian, RooAddPdf, RooPolynomial, RooExponential, RooJohnson

sys.path.append('../../') 
from Naming.varDict import varDict
from Fitting import fitFunction

var_dict = varDict()

m_B = RooRealVar(var_dict["B_mass"],"m_{B} [MeV]",5150,5700)
m_B_err = RooRealVar(var_dict["B_mass"]+"_err","#sigma_{mB}",0,50)

B_mass_fit = {}

# Define the Crystal Ball function
cb_mean = RooRealVar("B_mass_cb_mean", "Mean", 5300, 5250, 5350)
# scale_factor = RooRealVar("scale_factor", "per-event error scale factor", 2.5, 0.1, 10)
# sigma = RooProduct("sigma","sigma", RooArgList(scale_factor, m_B_err))
cb_sigma = RooRealVar("B_mass_cb_sigma", "Sigma", 15, 10, 120)
cb_alpha = RooRealVar("B_mass_cb_alpha", "Alpha", 1, 0.1, 5)
cb_n = RooRealVar("B_mass_cb_n", "n", 1, 0.0001, 2)
cb = RooCBShape("CrystalBall", "Crystal Ball", m_B, cb_mean, cb_sigma, cb_alpha, cb_n)
B_mass_fit['cb'] = fitFunction(cb, RooArgList(cb_mean, cb_sigma, cb_alpha, cb_n))

#Define Gaussian
# g_scale_factor = RooRealVar("gauss_scale_factor", "per-event error scale factor", 2.5, 0.1, 10)
# g_sigma = RooProduct("gauss_sigma","sigma", RooArgList(scale_factor, m_B_err))
g1_mean = RooRealVar("B_mass_g1_mean", "Mean 1", 5300, 5250, 5350)
g1_sigma = RooRealVar("B_mass_g1_sigma", "Sigma 1", 15, 10, 120)
g1 = RooGaussian("Gaussian1", "Gauss1", m_B, g1_mean, g1_sigma)
B_mass_fit['g'] = fitFunction(g1, RooArgList(g1_mean, g1_sigma))

#Define Double Gaussian
g2_mean = RooRealVar("B_mass_g1_mean", "Mean 2", 5300, 5250, 5350)
g2_sigma = RooRealVar("B_mass_g2_sigma", "Sigma 2", 15, 5, 40)
g2 = RooGaussian("Gaussian2", "Gauss 2", m_B, g2_mean, g2_sigma)
gg_frac = RooRealVar("frac", "frac", 0.5, 0.01, 1)
dg = RooAddPdf("DoubleGaussian","Double Gaussian",RooArgList(g1,g2),RooArgList(gg_frac),True)
B_mass_fit['dg'] = fitFunction(dg, RooArgList(g1_mean, g1_sigma, g2_mean, g2_sigma, gg_frac))

#TODO nice naming
#JOHNSON
j_lambda = RooRealVar("B_mass_johnson_lambda", "lambda",  3.0481e+01, 10, 100 )
j_gamma = RooRealVar("B_mass_johnson_gamma", "gamma", -3.1035e-01, -1.0,0.0)
j_delta = RooRealVar("B_mass_johnson_delta", "delta", 1.3739e+00, 0.3, 10)
j_mu = RooRealVar("B_mass_johnson_mu", "mu", 5.2726e+03, 5250, 5350)
johnson = RooJohnson("johnson_pdf", "Johnson PDF", m_B, j_mu, j_lambda, j_gamma, j_delta)
B_mass_fit['js'] = fitFunction(johnson, RooArgList(j_mu, j_lambda, j_gamma, j_delta))

#TODO
#GAUSS + EXP
g_mean_3 = RooRealVar("gauss_mean2", "Mean 3", 5300, 5150, 5350)
g_sigma_3 = RooRealVar("gauss_sigma2", "Sigma 3", 15, 10, 120)
g_3 = RooGaussian("g3", "Gauss3", m_B, g_mean_3, g_sigma_3)
exp_lambda = RooRealVar("exp_lambda", "exp_lambda",  -0.01, -1, 0)
exponential_pdf = RooExponential("expo", "expo", m_B, exp_lambda)
superposed_exp_gaussian = RooAddPdf("g3_expo", "Superposition of Exp and Gaussian", RooArgList(g_3, exponential_pdf), RooArgList(gg_frac), True)
B_mass_fit["expg"] = fitFunction(superposed_exp_gaussian, RooArgList(gg_frac))
