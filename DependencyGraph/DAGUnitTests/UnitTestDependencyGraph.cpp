/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Copyright SaeruHikari
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-11 17:06:04
 * @LastEditors: SaeruHikari
 * @LastEditTime: 2020-02-26 10:04:47
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

    Graph g(5);
   
    boost::add_edge(3, 1, g);
    boost::add_edge(2, 1, g);
    boost::add_edge(3, 2, g);
    boost::add_edge(2, 3, g);
    boost::add_edge(5, 3, g);
    std::cout << "num vertices: " << boost::num_vertices(g);
    
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


using namespace boost;
int _main(int,char*[])
{
    using vertexProp = property<first_name_t, std::string>;
    //typedef adjacency_list<vecS, vecS, bidirectionalS, vertexProp> Graph;
    using Graph = DAG::Graph<vertexProp>;
    // Make convenient labels for the vertices
    enum { A, B, C, D, E, N }; //代表 0 ，1，2，3，4 顶点,其中N为顶点数
    const int num_vertices = N;//N的值是5
    const char* name = "ABCDE";
    //图中的边
    typedef std::pair<int, int> Edge;
    Edge edge_array[] = { Edge(A,B), Edge(A,D), Edge(C,A), Edge(D,C),
                            Edge(C,E), Edge(B,D), Edge(D,E) };
    const int num_edges = sizeof(edge_array)/sizeof(edge_array[0]);
    // 创建一个拥有5个顶点的图对象
    Graph g(num_vertices);
    // 给图对象添加边
    for (int i = 0; i < num_edges; ++i)
      add_edge(edge_array[i].first, edge_array[i].second, g);//其中first表示第一个顶点，second表示第二个顶点，两个顶点连接
    std::cout << "num vertices: " << boost::num_vertices(g) << std::endl;
    return 0;
}
 