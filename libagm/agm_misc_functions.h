#pragma once

#include <agm_config.h>
#include <agm_model.h>
#include <agm_modelException.h>

#if ROBOCOMP_SUPPORT == 1
#include <AGM2.h>
using namespace RoboCompAGM2;
#endif

/*! Converts an STD string to float. */
float str2float(const std::string &s, bool debug = false);
/*! Converts an STD string to integer. */
int32_t str2int(const std::string &s);


/*! Converts a float to an STD string. */
std::string float2str(const float &f);
/*! Converts an integer to an STD string. */
std::string int2str(const int32_t &i);




/*!
 * \class AGMMisc
 * @ingroup CPPAPI
 * @brief Class containing several useful functions.
 *
 * Class containing several useful functions.
 * 
 */
class AGMMisc
{
public:
	static std::map<std::string, std::string> stringVector2stringMap(RoboCompAGM2::StringDictionary input);
	static RoboCompAGM2::StringDictionary stringMap2stringVector(std::map<std::string, std::string> input);
#if ROBOCOMP_SUPPORT == 1
	/*! Proposes a new world model (<em>newModel</em>) using the proxy <em>AGMExecutive</em> provinding <em>sender</em> as additional information that might be interesting for debugging purposes. */
	static void publishModification(AGMModel::SPtr &newModel, AGMDSRServicePrx &agmdsr, std::string sender="unspecified", std::string log="no log information");
	/*! Modifies a node (<em>symbol</em>) using the proxy <em>AGMExecutive</em>. */
	static void publishNodeUpdate(AGMModelSymbol::SPtr &symbol, AGMDSRServicePrx &agmdsr);
	/*! Modifies several nodes (<em>symbol</em>) using the proxy <em>AGMExecutive</em>. */
	static void publishNodesUpdate(std::vector<AGMModelSymbol::SPtr> symbols, AGMDSRServicePrx &agmdsr);
	/*! Modifies an edge (<em>edge</em>) using the proxy <em>AGMExecutive</em>. */
	static void publishEdgeUpdate(AGMModelEdge &edge, AGMDSRServicePrx &agmdsr);
	/*! Modifies several edges (<em>edge</em>) using the proxy <em>AGMExecutive</em>. */
	static void publishEdgesUpdate(std::vector<AGMModelEdge> edges, AGMDSRServicePrx &agmdsr);
#endif
	static inline float str2float(const std::string &s, bool debug = false);
	static inline int32_t str2int(const std::string &s);
	static inline std::string float2str(const float &f);
	static inline std::string int2str(const int32_t &i);
};

