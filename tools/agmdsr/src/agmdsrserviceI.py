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
#

import sys, os, Ice

ROBOCOMP = ''
try:
	ROBOCOMP = os.environ['ROBOCOMP']
except:
	print '$ROBOCOMP environment variable not set, using the default value /opt/robocomp'
	ROBOCOMP = '/opt/robocomp'
if len(ROBOCOMP)<1:
	print 'ROBOCOMP environment variable not set! Exiting.'
	sys.exit()

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

class AGMDSRServiceI(AGMDSRService):
	def __init__(self, worker):
		self.worker = worker

	def edgesUpdate(self, es, c):
		return self.worker.edgesUpdate(es)
	def broadcastModel(self, unusedI, c):
		return self.worker.broadcastModel(unusedI)
	def getEdge(self, input, c):
		return self.worker.getEdge(input)
	def getModel(self, unused, c):
		return self.worker.getModel(unused)
	def getNode(self, identifier, c):
		return self.worker.getNode(identifier)
	def symbolsUpdate(self, ns, c):
		return self.worker.symbolsUpdate(ns)
	def edgeUpdate(self, e, c):
		return self.worker.edgeUpdate(e)
	def symbolUpdate(self, n, c):
		return self.worker.symbolUpdate(n)
	def structuralChangeProposal(self, proposal, c):
		return self.worker.structuralChangeProposal(proposal)
