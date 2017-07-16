/*
 *    Copyright (C)2017 by YOUR NAME HERE
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

#include "specificworker.h"

/**
* \brief Default constructor
*/

#define BUUU printf("%s: %d\n", __FILE__, __LINE__);

SpecificWorker::SpecificWorker(MapPrx& mprx) : GenericWorker(mprx)
{
	worldModel = AGMModel::SPtr(new AGMModel());

	QPalette palette1 = modelWidget->palette();
	palette1.setColor( backgroundRole(), QColor( 255, 255, 255 ) );
	modelWidget->setPalette(palette1);
	
	rcdraw1 = new RCDraw( modelWidget);
	modelDrawer = new AGMModelDrawer(rcdraw1, tableWidget);

	QTimer *secondTimer = new QTimer();
	secondTimer->start(1000);
	connect(secondTimer, SIGNAL(timeout()), this, SLOT(setGeometry()));

//	set3DViewer();

	connect(quitButton,           SIGNAL(clicked()), this, SLOT(quitButtonClicked()));
	connect(broadcastModelButton, SIGNAL(clicked()), this, SLOT(broadcastModelButtonClicked()));
	connect(imCheck,              SIGNAL(clicked()), this, SLOT(imShow()));
	connect(robotCheck,           SIGNAL(clicked()), this, SLOT(showRobot()));
	connect(meshCheck,            SIGNAL(clicked()), this, SLOT(showMesh()));
	connect(planeCheck,           SIGNAL(clicked()), this, SLOT(showPlane()));

	connect(saveButton,           SIGNAL(clicked()), this, SLOT(saveModel()));

	
	connect(itemList,     SIGNAL(activated(QString)), this, SLOT(itemSelected(QString)));
	
	itemList->completer()->setCompletionMode (QCompleter::UnfilteredPopupCompletion);
// 	itemList->completer()->setCompletionMode (QCompleter::PopupCompletion);


	scrollArea->setAlignment (Qt::AlignCenter);
	QSize  widgetSize = modelWidget->size();
	scrollArea->ensureVisible(widgetSize.width()/2,widgetSize.height()/2 );
	innerModelVacio = new InnerModel();
	osgView = new OsgView(  inner3D );
	show();
	
	innerViewer = new InnerModelViewer(innerModelVacio, "root", osgView->getRootGroup(), true);
	manipulator = new osgGA::TrackballManipulator;
	osgView->setCameraManipulator(manipulator, true);
	innerViewer->setMainCamera(manipulator, InnerModelViewer::TOP_POV);
	timer.start(90);

	lastChange = QTime::currentTime();

	RoboCompAGM2::World w;
	agmdsrservice_proxy->getModel(true, w);
	structuralChange(w);
}

/**
* \brief Default destructor
*/
SpecificWorker::~SpecificWorker()
{

}

bool SpecificWorker::setParams(RoboCompCommonBehavior::ParameterList params)
{
//       THE FOLLOWING IS JUST AN EXAMPLE
//
//	try
//	{
//		RoboCompCommonBehavior::Parameter par = params.at("InnerModelPath");
//		innermodel_path = par.value;
//		innermodel = new InnerModel(innermodel_path);
//	}
//	catch(std::exception e) { qFatal("Error reading config params"); }
	timer.start(Period);
	return true;
}


void SpecificWorker::compute()
{
	secondsLabel->setText(QString::number(float(lastChange.elapsed())/1000));
// 	printf("compute 1\n");
	static QTime taim = QTime::currentTime();

// 	graphViewer->animateStep();
	
	{
		QMutexLocker dd(&modelMutex);
		
		
		if (tabWidget->currentIndex()==0)
		{
			modelDrawer->update(worldModel);
			//void QScrollArea::ensureVisible ( int x, int y, int xmargin = 50, int ymargin = 50 )
// 			qDebug()<<"***************************************************";
// 			qDebug()<<"widgetSize"<<widgetSize;
// 			qDebug()<<"***************************************************";
// 			qDebug()<<"rcdraw1->getWindow()"<<rcdraw1->getWindow();
		}
		else if (tabWidget->currentIndex() == 1 )
		{
			modelDrawer->recalculatePositions();
			modelDrawer->drawTable();
			innerViewer->update();
			osgView->autoResize();
			osgView->frame();
		}
	}

	taim = QTime::currentTime();
// 	printf("compute>\n");
}


void SpecificWorker::changeInner(InnerModel *inner)
{
	static InnerModel *b = innerModelVacio;
	if (innerViewer)
	{
		osgView->getRootGroup()->removeChild(innerViewer);
		delete b;
		b = inner;
	}
	innerViewer = new InnerModelViewer(inner, "root", osgView->getRootGroup(), true);
	lastChange = QTime::currentTime();
}


void SpecificWorker::structuralChange(const World &w)
{
	QMutexLocker dd(&modelMutex);
	AGMModelConverter::fromIceToInternal(w, worldModel);
	changeInner(AGMInner::extractInnerModel(worldModel));
	fillItemList();
}


void SpecificWorker::edgesUpdated(const EdgeSequence &modification)
{
	QMutexLocker dd(&modelMutex);
	for (auto e : modification)
	{
		AGMModelConverter::includeIceModificationInInternalModel(e, worldModel);
		AGMInner::updateImNodeFromEdge(worldModel, e, innerViewer->innerModel);
	}
}

void SpecificWorker::symbolsUpdated(const NodeSequence &modification)
{
	QMutexLocker dd(&modelMutex);
	for (auto n : modification)
		AGMModelConverter::includeIceModificationInInternalModel(n, worldModel);
}


// void SpecificWorker::update(const RoboCompAGMWorldModel::World &a,  const string &target_, const RoboCompPlanning::Plan &pl)
// {
// 	printf("SpecificWorker::update plan and target\n");
// 	QMutexLocker dd(&planMutex);
// 	plan = pl;
// 	target = target_;
// 	refreshPlan = true;
// }

void SpecificWorker::fillItemList()
{
	for (uint32_t i=0; i<worldModel->symbols.size(); ++i)
	{
		itemList->addItem(QString::fromStdString(worldModel->symbols[i]->toString()));
	}
	for (uint32_t i=0; i<worldModel->edges.size(); ++i)
	{
		itemList->addItem(QString::fromStdString(worldModel->edges[i]->toString(worldModel)));
	}


}

void SpecificWorker::itemSelected(QString nameItem)
{
	qDebug()<<"nameItem"<<nameItem;
	modelDrawer->setInterest(nameItem.toStdString());
}



void SpecificWorker::imShow()
{
	qDebug()<<"imCheck->isChecked()"<<imCheck->isChecked();
	modelDrawer->setShowInnerModel(imCheck->isChecked());
	
}
void SpecificWorker::showRobot()
{
	qDebug()<<"robotCheck->isChecked()"<<robotCheck->isChecked();
	modelDrawer->setShowRobot(robotCheck->isChecked());	
	
}
void SpecificWorker::showMesh()
{
	qDebug()<<"showMesh: meshCheck->isChecked()"<<meshCheck->isChecked();
	modelDrawer->setShowMesh(meshCheck->isChecked());	
	
}
void SpecificWorker::showPlane()
{
	qDebug()<<"showPlane: planeCheck->isChecked()"<<planeCheck->isChecked();
	modelDrawer->setShowPlane(planeCheck->isChecked());	
	
}



void SpecificWorker::quitButtonClicked()
{
	printf("quit button\n");
	exit(0);
}

void SpecificWorker::broadcastModelButtonClicked()
{
	printf("broadcast model button\n");
	try
	{
		bool ignored;
		agmdsrservice_proxy->broadcastModel(false, ignored);
	}
	catch(const Ice::Exception &e)
	{
		QMessageBox::critical(this, "Can't connect to the dsr", "Can't connect to the dsr. Please, make sure the dsr is running properly");
	}
}



/*
void SpecificWorker::set3DViewer()
{
// #if QT_VERSION >= 0x050000
// 	// Qt5 is currently crashing and reporting "Cannot make QOpenGLContext current in a different thread" when the viewer is run multi-threaded, this is regression from Qt4
	osgViewer::ViewerBase::ThreadingModel threadingModel = osgViewer::ViewerBase::SingleThreaded;
// #else
// 	osgViewer::ViewerBase::ThreadingModel threadingModel = osgViewer::ViewerBase::CullDrawThreadPerContext;
// #endif

	graphViewer = new GraphModelViewer(threadingModel, widget3D, false);

	setGeometry();
	graphViewer->show();
}
*/

void SpecificWorker::setGeometry()
{
// 	graphViewer->setGeometry(0, 0, widget3D->width(), widget3D->height());
}

void SpecificWorker::saveModel()
{
	QString f = QFileDialog::getSaveFileName(this, "Choose a file to save the current model");
	worldModel->save(f.toStdString());
}

