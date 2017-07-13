#
# Copyright (C) 2017 by YOUR NAME HERE
#
#    This file is part of RoboComp
#
#    RoboComp is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    RoboComp is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with RoboComp.  If not, see <http://www.gnu.org/licenses/>.

import sys, Ice, os
from PySide import *

ROBOCOMP = ''
try:
	ROBOCOMP = os.environ['ROBOCOMP']
except KeyError:
	print '$ROBOCOMP environment variable not set, using the default value /opt/robocomp'
	ROBOCOMP = '/opt/robocomp'

preStr = "-I/opt/robocomp/interfaces/ -I"+ROBOCOMP+"/interfaces/ --all /opt/robocomp/interfaces/"
Ice.loadSlice(preStr+"CommonBehavior.ice")
import RoboCompCommonBehavior

additionalPathStr = ''
icePaths = []
try:
	SLICE_PATH = os.environ['SLICE_PATH'].split(':')
	for p in SLICE_PATH:
		icePaths.append(p)
		additionalPathStr += ' -I' + p + ' '
	icePaths.append('/opt/robocomp/interfaces')
except:
	print 'SLICE_PATH environment variable was not exported. Using only the default paths'
	pass

ice_AGM2 = False
for p in icePaths:
	print 'Trying', p, 'to load AGM2.ice'
	if os.path.isfile(p+'/AGM2.ice'):
		print 'Using', p, 'to load AGM2.ice'
		preStr = "-I/opt/robocomp/interfaces/ -I"+ROBOCOMP+"/interfaces/ " + additionalPathStr + " --all "+p+'/'
		wholeStr = preStr+"AGM2.ice"
		Ice.loadSlice(wholeStr)
		ice_AGM2 = True
		break
if not ice_AGM2:
	print 'Couln\'t load AGM2'
	sys.exit(-1)
from RoboCompAGM2 import *


from agmdsrserviceI import *

import rospy
from std_msgs.msg import *
try:
	from RoboCompAGM2ROS.msg import *
except:
	print "couldn't load msg"
from RoboCompAGM2ROS.srv import *
#class for rosPublisher
class PublisherAGMDSRTopic():
	def __init__(self):
		self.pub_structuralChange = rospy.Publisher("structuralChange", World, queue_size=1000)
		self.pub_edgesUpdated = rospy.Publisher("edgesUpdated", EdgeSequence, queue_size=1000)
		self.pub_symbolsUpdated = rospy.Publisher("symbolsUpdated", NodeSequence, queue_size=1000)
	def structuralChange(self, w):
		self.pub_structuralChange.publish(w)
	def edgesUpdated(self, modification):
		self.pub_edgesUpdated.publish(modification)
	def symbolsUpdated(self, modification):
		self.pub_symbolsUpdated.publish(modification)

class GenericWorker(QtCore.QObject):
	kill = QtCore.Signal()


	def __init__(self, mprx):
		super(GenericWorker, self).__init__()


		self.agmdsrtopic_proxy = mprx["AGMDSRTopicPub"]
		self.agmdsrtopic_rosproxy = PublisherAGMDSRTopic()


		self.mutex = QtCore.QMutex(QtCore.QMutex.Recursive)
		self.Period = 30
		self.timer = QtCore.QTimer(self)


	@QtCore.Slot()
	def killYourSelf(self):
		rDebug("Killing myself")
		self.kill.emit()

	# \brief Change compute period
	# @param per Period in ms
	@QtCore.Slot(int)
	def setPeriod(self, p):
		print "Period changed", p
		Period = p
		timer.start(Period)
