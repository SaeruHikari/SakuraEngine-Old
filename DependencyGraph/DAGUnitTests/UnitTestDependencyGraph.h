/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-11 19:29:17
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-11 20:23:23
 */
#pragma once
#include "../Graph.h"
#include <iostream>
using namespace boost;
  
struct first_name_t {
    using kind = vertex_property_tag;
};

template <typename EdgeIter, typename Graph>
void who_owes_who(EdgeIter first, EdgeIter last, const Graph& G)
{
    // Access the propety acessor type for this graph
    typedef typename property_map<Graph, 
      first_name_t>::const_type NameMap;
    NameMap name = get(first_name_t(), G);

    typedef typename boost::property_traits<NameMap>
      ::value_type NameType;

    NameType src_name, targ_name;

    while (first != last) {
      src_name = boost::get(name, source(*first, G));
      targ_name = boost::get(name, target(*first, G));
      std::cout << src_name << " owes " 
           << targ_name << " some money" << std::endl;
      ++first;
    }
}