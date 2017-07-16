#ifndef WORLDMODELPRINTER_H
#define WORLDMODELPRINTER_H

#include <stdio.h>

#include <agm_config.h>
#include <agm_model.h>
#include <agm_modelEdge.h>
#include <agm_modelSymbols.h>

#if ROBOCOMP_SUPPORT == 1
#include <AGM2.h>
#endif

/*!
 * @brief Utility class to print AGMModel graphs
 *
 * @ingroup CPPAPI
 * 
 * 
 */
class AGMModelPrinter
{
public:
	/// Prints a model given a shared pointer to it.
	static void printWorld(const AGMModel::SPtr &w);
	/// Prints a model given a regular pointer to it.
	static void printWorld(const AGMModel *w);
#if ROBOCOMP_SUPPORT == 1
	/// Prints a model given a RoboComp <em>RoboCompAGMWorldModel::World</em> model data type.
	static void printWorld(const RoboCompAGM2::World &w);
	/// Prints a model given a RoboComp <em>RoboCompAGMWorldModel::World</em> model data type, to a file.
	static void printWorld(FILE *fd, const RoboCompAGM2::World &w);
#endif
};


#endif

