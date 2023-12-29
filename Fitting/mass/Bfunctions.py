import sys
from ROOT import RooRealVar, RooArgList
from ROOT import RooCBShape, RooGaussian, RooAddPdf

sys.path.append('../../') 
from Naming.varDict import varDict

var_dict = varDict()

m_B = RooRealVar(var_dict["B_mass"],"m_{B} [MeV]",5150,5700)
m_B_err = RooRealVar(var_dict["B_mass"]+"_err","#sigma_{mB}",0,50)
B_cos_theta_K = RooRealVar(var_dict["B_Theta_K"], "cos(#theta_{K})", -1, 1)
B_cos_theta_L = RooRealVar(var_dict["B_Theta_L"], "cos(#theta_{L})", -1, 1)
B_phi = RooRealVar(var_dict["B_Phi"], "#phi", -3.15, 3.15)

# Define the Crystal Ball function
mean = RooRealVar("mean", "Mean", 5300, 5250, 5350)

# scale_factor = RooRealVar("scale_factor", "per-event error scale factor", 2.5, 0.1, 10)
# sigma = RooProduct("sigma","sigma", RooArgList(scale_factor, m_B_err))
sigma = RooRealVar("sigma", "Sigma", 15, 10, 120)
alpha = RooRealVar("alpha", "Alpha", 1, 0.1, 5)
n = RooRealVar("n", "n", 1, 0.0001, 2)
cb = RooCBShape("cb", "Crystal Ball", m_B, mean, sigma, alpha, n)

# g_scale_factor = RooRealVar("gauss_scale_factor", "per-event error scale factor", 2.5, 0.1, 10)
# g_sigma = RooProduct("gauss_sigma","sigma", RooArgList(scale_factor, m_B_err))
g_mean = RooRealVar("gauss_mean", "Mean", 5300, 5250, 5350)
g_sigma = RooRealVar("gauss_sigma", "Sigma", 15, 10, 120)
g = RooGaussian("g", "Gauss", m_B, g_mean, g_sigma)

gg_mean = RooRealVar("ggauss_mean", "Mean", 5300, 5250, 5350)
gg_sigma = RooRealVar("ggauss_sigma", "Sigma", 15, 5, 40)
gg = RooGaussian("gG", "Gauss", m_B, g_mean, gg_sigma)

frac = RooRealVar("frac", "frac", 0.5, 0.01, 1)
frac2 = RooRealVar("frac2", "frac", 0.1, 0.001, 0.3)
f = RooAddPdf("cbg","cbg",RooArgList(cb,g),RooArgList(frac),True)
# f = RooAddPdf("cbg","cbg",RooArgList(g,gg),RooArgList(frac),True)
# f = g
# f=cb

#JOHNSON
j_lambda = RooRealVar("lambda", "lambda",  3.0481e+01, 10, 100 )
j_gamma = RooRealVar("gamma", "gamma", -3.1035e-01, -1.0,0.0)
j_delta = RooRealVar("delta", "delta", 1.3739e+00, 0.3, 10)
j_mu = RooRealVar("mu", "mu", 5.2726e+03, 5250, 5350)
johnson = RooJohnson("johnson_pdf", "Johnson PDF", m_B, j_mu, j_lambda, j_gamma, j_delta)

#GAUSS + GAUSS
g_mean_1 = RooRealVar("gauss_mean1", "Mean1", 5300, 5250, 5350)
g_sigma_1 = RooRealVar("gauss_sigma1", "Sigma1", 15, 10, 120)
g_1 = RooGaussian("g1", "Gauss1", m_B, g_mean_1, g_sigma_1)
g_mean_2 = RooRealVar("gauss_mean2", "Mean2", 5300, 5250, 5350)
g_sigma_2 = RooRealVar("gauss_sigma2", "Sigma2", 15, 10, 120)
g_2 = RooGaussian("g2", "Gauss2", m_B, g_mean_2, g_sigma_2)
superposed_gaussians = RooAddPdf("g1_g2","Superposition of Gaussians",RooArgList(g_1,g_2),RooArgList(frac),True)

#GAUSS + EXP
g_mean_3 = RooRealVar("gauss_mean2", "Mean2", 5300, 5150, 5350)
g_sigma_3 = RooRealVar("gauss_sigma2", "Sigma2", 15, 10, 120)
g_3 = RooGaussian("g3", "Gauss3", m_B, g_mean_3, g_sigma_3)
exp_lambda = RooRealVar("exp_lambda", "exp_lambda",  -0.01, -1, 0)
exponential_pdf = RooExponential("expo", "expo", m_B, exp_lambda)
superposed_exp_gaussian = RooAddPdf("g3_expo", "Superposition of Exp and Gaussian", RooArgList(g_3, exponential_pdf), RooArgList(frac), True)

#Anglefit
#POLYNOMIAL 
coef_names = ["coef_theta_K_1", "coef_theta_K_2","coef_theta_K_3", "coef_theta_L_1", "coef_theta_L_2", "coef_theta_L_3", "coef_phi_1","coef_phi_2","coef_phi_3"]
coefs_list = [RooRealVar(coef_name, coef_name, 0.1, -1, 1) for coef_name in coef_names]

coefList_cos_theta_K = RooArgList(*coefs_list[:3])
coefList_cos_theta_L = RooArgList(*coefs_list[3:6])
coefList_phi = RooArgList(*coefs_list[6:9])

poly_theta_K = RooPolynomial("poly_theta_K", "Pol3 Fit for B_Theta_K", B_cos_theta_K, coefList_cos_theta_K)
poly_theta_L = RooPolynomial("poly_theta_L", "Pol3 Fit for B_Theta_L", B_cos_theta_L, coefList_cos_theta_L)
poly_phi = RooPolynomial("poly_phi", "Polynomial Fit for B_Phi", B_phi, coefList_phi)

