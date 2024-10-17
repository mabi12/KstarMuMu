import re
def clean_filename(filename):
    return re.sub(r'[^a-zA-Z0-9_-]', '_', filename)

def extract_clean_var_name(var_full_name):
    if isinstance(var_full_name, str):
        match = re.search(r'RooRealVar::(\w+)', var_full_name)
        if match:
            return match.group(1)
    return var_full_name