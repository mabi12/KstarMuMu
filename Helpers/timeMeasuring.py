from time import time

class Timer():
    def __init__(self):
        self.starting_time = None

    def start(self):
        self.starting_time = time()

    def time(self ):
        r = time() - self.starting_time
        return round(r)
    
    def print(self, message=""):
        print(message, f"{timer.time()} s")
        
timer = Timer()