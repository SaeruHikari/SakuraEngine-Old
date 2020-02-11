/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-11 17:06:04
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-11 23:40:46
 */
#include "UnitTestDependencyGraph.h"

//struct first_name_t {
//    using kind = vertex_property_tag;
//};

int main(void)
{
    using vertexProp = property<first_name_t, std::string>;
    using Graph = DAG::Graph<vertexProp>;
    using GraphVertex = DAG::GraphVertex<vertexProp>;
    using intE = std::pair<int, int>;

    intE edge_array[1] = {intE(1, 2)};
    Graph g(edge_array, 
        edge_array + sizeof(edge_array) / sizeof(intE), 2);
   
    boost::add_edge(3, 1, g);
    boost::add_edge(2, 1, g);
    boost::add_edge(3, 2, g);
    boost::add_edge(2, 3, g);
    boost::add_edge(5, 3, g);
    
    property_map<Graph, first_name_t>::type
        val = get(first_name_t(), g);
    boost::put(val, 1, "WTF");
    val[2] = " is this magic?";
    boost::put(val, 3, "New Guy");
    
    GraphVertex vert(3);

    auto src = DAG::num_edges(g);
    std::cout << DAG::out_degree(vert, g);
    std::cout << DAG::in_degree(vert, g);
    std::cout << DAG::out_degree(DAG::vertex(5, g), g);
    
    using IndexMap = property_map<Graph, vertex_index_t>::type;
    IndexMap index = get(vertex_index, g);
    auto inedges = DAG::in_edges(vert, g);//2, 5
    for(auto iter = inedges.first; iter!=inedges.second; iter++)
    {
        std::cout << std::endl << DAG::source(*iter, g);
    }
    auto neigs = DAG::adjacent_vertices(vert, g);
    for(auto iter = neigs.first; iter!=neigs.second; iter++)
    {
        std::cout << std::endl << DAG::vertex_number(*iter, g);
    }
    auto inv_neigs = DAG::inv_adjacent_vertices(vert, g);
    for(auto iter = inv_neigs.first; iter!=inv_neigs.second; iter++)
    {
        std::cout << std::endl << 
            DAG::get_vertex_property<first_name_t>(*iter, g);
    }
    
    std::cout << std::endl << std::endl;
    who_owes_who(edges(g).first, edges(g).second, g);

    return 0;
}