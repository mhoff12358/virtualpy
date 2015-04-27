class C(object):
    def __init__(self, x, y = 0):
        self.u = x
        self.v = y
        
    def real(self):
        return self.u
        
    def imag(self):
        return self.v
        
    def __add__(self, other):
        return C(self.u + other.u, self.v + other.v)
        
    def __sub__(self, other):
        return C(self.u - other.u, self.v - other.v)
        
    def __mul__(self, other):
        return C(self.u * other.u - self.v * other.v, self.u * other.v + self.v * other.u)