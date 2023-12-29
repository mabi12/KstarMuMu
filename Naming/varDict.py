def varDict(debugPrint=False):
    """
    return dictMC where 
    """
    var_dict = {}
    var_dict["B_mass"] = "BmumuKst_B_mass"
    var_dict["Bb_mass"] = "BmumuKst_Bbar_mass"
    var_dict["Kstar_mass"] = "BmumuKst_kaonPion_mass"
    var_dict["Kstarb_mass"] = "BmumuKst_pionKaon_mass"
    
    if debugPrint:
        print(5*"="+"varDict"+5*"=")
        for v in var_dict:
            print(v,":",var_dict)
        print(5*"="+7*"="+5*"=")
        
    return var_dict
