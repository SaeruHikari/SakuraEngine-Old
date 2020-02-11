/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-11 17:06:04
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-11 20:45:03
 */
#include "UnitTestDependencyGraph.h"

//struct first_name_t {
//    using kind = vertex_property_tag;
//};

int main(void)
{
    using vertexProp = property<first_name_t, std::string>;
    using Graph = Sakura::Graph<vertexProp>;
    using intE = std::pair<int, int>;

    intE edge_array[1] = {intE(1, 2)};
    Graph g(edge_array, 
        edge_array + sizeof(edge_array) / sizeof(intE), 2);
    boost::add_edge(3, 1, g);
    boost::add_edge(2, 1, g);
    property_map<Graph, first_name_t>::type
        val = get(first_name_t(), g);
    boost::put(val, 1, "WTF");
    boost::put(val, 3, "New Guy");

    val[2] = " is this magic?";

    who_owes_who(edges(g).first, edges(g).second, g);

    return 0;
}