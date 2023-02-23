import sys, os 
from tqdm import tqdm
import ROOT

#hack to load from parent directory
sys.path.append('../') 
os.chdir("mcStudies")

from Naming.mcDict import mcDict
from Naming.cuts import cuts_run1, var

#turn off graphics mode
ROOT.gROOT.SetBatch(True)

path = "/eos/atlas/atlascerngroupdisk/phys-beauty/RKstarRun2/ntuples/muon_channel_v0.9/ntuples_updated/"
path = "../data/"
draw = True

mc_dict = mcDict(debugPrint=False)

def rename_htemp(name=""):
    try:
        h = ROOT.gPad.GetPrimitive("htemp")
        h.SetTitle(name)
    except:
        pass

range_ = {"BmumuKst_B_mass" : "BmumuKst_B_mass>5150 && BmumuKst_B_mass<5700",
          "BmumuKst_Bbar_mass" :"BmumuKst_Bbar_mass>5150 && BmumuKst_Bbar_mass<5700" ,
          "BmumuKst_kaonPion_mass" : "BmumuKst_kaonPion_mass > 846 && BmumuKst_kaonPion_mass < 946",
          "BmumuKst_pionKaon_mass" :  "BmumuKst_pionKaon_mass > 846 && BmumuKst_pionKaon_mass < 946",
          "BmumuKst_diMuon_mass*BmumuKst_diMuon_mass" : "BmumuKst_diMuon_mass*BmumuKst_diMuon_mass>1000000 &&  BmumuKst_diMuon_mass*BmumuKst_diMuon_mass < 6000000",
          f"{var['tau']}/{var['tau']}_err" : f"{var['tau']}/{var['tau']}_err>0 && {var['tau']}/{var['tau']}_err<30",
          "BmumuKst_chi2_over_nDoF" : "BmumuKst_chi2_over_nDoF<3",
          "BmumuKst_diMuon_chi2_over_nDoF" : "BmumuKst_diMuon_chi2_over_nDoF>0",
          "BmumuKst_muon0_pT": "BmumuKst_muon0_pT<20000",
          "BmumuKst_muon1_pT": "BmumuKst_muon1_pT<20000",
          "BmumuKst_diMuon_pT": "BmumuKst_diMuon_pT<50000"
         }

rows = len(range_)
c = ROOT.TCanvas("c","c",3*1024,rows*1024)
c.Divide(3,rows)

combinations = [
    "Bd_Kstar_mumu sg BmumuKst_isTrueNonresBd==1",
    "Bd_Kstar_mumu selfBg BmumuKst_isTrueNonresBd==0",
    "Bd_Kstar_Jpsi_mumu sgJpsi BmumuKst_isTrueResBd==1",
    "Bd_Kstar_Jpsi_mumu selfBgJpsi BmumuKst_isTrueResBd==0",
]
print(os.getcwd())
for comb in combinations:
    decay, cutFlag, cut =  comb.split()
        
    file_name = os.path.join(path,"ntuple-"+mc_dict[decay]+"_part_01.root")
    f = ROOT.TFile(file_name)
    t = f.Get("Nominal/BaseSelection_KStarMuMu_BmumuKstSelection")

    N_events = t.Draw("1","","goff")
    
    N_all = t.Draw("1","BmumuKst_B_mass>0","goff")
    N_passed = t.Draw("1",cuts_run1,"goff")

    N_cut = t.Draw("1",cut,"goff")
    N_cut_passed = t.Draw("1",cut+"&&"+cuts_run1,"goff")

    print(N_all,N_passed)
    print(N_cut,N_cut_passed)

    if draw:
        for j,v in enumerate(tqdm(range_)):
            c.cd(1+3*j)
            
            t.Draw(v,cut)
            rename_htemp(v.replace("BmumuKst_",""))
            
            c.cd(2+3*j)
            t.Draw(v,cut+" && "+range_[v])
            rename_htemp("zoom")
                
            c.cd(3+3*j)
            t.Draw(v,cut+" && "+range_[v]+" && "+cuts_run1)
            rename_htemp("RUN 1 cuts")    

        c.Draw()
        c.SaveAs(f"out/{decay}_{cutFlag}.png")
        c.SaveAs(f"out/{decay}_{cutFlag}.pdf")


    with open(f"out/{decay}_{cutFlag}.log", 'w') as f:
        f.write(f"{cutFlag} {cut} \n")
        f.write(f"events: {N_events}\n")
        f.write(f"all: {N_all}, passed run1 cuts: {N_passed}\n")
        f.write(f"selected: {N_cut}, passed run1 cuts: {N_cut_passed}\n")
        f.write(f"selected over events: {N_cut/N_events}, passed run1 cuts: {N_cut_passed/N_events}\n")
        f.write("\n")
        f.write("plotted vars in range:\n")
        for r in range_:
            f.write(f"{r} - {range_[r]} \n")
        f.write("\n")
        f.write("\n")
        f.write(f"{cuts_run1}\n")

    with open(f"out/results.log", 'w') as f:
        f.write(f"{cutFlag} {cut} \n")
        f.write(f"events: {N_events}\n")
        f.write(f"all: {N_all}, passed run1 cuts: {N_passed}\n")
        f.write(f"selected: {N_cut}, passed run1 cuts: {N_cut_passed}\n")
        f.write(f"selected over events: {N_cut/N_events}, passed run1 cuts: {N_cut_passed/N_events}\n")
        f.write("\n")
        