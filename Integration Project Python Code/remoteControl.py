#!/use/bin/env python
#
from nanpy import ArduinoApi, SerialManager, Servo
from time import sleep
import pygame
import logging
import sys
import time

global speed
speed = 220
global spd
spd = 0
global target
target = 0
global rotating_speed
global flag
flag = 0
rotating_speed = 100
print 'a'
c = SerialManager(device='/dev/ttyACM0')
a = ArduinoApi(connection=c)
print 'b'
myservo = Servo( 17, connection = c)
dirA = 8
dirB = 10
pwmA = 9
pwmB = 11
stp = 2
dir = 3
MS1 = 4
MS2 = 5
EN  = 6
a.pinMode(stp, a.OUTPUT);
a.pinMode(dir, a.OUTPUT);
a.digitalWrite(MS1, a.HIGH);
a.digitalWrite(MS2, a.HIGH);
a.pinMode(EN, a.OUTPUT);
a.digitalWrite(EN, a.LOW)




screen = pygame.display.set_mode([300,100])
screen.fill([255,255,255])
pygame.display.set_caption("W C")

def forward(spd):
    global flag
    flag = 1
    while(spd <speed and flag == 1):
        spd+=20
        a.digitalWrite(dirA, a.HIGH)
        a.digitalWrite(dirB, a.LOW)
        a.analogWrite(pwmA, spd)
        a.analogWrite(pwmB, spd)
    return 


def backward(spd):
    global flag
    flag = 1
    flag = 1
    while(spd <speed and flag == 1):
        spd+=20
        a.digitalWrite(dirA, a.LOW)
        a.digitalWrite(dirB, a.HIGH)
        a.analogWrite(pwmA, spd)
        a.analogWrite(pwmB, spd)
    return 
    return 

def right(target):
    global flag
    flag = 1
    a.digitalWrite(dirA, a.HIGH)
    a.digitalWrite(dirB, a.HIGH)
    a.analogWrite(pwmA, 170)
    a.analogWrite(pwmB, 170)
    return



def left(target):
    global flag
    flag = 1
    a.digitalWrite(dirA, a.LOW)
    a.digitalWrite(dirB, a.LOW)
    a.analogWrite(pwmA, 170)
    a.analogWrite(pwmB, 170)
    return 

def hard_brakes():
    flag = 0
    a.analogWrite(pwmA, 0)
    a.analogWrite(pwmB, 0)
    a.digitalWrite(18, a.LOW)
    a.digitalWrite(19, a.LOW)
    a.analogWrite(stp, 0)
    spd = 0
    return 




def Stairs():
    global flag
    flag = 0
    while(spd > 0 and flag == 0):
        spd-= spd
        a.digitalWrite(m1dir, a.LOW)
        a.digitalWrite(m2dir, a.HIGH)
        a.analogWrite(m1pwm, spd)
        a.analogWrite(m2pwm, spd)
    return
def StepForward():
    a.digitalWrite(dir, a.LOW)
    a.analogWrite(stp, 127)

def StepBackward():
    a.digitalWrite(dir, a.HIGH);
    a.analogWrite(stp, 127)
def ServoForward():
    pos = 0
    while pos < 180:
        myservo.write(pos)
        pos = pos + 1
    while pos > 0:
        myservo.write(pos)
        pos = pos -1
def leadUp():
    a.digitalWrite(18, a.HIGH)
    a.digitalWrite(19, a.LOW)
def leadDown():
    a.digitalWrite(18, a.LOW)
    a.digitalWrite(19, a.HIGH)

while(1):
    key = pygame.key.get_pressed()
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            break
        elif event.type == pygame.KEYDOWN:
            #wasd for drivetrain movement
            if event.key == pygame.K_w:
                print("forward")
                forward(spd)
            if event.key == pygame.K_a:
                print("left")
                left(target)
            if event.key == pygame.K_s:
                print("back")
                backward(spd)
            if event.key == pygame.K_d:
                print("right")
                right(target)
             #o and p for the front arm aparatus lateral movement
            if event.key == pygame.K_o:
                StepForward()
                print("StepForward")
            if event.key == pygame.K_p:
                StepBackward()
                print("StepBackward")
            if event.key == pygame.K_ESCAPE:
                break
            if event.key == pygame.K_SPACE:
                print("stop")
                hard_brakes()
            #k and t for the servo movement
            if event.key == pygame.K_y:
                print("servo forward")
                ServoForward()
            if event.key == pygame.K_t:
                print("servo backward")
                ServoForward()
            #l and m for the arm vertical movement
            if event.key == pygame.K_l:
                print("up")
                leadUp()
            if event.key == pygame.K_m:
                print("down")
                leadDown()
                
            
        elif event.type == pygame.KEYUP:
            print("stop")
            hard_brakes()
    pygame.display.update()
pygame.quit
