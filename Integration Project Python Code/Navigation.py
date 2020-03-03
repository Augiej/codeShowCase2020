#!/usr/bin/env python
import re
import math
import time
import serial
import sys

from nanpy import ArduinoApi,SerialManager
from marvelmind import MarvelmindHedge
from time import sleep
from Point import Point
from Vector import Vector

#Set up the arduino as a slave to the raspberry pi through a serial port
c = SerialManager(device='/dev/ttyACM0')
a = ArduinoApi(connection=c)
#Arduino Motor Control Pins
dirA = 8
dirB = 10
pwmA = 9
pwmB = 11
# initialize the MarvelMindHedge GPS
hedge = MarvelmindHedge(tty = "/dev/ttyACM2", adr=9, debug=False)
#initialize the input from the IMU
ser = serial.Serial('/dev/ttyACM1', 115200, timeout = 0)
# set the path
path = [Point(.2, -32), Point(0.016, -28.119), Point(-0.591, -21.995), Point(-0.197, -15.945), Point(0.293, -9.742), Point(0.077, -2.05), Point(.565, -2.212)]
#set arduino pins - motors start off
#initalize self point
self = Point(0,0)
data = hedge.position()
self.setX(data[1])
self.setY(data[2])
#initialize direction
direction = 0
#flush imu input to get most recent value (serial is a queue)
ser.flushInput()
time.sleep(.5)
#read and parse the data
rawDir = ser.readline()
rawDir = rawDir.replace('\r\n', '')
#initilze the offset
offset = float(rawDir) + 24
time.sleep(1)
print str(offset)
def navigation(hedge, ser, path, self, direction, rawDir, offset):
    count = 0
    #initialize first vector
    pathPoint = 0
    vecGo = Vector(self, path[pathPoint])
    hedge.start()
    while True:
        #set current position
        data = hedge.position()
        self.setX(data[1])
        self.setY(data[2])
        #set vecGo for the angle Calc#
        vecGo = Vector(self, path[pathPoint])
        #set direction
        ser.flushInput()
        time.sleep(.25)
        rawDir = ser.readline()
        rawDir = rawDir.replace('\r\n', '')

####################### - HANDLE TURN - ###################################################
        #normalize the IMU angle
        direction = normalAngle(float(rawDir), offset)
        direction = confineAngle(direction)
        #find how much to turn and negative if left and positive if right
        turn = vecGo.angleBetween(direction)
    
        #desired angle
        desAngle = confineAngle(direction)
####################### - HANDLE MOVEMENT - ###################################################
        #if i am still too far from my desired point move forward
        if(vecGo.toScalar() < 1):
            pathPoint += 1
            vecGo = Vector(self, path[pathPoint])
            print 'go to next path point'
            time.sleep(1)
        else:
            if(abs(turn) > 6):
                angleCorrect(turn, vecGo)
            else:
                moveForward(vecGo)
        #print current direction and the distance to the next path point
        print ' direction  '  + str(direction) + ' angle to turn ' + str(turn) 
        print 'Scalar Distance to Point is: ' + str(vecGo.toScalar())
       


#debug motor code (Instead of motor movement have print statements)     
       #pathPoint += 1
##def moveForward(vecGo):
##    print 'moveforward called, distance to target is ' + str(vecGo.toScalar())
##def angleCorrect(turn, vecGo):
##    if(turn < 0):
##        print 'turn right ' + str(turn) + ' Distance to ' + str(vecGo.toScalar())
##    else:
##        print 'turn left ' + str(turn) + ' Distance to ' + str(vecGo.toScalar())
##    #print 'angle correct called, desired change in angle is ' + str(turn)
##    time.sleep(.1)

###Movement Code###
#turn motors right or left for half a second
def angleCorrect(turn, currentAngle):
    if(turn > 0):
        print 'turn left'
        a.digitalWrite(dirA, a.LOW)
        a.digitalWrite(dirB, a.LOW)
    elif(turn < 0):
        print'turn right'
        a.digitalWrite(dirA, a.HIGH)
        a.digitalWrite(dirB, a.HIGH)
#TURN MOTORS ON FOR A CERTAIN AMOUNT OF TIME
    a.analogWrite(pwmA, 170)
    a.analogWrite(pwmB, 170)
    time.sleep(.5)
#STOP TURNING
    a.analogWrite(pwmA, 0)
    a.analogWrite(pwmB, 0)
#Moveforward for 1.25 seconds
def moveForward(vecGo):
    #constant will be a multiplier for how LONG your robot moves forward
    constant = 0.2
    a.digitalWrite(dirA, a.LOW)
    a.digitalWrite(dirB, a.HIGH)
#TURN MOTORS ON FOR A CERTAIN AMOUNT OF TIME
    a.analogWrite(pwmA, 170)
    a.analogWrite(pwmB, 170)
    time.sleep(1.25)
#STOP
    a.analogWrite(pwmA, 0)
    a.analogWrite(pwmB, 0)
 #code to confine the angle to within 360 degrees
def confineAngle(angle):
    if(angle > 360):
        angle = angle - 360
    elif(angle < 0):
        angle = angle + 360
    return angle
#code to normalize the angle with the offset
def normalAngle(rawAngle, offset):
    return rawAngle + offset
    
navigation(hedge, ser, path, self, direction, rawDir, offset)

    
