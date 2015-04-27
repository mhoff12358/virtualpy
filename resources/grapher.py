#GF: first attempt at switching functions
#GF: note: the windowLib=glut works but badly. Was neglected.
#This is Crowell bzr rev 9
#Crowell 21dec11, Hoffman 15dec11, GF 22dec11
import sys, math, time
from complex import C

speed=.5 #original 10deg/tope is too fast in the cube.
        
def ThreeHalfs( c ):
    o = c*c
    m = c*c*c
    return m.real(), o.real(), m.imag(), o.imag()

def Square( c ):
    o = c
    m = c*c
    return m.real(), o.real(), m.imag(), o.imag()

def Exponential( c ):
    o = c
    m = C(math.exp(c.real()))*(C(math.cos(c.imag()))+C(0,1)*C(math.sin(c.imag())))
    return m.real(), o.real(), m.imag(), o.imag()
  

class Grapher(object):
    def __init__(self, fun = Exponential):
        
        self.function = fun
        self.colorsComputed = False
        self.values = None
            
        self.zap()
                

    def zap(self):
        if self.function == Exponential:
            self.umin = -2.0
            self.umax = 3.0
            self.udel = .5
            self.vmin = -1.0
            self.vmax = 10.0
            self.vdel = .5
        elif self.function == ThreeHalfs:
            self.umin = -1.0
            self.umax = 1.0
            self.udel = .1
            self.vmin = -1.0
            self.vmax = 1.0
            self.vdel = .1
        elif self.function == Square:
            self.umin = -2.0
            self.umax = 2.0
            self.udel = .1
            self.vmin = -2.0
            self.vmax = 2.0
            self.vdel = .1
        
        self.values = []
        
        self.rotValXW = 0
        self.drotValXW = 0
        self.rotValYW = 0
        self.drotValYW = 0
        self.rotValZW = 0
        self.drotValZW = 0
        
        self.xaxis = (1,0,0,0)
        self.yaxis = (0,1,0,0)
        self.zaxis = (0,0,1,0)
        self.waxis = (0,0,0,1)
        
        self.drawWireframe = True
        self.drawPolar = False
        
        self.graphicalValues = []

            
    def makeValues(self):
        self.values = [[self.function( complex(uval, vval)) \
            for vval in range(self.vmin, self.vmax, self.vdel)] \
            for uval in range(self.umin, self.umax, self.udel)]
    
    def compute(self):
        self.setRotationAngle(self.rotValXW + math.radians(self.drotValXW), "XW")
        self.setRotationAngle(self.rotValYW + math.radians(self.drotValYW), "YW")
        self.setRotationAngle(self.rotValZW + math.radians(self.drotValZW), "ZW")
        
        self.makeGraphicalValues()
        
    def displayCallback(self):
        self.compute()
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        self.display() #does the actual drawing
        glutSwapBuffers() #Does the glut specific drawing stuff
        glutPostRedisplay()
        
    def computeRGB(self, u, v):
        return (127, self.graphicalValues[u][v][3] / self.umax + .5, .5 - self.graphicalValues[u][v][3] / self.umax)
        
    def getRGB(self, u, v):
        return self.colorValues[u][v]

    def display(self):
        glMatrixMode(GL_MODELVIEW)
        
        glPushMatrix()
        
        if self.windowLib == "szg":
            glTranslate(0,5,-5)
        else:
            glTranslatef(0, -1, -5)
                
        if self.drawWireframe:
            # glDepthFunc(GL_LEQUAL)
            # glDisable(GL_DEPTH_TEST)
            glColor3f(1,1,1)
            for i in range(len(self.graphicalValues)-1):
                glBegin(GL_LINE_STRIP)
                for j in range(len(self.graphicalValues[i])-1):
                    glColor3f(*self.getRGB(i,j))
                    glVertex3f( *self.graphicalValues[i][j][:3] )
                glEnd()
            for j in range(len(self.graphicalValues[0])-1):
                glBegin(GL_LINE_STRIP)
                for i in range(len(self.graphicalValues)-1):
                    glColor3f(*self.getRGB(i,j))
                    glVertex3f( *self.graphicalValues[i][j][:3] )
                glEnd()
            # glDepthFunc(GL_LESS)
        else:
            for i in range(1, len(self.graphicalValues)-2, 2):
                for j in range(1, len(self.graphicalValues[i])-2, 2):
                    glBegin(GL_TRIANGLE_FAN)
                    glColor3f(*self.getRGB(i,j))
                    glVertex3f(self.graphicalValues[i][j][0], self.graphicalValues[i][j][1], self.graphicalValues[i][j][2])
                    glColor3f(*self.getRGB(i,j-1))
                    glVertex3f(self.graphicalValues[i][j-1][0], self.graphicalValues[i][j-1][1], self.graphicalValues[i][j-1][2])
                    glColor3f(*self.getRGB(i-1,j-1))
                    glVertex3f(self.graphicalValues[i-1][j-1][0], self.graphicalValues[i-1][j-1][1], self.graphicalValues[i-1][j-1][2])
                    glColor3f(*self.getRGB(i-1,j))
                    glVertex3f(self.graphicalValues[i-1][j][0], self.graphicalValues[i-1][j][1], self.graphicalValues[i-1][j][2])
                    glColor3f(*self.getRGB(i-1,j+1))
                    glVertex3f(self.graphicalValues[i-1][j+1][0], self.graphicalValues[i-1][j+1][1], self.graphicalValues[i-1][j+1][2])
                    glColor3f(*self.getRGB(i,j+1))
                    glVertex3f(self.graphicalValues[i][j+1][0], self.graphicalValues[i][j+1][1], self.graphicalValues[i][j+1][2])
                    glColor3f(*self.getRGB(i+1,j+1))
                    glVertex3f(self.graphicalValues[i+1][j+1][0], self.graphicalValues[i+1][j+1][1], self.graphicalValues[i+1][j+1][2])
                    glColor3f(*self.getRGB(i+1,j))
                    glVertex3f(self.graphicalValues[i+1][j][0], self.graphicalValues[i+1][j][1], self.graphicalValues[i+1][j][2])
                    glColor3f(*self.getRGB(i+1,j-1))
                    glVertex3f(self.graphicalValues[i+1][j-1][0], self.graphicalValues[i+1][j-1][1], self.graphicalValues[i+1][j-1][2])
                    glColor3f(*self.getRGB(i,j-1))
                    glVertex3f(self.graphicalValues[i][j-1][0], self.graphicalValues[i][j-1][1], self.graphicalValues[i][j-1][2])
                    glEnd()
        
        # glDisable(GL_DEPTH_TEST)
        glBegin(GL_LINES)
        glColor(255,0,0)
        glVertex3f(0,0,0)
        glVertex3f( *self.xaxis )
#        glVertex3f(self.xaxis[0],self.xaxis[1],self.xaxis[2])
        glEnd()
        glBegin(GL_LINES)
        glColor(0,255,0)
        glVertex3f(0,0,0)
        glVertex3f( *self.yaxis )
#        glVertex3f(self.yaxis[0],self.yaxis[1],self.yaxis[2])
        glEnd()
        glBegin(GL_LINES)
        glColor(0,0,255)
        glVertex3f(0,0,0)
        glVertex3f( *self.zaxis )
#        glVertex3f(self.zaxis[0],self.zaxis[1],self.zaxis[2])
        glEnd()
        glBegin(GL_LINES)
        glColor(255,255,255)
        glVertex3f(0,0,0)
        glVertex3f( *self.waxis )
#        glVertex3f(self.waxis[0],self.waxis[1],self.waxis[2])
        glEnd()
        # glEnable(GL_DEPTH_TEST)
        
        glPopMatrix()
        
    def setRotationAngle(self, angle, plane):
        if plane == "XW":
            self.rotValXW = angle
        elif plane == "YW":
            self.rotValYW = angle
        elif plane == "ZW":
            self.rotValZW = angle
        
    def autoXRot(self):
        self.drotValXW += speed
        if self.drotValXW == 2*speed: 
           self.drotValXW = -speed 
        
    def autoYRot(self):
        self.drotValYW += speed
        if self.drotValYW == 2*speed:
           self.drotValYW = -speed
        
    def autoZRot(self):
        self.drotValZW += speed
        if self.drotValZW ==2*speed:
           self.drotValZW = -speed
        
    def setdrot(self, value, var):
        if var == value:
            var = 0
        else:
            var = value
        
    def keyDown(self, key, x, y):
        # print key, x, y
        if key == 'x':
            if self.drotValXW == 10:
                self.drotValXW = 0
            else:
                self.drotValXW = 10
        if key == 'X':
            if self.drotValXW == -10:
                self.drotValXW = 0
            else:
                self.drotValXW = -10
        if key == 'y':
            if self.drotValYW == 10:
                self.drotValYW = 0
            else:
                self.drotValYW = 10
        if key == 'Y':
            if self.drotValYW == -10:
                self.drotValYW = 0
            else:
                self.drotValYW = -10
        if key == 'z':
            if self.drotValZW == 10:
                self.drotValZW = 0
            else:
                self.drotValZW = 10
        if key == 'Z':
            if self.drotValZW == -10:
                self.drotValZW = 0
            else:
                self.drotValZW = -10
        if key == 'w':
            self.drawWireframe = not self.drawWireframe
        if key == 'r':
            self.zap()
        if key == chr(27):
            sys.exit(0)

if __name__ == "__main__":
    gasket = Grapher()