#include <agm_misc_functions.h>
#include <agm_modelConverter.h>
#include <agm_modelPrinter.h>

#include <algorithm>

float str2float(const std::string &s, bool debug)
{
	if (s.size()<=0)
	{
		AGMMODELEXCEPTION("libagm: Error parsing float <empty>\n");
	}

	if (debug) printf("s1 %s\n", s.c_str());
	float ret;
	std::string str = s;
	replace(str.begin(), str.end(), ',', '.');
	if (debug) printf("s2 %s\n", str.c_str());
	std::istringstream istr(str);
	istr.imbue(std::locale("C"));
	istr >> ret;
	return ret;
}

int32_t str2int(const std::string &s)
{
	if (s.size()<=0)
	{
		AGMMODELEXCEPTION("libagm: Error parsing int <empty>\n");
	}

	int32_t ret;
	std::string str = s;
	replace(str.begin(), str.end(), ',', '.');
	std::istringstream istr(str);
	istr.imbue(std::locale("C"));
	istr >> ret;
	return ret;
}


std::string float2str(const float &f)
{
	std::ostringstream ostr;
	ostr.imbue(std::locale("C"));
	ostr << f;
	return ostr.str();
}

std::string int2str(const int32_t &i)
{
	std::ostringstream ostr;
	ostr.imbue(std::locale("C"));
	ostr << i;
	return ostr.str();
}


#if ROBOCOMP_SUPPORT == 1

void AGMMisc::publishModification(AGMModel::SPtr &newModel, AGMDSRServicePrx &agmdsr, std::string sender, std::string log)
{
	RoboCompAGM2::StructuralChangeStruct structuralChangeStruct;
	// Prepare .world
	newModel->removeDanglingEdges();	
	AGMModelConverter::fromInternalToIce(newModel, structuralChangeStruct.model);
	// Parepare .changes
	// Prepare .sender
	structuralChangeStruct.sender = sender;
	// Prepare .log
	structuralChangeStruct.log = log;

	int ret;
#warning "We should here return the corresponding exception, depending on the value returned"
	agmdsr->structuralChangeProposal(structuralChangeStruct, ret);
}

void AGMMisc::publishNodeUpdate(AGMModelSymbol::SPtr &symbol, AGMDSRServicePrx &agmdsr)
{
	RoboCompAGM2::Node iceSymbol;
	AGMModelConverter::fromInternalToIce(symbol, iceSymbol);
	
	int ret;
#warning "We should here return the corresponding exception, depending on the value returned"
	agmdsr->symbolUpdate(iceSymbol, ret);
}

void AGMMisc::publishNodesUpdate(std::vector<AGMModelSymbol::SPtr> symbols, AGMDSRServicePrx &agmdsr)
{
	RoboCompAGM2::NodeSequence symbol_sequence;
	for (std::vector<AGMModelSymbol::SPtr>::iterator it=symbols.begin();it != symbols.end(); it++)
	{
		RoboCompAGM2::Node iceSymbol;
		AGMModelConverter::fromInternalToIce(*it, iceSymbol);
		symbol_sequence.push_back(iceSymbol);
	}

	int ret;
#warning "We should here return the corresponding exception, depending on the value returned"
	agmdsr->symbolsUpdate(symbol_sequence, ret);
}

void AGMMisc::publishEdgeUpdate(AGMModelEdge &edge, AGMDSRServicePrx &agmdsr)
{
	RoboCompAGM2::Edge iceEdge;
	AGMModelConverter::fromInternalToIce(&edge, iceEdge);

	int ret;
#warning "We should here return the corresponding exception, depending on the value returned"
	agmdsr->edgeUpdate(iceEdge, ret);
}

void AGMMisc::publishEdgesUpdate(std::vector<AGMModelEdge> edges, AGMDSRServicePrx &agmdsr)
{
	RoboCompAGM2::EdgeSequence edge_sequence;
	for (std::vector<AGMModelEdge>::iterator it=edges.begin();it != edges.end(); it++)
	{
		RoboCompAGM2::Edge iceEdge;
		AGMModelConverter::fromInternalToIce(&(*it), iceEdge);
		edge_sequence.push_back(iceEdge);
	}

	int ret;
#warning "We should here return the corresponding exception, depending on the value returned"
	agmdsr->edgesUpdate(edge_sequence, ret);
}

#endif


float AGMMisc::str2float(const std::string &s, bool debug)
{
	const std::string st = s;
	if (debug) printf("s %s\n", st.c_str());
	const float f = ::str2float(s, debug);
	if (debug) printf("f %f\n", f);
	return f;
}


int32_t AGMMisc::str2int(const std::string &s)
{
	return ::str2int(s);
}


std::string AGMMisc::float2str(const float &f)
{
	return ::float2str(f);
}


std::string AGMMisc::int2str(const int32_t &i)
{
	return ::int2str(i);
}

std::map<std::string, std::string> AGMMisc::stringVector2stringMap(RoboCompAGM2::StringDictionary input)
{
	std::map<std::string, std::string> ret;
	for (auto &e: input)
	{
		ret[e.first] = e.second;
	}
	return ret;
}

RoboCompAGM2::StringDictionary AGMMisc::stringMap2stringVector(std::map<std::string, std::string> input)
{
	RoboCompAGM2::StringDictionary ret;
	for (auto &kv: input)
	{
		RoboCompAGM2::StringPair p;
		p.first = kv.first;
		p.second = kv.second;
		ret.push_back(p);
	}
	return ret;
}



