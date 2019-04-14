/*

Copyright (C) 2018-2019 Matthew Milton

This file is part of the LB-LMC Solver C++ Library.

LB-LMC Solver C++ Library is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

LB-LMC Solver C++ Library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with LB-LMC Solver C++ Library.  If not, see <https://www.gnu.org/licenses/>.

*/

#include "SystemSourceVector.hpp"

#include <cstdlib>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

namespace LBLMC
{

SystemSourceVector::SystemSourceVector(unsigned int dimension) :
	vector(dimension, std::vector<long>()), source_nodes(), dimension(dimension), src_index(0)
{
	//do nothing else
}

SystemSourceVector::SystemSourceVector(const SystemSourceVector& base) :
	vector(base.vector), source_nodes(base.source_nodes), dimension(base.dimension),
	src_index(base.src_index)
{
	//do nothing else
}

void SystemSourceVector::reset(unsigned int dimension)
{
	vector = std::vector<std::vector<long> >(dimension, std::vector<long>());
	source_nodes.clear();
	this->dimension = dimension;
	src_index = 0;
}

void SystemSourceVector::reset(const SystemSourceVector& base)
{
	vector = base.vector;
	source_nodes = base.source_nodes;
	dimension = base.dimension;
	src_index = base.src_index;
}

std::vector<long>& SystemSourceVector::asVector(unsigned int n)
{
	return vector[n-1];
}

std::map<long, std::vector<long> >& SystemSourceVector::asMap()
{
	return source_nodes;
}

unsigned int SystemSourceVector::getDimension() const
{
	return dimension;
}

unsigned int SystemSourceVector::getNumSources() const
{
	return src_index;
}

unsigned int SystemSourceVector::insertSource(unsigned int npos, unsigned int nneg)
{
	if(npos == nneg) return 0;

	++src_index;

	if(npos != 0)
	{
		vector[npos-1].push_back(+src_index);
	}
	if(nneg != 0)
	{
		vector[nneg-1].push_back(-src_index);
	}

	source_nodes[src_index].push_back(npos);
	source_nodes[src_index].push_back(nneg);

	return src_index;
}

std::vector<unsigned int> SystemSourceVector::insertComponents(std::vector<unsigned int> nodes)
{
    if(nodes.empty() || (nodes.size()%2 != 0) ) return std::vector<unsigned int>();

        //check if any source is shorted (npos == nneg)
    for(unsigned int i = 0; i < nodes.size(); i+=2)
    {
        if( nodes[i] == nodes[i+1] ) return std::vector<unsigned int>();
    }

    std::vector<unsigned int> indices = std::vector<unsigned int>();

        //insert the sources, via their nodes, into the source vector
    for(unsigned int i = 0; i < nodes.size(); i+=2)
    {
        indices.push_back( insertSource( nodes[i], nodes[i+1] ) );
    }

    return indices;
}

const char* SystemSourceVector::asString(std::string& buffer)
{
	std::stringstream sstrm;

	for(unsigned int i = 0; i < dimension; i++)
	{
		if(vector[i].empty())
		{
			sstrm << i+1 << ": 0\n";
			continue;
		}

		sstrm << i+1 << ": ";

		std::vector<long>::iterator iter = vector[i].begin();
		std::vector<long>::iterator end  = vector[i].end();
		for(iter; iter != end; iter++)
		{
			sstrm << *iter << " ";
		}

		sstrm << "\n";
	}

	buffer = sstrm.str();
	return buffer.c_str();
}

const char* SystemSourceVector::asCFunction(std::string& buffer, const char* func_name)
{
	std::stringstream sstrm;

	sstrm <<
	//"void " << func_name << "(NumType* b, NumType* b_components)\n"
	"void " << func_name << "(LBLMC::NumType b["<<dimension<<"], LBLMC::NumType b_components["<<src_index<<"])\n"
	"{\n\t";

	for(unsigned int i = 0; i < dimension; i++)
	{
		if(vector[i].empty())
		{
			sstrm << "b[" << i << "] = 0.0;\n\t";
			continue;
		}

		sstrm << "b[" << i << "] = ";

		std::vector<long>::iterator iter = vector[i].begin();
		std::vector<long>::iterator end  = vector[i].end();
		for(iter; iter != end; iter++)
		{
			if( (*iter) >= 0)
			{
				sstrm << " b_components[" << long(abs(*iter)-1) << "] ";
			}
			else
			{
				sstrm << " -b_components[" << long(abs(*iter)-1) << "] ";
			}

			if((iter+1) == end) sstrm << ";\n\t";
			else sstrm << "+";
		}

		//sstrm << "0.0;\n\t";
	}

	sstrm << "\n}";

	buffer = sstrm.str();
	return buffer.c_str();
}

int SystemSourceVector::exportAsCFunctionSource(const char* dir, const char* filename, const char* func_name)
{
	std::fstream file;
	std::fstream header;
	std::fstream source;

	std::string hname = dir; hname+=filename; hname += ".hpp";
	std::string sname = dir; sname+=filename; sname += ".cpp";

	try
	{
		//file.open(filename, std::fstream::out | std::fstream::trunc);
		header.open((hname).c_str(), std::fstream::out | std::fstream::trunc);
		source.open((sname).c_str(), std::fstream::out | std::fstream::trunc);
	}
	catch(...)
	{
		header.close();
		source.close();
		return -1;
	}

	header <<
			"/**\n"
			" *\n"
			" * LBLMC Vivado HLS Simulation Engine for FPGA Designs\n"
			" *\n"
			" * Auto-generated by SystemSourceVector Object\n"
			" *\n"
			" */\n\n";

	header << "#ifndef " << func_name << "_HPP\n";
	header << "#define " << func_name << "_HPP\n\n";
	header << "\n#include \"LBLMC/DataTypes.hpp\"\n\n";
	//header << "void " << func_name << "(NumType* b, NumType* b_components);\n\n";
	header << "void " << func_name << "(LBLMC::NumType b["<<dimension<<"], LBLMC::NumType b_components["<<src_index<<"]);\n\n";
	header << "#endif";
	header.close();

	source << "#include \"" << filename<< ".hpp" << "\"\n\n";

	std::string buf;
	source << asCFunction(buf,func_name);
	source.close();

	return 0;
}

} //namespace LBLMC
