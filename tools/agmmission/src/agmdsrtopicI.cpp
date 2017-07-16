/*
 *    Copyright (C) 2017 by YOUR NAME HERE
 *
 *    This file is part of RoboComp
 *
 *    RoboComp is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    RoboComp is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with RoboComp.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "agmdsrtopicI.h"

AGMDSRTopicI::AGMDSRTopicI(GenericWorker *_worker)
{
	worker = _worker;
}


AGMDSRTopicI::~AGMDSRTopicI()
{
}

void AGMDSRTopicI::structuralChange(const World  &w, const Ice::Current&)
{
	worker->structuralChange(w);
}

void AGMDSRTopicI::edgesUpdated(const EdgeSequence  &modification, const Ice::Current&)
{
	worker->edgesUpdated(modification);
}

void AGMDSRTopicI::symbolsUpdated(const NodeSequence  &modification, const Ice::Current&)
{
	worker->symbolsUpdated(modification);
}

