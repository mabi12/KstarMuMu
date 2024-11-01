from ROOT import RooRealVar, RooArgList, RooProdPdf, RooAbsPdf

class fitFunction():
    def __init__(self, function : RooAbsPdf, params : RooArgList):
        self.f = function
        self.name = function.GetName()
        self.ps = params        
        self.n_params = params.getSize()
        self.lps = {params.at(i).GetName(): params.at(i) for i in range(self.n_params)}
        
    def __mul__(self, other):
        fraction
        self.f = rooprod(self.f,other.f)
        self.name = self.name+other.name
        self.ps = self.ps+other.ps+[fraction]  
        self.n_params = self.ps.getSize()
        self.lps = {params.at(i).GetName(): params.at(i) for i in range(self.n_params)}
        
        pass
        # TODO Treturn fitFunction()
        