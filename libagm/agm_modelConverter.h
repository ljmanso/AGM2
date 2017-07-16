#pragma once

#include <agm_config.h>
#include <agm_model.h>
#include <agm_modelEdge.h>
#include <agm_modelSymbols.h>

#if ROBOCOMP_SUPPORT == 1
#include <AGM2.h>
#endif

/*!
 * @brief Utility class used to convert between different graph representation.
 *
 * 
 * 
 */
class AGMModelConverter
{
public:

#if ROBOCOMP_SUPPORT == 1
	///World
	/// Converts an AGM world model from an AGMModel container to a RoboCompAGM2::World container.
	static void fromInternalToIce(const AGMModel::SPtr &world, RoboCompAGM2::World &dst);
	/// Converts an AGM world model from a RoboCompAGM2::World container to an AGMModel container.
	static void fromIceToInternal(const RoboCompAGM2::World &world, AGMModel::SPtr &dst);

	///Nodes
	/// Converts an AGM symbol model from an AGMModelSymbol container (given by a shared pointer) to a RoboCompAGM2::Node container.
	static void fromInternalToIce(const AGMModelSymbol::SPtr &node, RoboCompAGM2::Node &dst);
	/// Converts an AGM symbol model from an AGMModelSymbol container (given by a regular pointer) to a RoboCompAGM2::Node container.
	static void fromInternalToIce(const AGMModelSymbol *node, RoboCompAGM2::Node &dst);
	/// Converts an AGM symbol model from a RoboCompAGM2::Node container to an AGMModelSymbol container.
	static void fromIceToInternal(const RoboCompAGM2::Node &node, AGMModelSymbol::SPtr &dst);
	
	///Edges
	/// Converts an AGM edge model from an AGMModelEdge container (given by a shared pointer) to a RoboCompAGM2::Edge container.
// 	static void fromInternalToIce(const AGMModelEdge::SPtr &edge, RoboCompAGM2::Edge &dst);
	/// Converts an AGM edge model from an AGMModelEdge container (given by a regular pointer) to a RoboCompAGM2::Edge container.
	static void fromInternalToIce(const AGMModelEdge *edge, RoboCompAGM2::Edge &dst);
	/// Converts an AGM symbol model from a RoboCompAGM2::Edge container to an AGMModelEdge container.
	static void fromIceToInternal(const RoboCompAGM2::Edge &edge, AGMModelEdge &dst);


	/// Updates an AGM world model (an AGMModel object) using an updated node (given a RoboCompAGM2::Node instance).
	static bool includeIceModificationInInternalModel(const RoboCompAGM2::Node &node, AGMModel::SPtr &world);
	static bool includeIceModificationInInternalModel(const std::vector<RoboCompAGM2::Node> &nodes, AGMModel::SPtr &world);

	/// Updates an AGM world model (an AGMModel object) using an updated edge (given a RoboCompAGM2::Edge instance).
	static bool includeIceModificationInInternalModel(const RoboCompAGM2::Edge &edge, AGMModel::SPtr &world);
	static bool includeIceModificationInInternalModel(const std::vector<RoboCompAGM2::Edge> &edges, AGMModel::SPtr &world);
#endif

	/// Creates an AGM world model (AGMModel object) given the path to an existing XML description.
	static void fromXMLToInternal(const std::string path, AGMModel::SPtr &dst);


};


