

import math
#Point class to make a 2d point and do distance calculations between points
class Point(object):

	def __init__(self, x, y=0):
		if (isinstance(x, list)):
			# list x,y,z
			self.x = x[0]
			self.y = x[1]
		else:
			self.x = x
			self.y = y

	def distanceTo(self, vec):
		return math.sqrt(
			((vec.getX() - self.x) ** 2) + 
			((vec.getY() - self.y) ** 2))

	def getX(self):
		return self.x

	def getY(self):
		return self.y

	def setX(self, x):
		self.x = x

	def setY(self, y):
		self.y = y


	def toString(self):
		return "[{}, {}, {}]".format(self.x, self.y)
