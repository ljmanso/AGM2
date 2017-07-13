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

import sys, os, traceback, time

# AGM
sys.path.append('/usr/local/share/agm2')

from PySide import *
from genericworker import *

import xmlModelParser
import AGM2ModelConversion

class SpecificWorker(GenericWorker):
	def __init__(self, proxy_map):
		super(SpecificWorker, self).__init__(proxy_map)
		self.Period = 2000
		self.timer.timeout.connect(self.compute)
		self.timer.start(self.Period)

	def setParams(self, params):
		# Get initial model path
		try:
			self.initialModelPath = par = params["InitialModelPath"]
		except:
			traceback.print_exc()
			print "Error reading config params"
			sys.exit(-1)
		# Read initial model and set it as the last modification
		#try:
		self.currentModel = xmlModelParser.graphFromXML(self.initialModelPath)
		#except Exception, e:
			#print 'Can\'t open ' + self.initialModelPath + '.'
			#sys.exit(-1)
		self.initialModel = self.currentModel
		print 'INITIAL MODEL: ', self.initialModel
		self.lastModification = self.initialModel
		print self.lastModification.version

		self.worldModelICE = AGM2ModelConversion.fromInternalToIce(self.currentModel)

		self.timer.start(self.Period)
		self.timer.timeout.connect(self.compute)

		self.broadcastCurrentModel()

		return True

	@QtCore.Slot()
	def compute(self):
		self.broadcastCurrentModel()
		return True


	#
	# edgesUpdate
	#
	def edgesUpdate(self, es):
		#
		#implementCODE
		#
		ret = int()
		return ret




	#
	# broadcastModel
	#
	def broadcastModel(self, unusedI):
		unusedR = self.broadcastCurrentModel()
		return unusedR


	#
	# getEdge
	#
	def getEdge(self, input):
		#
		#implementCODE
		#
		ret = Edge()
		return ret


	#
	# getModel
	#
	def getModel(self, unused):
		#
		#implementCODE
		#
		ret = World()
		return ret


	#
	# getNode
	#
	def getNode(self, identifier):
		#
		#implementCODE
		#
		ret = Node()
		return ret


	#
	# symbolsUpdate
	#
	def symbolsUpdate(self, ns):
		#
		#implementCODE
		#
		ret = int()
		return ret


	#
	# edgeUpdate
	#
	def edgeUpdate(self, e):
		#
		#implementCODE
		#
		ret = int()
		return ret


	#
	# symbolUpdate
	#
	def symbolUpdate(self, n):
		#
		#implementCODE
		#
		ret = int()
		return ret


	#
	# structuralChangeProposal
	#
	def structuralChangeProposal(self, proposal):
		#
		#implementCODE
		#
		ret = int()
		return ret

	def ROSedgesUpdate(self, req):
		#
		#implementCODE
		#Example ret = req.a + req.b
		#
		return edgesUpdateResponse(ret)
	def ROSbroadcastModel(self, req):
		#
		#implementCODE
		#Example ret = req.a + req.b
		#
		return broadcastModelResponse(ret)
	def ROSgetEdge(self, req):
		#
		#implementCODE
		#Example ret = req.a + req.b
		#
		return getEdgeResponse(ret)
	def ROSgetModel(self, req):
		#
		#implementCODE
		#Example ret = req.a + req.b
		#
		return getModelResponse(ret)
	def ROSgetNode(self, req):
		#
		#implementCODE
		#Example ret = req.a + req.b
		#
		return getNodeResponse(ret)
	def ROSsymbolsUpdate(self, req):
		#
		#implementCODE
		#Example ret = req.a + req.b
		#
		return symbolsUpdateResponse(ret)
	def ROSedgeUpdate(self, req):
		#
		#implementCODE
		#Example ret = req.a + req.b
		#
		return edgeUpdateResponse(ret)
	def ROSsymbolUpdate(self, req):
		#
		#implementCODE
		#Example ret = req.a + req.b
		#
		return symbolUpdateResponse(ret)
	def ROSstructuralChangeProposal(self, req):
		#
		#implementCODE
		#Example ret = req.a + req.b
		#
		return structuralChangeProposalResponse(ret)




	def setAndBroadcastModel(self, model):
		self.currentModel = model
		self.broadcastCurrentModel()

	def broadcastCurrentModel(self):
		self.mutex.lock()
		#try:
		print '<<<broadcastinnn'
		print self.currentModel.filterGeometricSymbols()
		self.agmdsrtopic_proxy.structuralChange(AGM2ModelConversion.fromInternalToIce(self.currentModel))
		self.agmdsrtopic_rosproxy.structuralChange(AGM2ModelConversion.fromInternalToROS(self.currentModel))
		print 'broadcastinnn>>>'
		#except:
			#print traceback.print_exc()
			#print 'There was some problem broadcasting the model'
			#sys.exit(1)
		self.mutex.unlock()

