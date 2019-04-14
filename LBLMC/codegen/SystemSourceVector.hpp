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

#ifndef SYSTEMSOURCEVECTOR_HPP
#define SYSTEMSOURCEVECTOR_HPP

#include <vector>
#include <map>
#include <string>
#include "LBLMC/DataTypes.hpp"

namespace LBLMC
{

/**
 * @brief encapsulates information of the source vector b of a system model in LB-LMC method
 *
 * This class is intended for use of generating functions that aggregate source contributions
 * and compute source vectors during online simulation.  The functions are generated offline and
 * can then be compiled into the source for a LB-LMC simulation engine of a model.
 *
 * Since the source vector b is a function, not a constant, this class stores the information needed
 * to generate the functions that compute the source vector.  This information is merely the indices
 * of the contributing sources in a given LB-LMC model and the nodes the sources are connected
 * across.  From this information, the particular source vector function can be generated, this
 * function in the general pseudocode form:
 * 	void computeSourceVectorb(NumType b[], NumType b_components[])
 * 	{
 * 		b[0] = +/-b_components[i] +/- b_components[j] +/- ... b_components[k];
 * 		...
 * 		b[s] = +/-b_components[l] +/- b_components[m] +/- ... b_components[n];
 * 	}
 *
 * 	where i,j,k,l,m,n are just indices of the source contributions from the model components and
 * 	s is the index of last element of the source vector.  If a source vector b element has no
 * 	source contributions, it is assigned zero.
 *
 * @note This class is NOT intended for RTL Synthesis.
 *
 *
 */
class SystemSourceVector
{
private:
	std::vector<std::vector<long> > vector; ///< vector of vectors that contains the indices of sources that contribute to the source vector b
	std::map<long,std::vector<long> > source_nodes; ///< map of index of source to source's nodes
	unsigned int dimension; ///< size of the source vector; number of solutions in system Gx=b
	unsigned int src_index; ///< tracks the current used source index

public:
	/**
	 * parameter constructor
	 * @param dimension size of the source vector; number of solutions in system Gx=b
	 */
	SystemSourceVector(unsigned int dimension);

	/**
	 * copy constructor
	 * @param base object to copy from
	 */
	SystemSourceVector(const SystemSourceVector& base);

	/**
	 * resets the source vector
	 * @param dimension size of the source vector; number of solutions in system Gx=b
	 */
	void reset(unsigned int dimension);

	/**
	 * resets the source vector to be copy of another source vector
	 * @param base object to copy from
	 */
	void reset(const SystemSourceVector& base);

	/**
	 * returns the vector of indices of sources contributing to particular vector-element in source vector indexed by argument n
	 * @param n the nonzero (>0) index of the source vector vector-element whose index vector is returned
	 * @return the index vector of sources contributing to source vector element n
	 */
	std::vector<long>& asVector(unsigned int n);

	/**
	 * returns an ordered C++ map of the source indices and their node indices
	 * map: src_index -> {pos_node_index, neg_node_index}
	 * @return reference to the ordered map
	 */
	std::map<long, std::vector<long> >& asMap();

	/**
	 * @return the dimension (number of solutions in Gx=b) of the source vector
	 */
	unsigned int getDimension() const;

	/**
	 * @return the number of sources inserted in the source vector
	 */
	unsigned int getNumSources() const;

	/**
	 * inserts a contributing source's index into the source vector between given nodes
	 * @param npos positive node of the source
	 * @param nneg negative node of the source
	 * @return the index of the inserted source; 0 if source not inserted from having no effect (npos==nneg)
	 */
	unsigned int insertSource(unsigned int npos, unsigned int nneg);


    /**
        @brief inserts the indices of several contributing sources from components, into the source vector

        This method takes the nodes of contributing sources from components and inserts these
        sources' source-indices into the source vector.

        This method can stamp one or more components into the source vector if their source node indices are in the given nodes vector

        @param nodes a vector of node-index pairs that indicate how the indices of sources of (a) component(s) are to be stamped into source vector

        nodes vector must have an even number of node indices as each source has 2 nodes each.

        @return vector of indices for the inserted components, in order as in nodes; empty vector if any source has no effect (npos==nneg)
    **/
	std::vector<unsigned int> insertComponents(std::vector<unsigned int> nodes);

	/**
	 * creates a table, as a string, of the contributing source indices corresponding to each source vector element
	 * @param buffer string that will store the table
	 * @return the buffer string as a const char* string
	 */
	const char* asString(std::string& buffer);

	/**
	 * Generates a C/C++ function, as a compilable string, that aggregates/computes the source vector b from array of source contributions
	 * The generated function is created from the indices stored in this object.
	 * @param buffer string that will store the source code for the function
	 * @param func_name the name of the generated function
	 * @return buffer string as a const char* string
	 */
	const char* asCFunction(std::string& buffer, const char* func_name="aggregateSources");

	/**
	 * Generates the C/C++ source code for a function that aggregates/computes the source vector b from array of given source contributions
	 * The generated function is created from the indices stored in this object.
	 * @param dir existing directory to store source files in, written as "/dir/loc/"; input "" for local directory of calling program; given directory must already exist!
	 * @param filename filename of the source code files, without extension
	 * @param func_name the name of the generated function; default is "aggregateSources"
	 * @return 0 if successful, -1 if fails
	 */
	int exportAsCFunctionSource(const char* dir, const char* filename, const char* func_name="aggregateSources");
};

} //namespace LBLMC

#endif // SYSTEMSOURCEVECTOR_HPP
