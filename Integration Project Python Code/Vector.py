#!/usr/bin/env python
# Vector.py
# Contains a class for making 2D and 3D vectors

import math
from Point import Point

class Vector(object):

	def __init__(self, p1, p2):
		self.p1 = Point(p1.getX(), p1.getY())
		self.p2 = Point(p2.getX(), p2.getY())

	def magnitude(self):
		return self.p1.distanceTo(self.p2)

	def vectorLength(self):
		""" Rectangular vector length """
		return [self.p2.getX() - self.p1.getX(), self.p2.getY() - self.p1.getY()]

	def toString(self):
		return "from " + self.p1.toString() + " to " + self.p2.toString()

	def setFrom(self, point):
		if (isinstance(point, Point)):
			self.p1 = point
		if (isinstance(point, list)):
			if (len(point) > 2):
				self.p1 = Point(point[0], point[1], point[2])
			else:
				self.p1 = Point(point[0], point[1])

	def setTo(self, point):
		if (isinstance(point, Point)):
			self.p2 = point
		if (isinstance(point, list)):
			if (len(point) > 2):
				self.p2 = Point(point[0], point[1], point[2])
			else:
				self.p2 = Point(point[0], point[1])
	def toScalar(self):
                return math.sqrt((self.p2.getX() - self.p1.getX())**2 + (self.p2.getY() - self.p1.getY())**2)
	def angleBetween(self, secondAngle):
                magnitude = self.toScalar()
                lengths = self.vectorLength()
                ####### Conver both angles to unit vectors #######
                unitVecY = lengths[1] / magnitude
                unitVecX = lengths[0] / magnitude
                unitVecAngleY = math.sin(math.radians(secondAngle))
                unitVecAngleX = math.cos(math.radians(secondAngle))
                
                selfAngle = math.degrees(math.acos(unitVecX))
                #### adjust for quadrants of unit vec x and y ####
                
                #### Calculate anglemag between the two vectors ####
                
                
                selfAngle = selfAngle - 180

                selfAngleUnitVecX = math.cos(math.radians(selfAngle))
                selfAngleUnitVecY = math.sin(math.radians(selfAngle))
                angleMag = math.degrees(math.acos(selfAngleUnitVecY * unitVecAngleY + selfAngleUnitVecX * unitVecAngleX))    
                if(selfAngle <= 0):
                        selfAngle = selfAngle + 360
               
                ####### see which way to turn #######
                ### not in opposite quadrant ####
                ### is the angle greater than or less than our og angle ####
                
                if(selfAngle < secondAngle and (selfAngle + 180) > secondAngle):
                        return -angleMag
                elif(selfAngle < secondAngle and (selfAngle + 180) < secondAngle):
                        return angleMag
                elif(selfAngle > secondAngle and (secondAngle + 180) > selfAngle):
                        return angleMag
                elif(selfAngle > secondAngle and (secondAngle + 180) < selfAngle):
                        return -angleMag
