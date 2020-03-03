from gpiozero import DistanceSensor
from time import sleep
avoidanceTurn = 90
avoidanceDist = 90
sonarLeft = DistanceSensor(echo=17, trigger=27)
sonarFront = DistanceSensor(echo = 11, trigger = 9)
sonarRight = DistanceSensor(echo = 26, trigger = 19) 
##
def readRight():
    cm = sonarRight.distance * 100
    if(cm == 0):
        cm = 250
    return cm

def readLeft():
    cm = sonarLeft.distance * 100
    if(cm == 0):
        cm = 250
    return cm

def readFront():
    cm = sonarFront.distance * 100
    if(cm == 0):
        cm = 250
    return cm 

def turn(degrees):
    print("turning: " , degrees)
    #real motor movements

def moveForward(avoidanceDist):
    print ("move forward:" , avoidanceDist)
    #real motor movements

##readRight()
##readLeft()
##readFront()
while(1):
    #test if all of the sensors are connected properly
    print 'front' + str(readFront())
    print 'right' + str(readRight())
    print 'left' + str(readLeft())
    sleep(.)