from time import time

class timer_():
    def __init__(self):
        self.starting_time = None

    def start(self):
        self.starting_time = time()

    def time(self):
        r = time() - self.starting_time
        return round(r)

timer = timer_()