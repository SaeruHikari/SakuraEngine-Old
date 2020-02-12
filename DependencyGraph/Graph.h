/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Wrapper of boost Graph library
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-11 17:28:48
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-11 23:40:35
 */
// Source page:
//   boost.org/doc/libs/1_61_0/libs/graph/doc/
//    using_adjacency_list.html#sec:adjacency-list-properties
#pragma once
#include <memory_resource>
#include <vector>
#include <boost/graph/adjacency_list.hpp>

namespace DAG
{
    namespace ____
    {
        struct pmrVecS{};
        struct pmrListS{};
    }

    template <class VertexProperty = boost::no_property,
        class EdgeProperty = boost::no_property,
        class GraphProperty = boost::no_property,
        class EdgeListS = boost::listS>
    using Graph = boost::adjacency_list<____::pmrVecS, ____::pmrVecS, 
        boost::bidirectionalS,
        VertexProperty, EdgeProperty, GraphProperty, EdgeListS>;
    
    template <class VertexProperty = boost::no_property,
        class EdgeProperty = boost::no_property,
        class GraphProperty = boost::no_property,
        class EdgeListS = boost::listS>
    using GraphVertex = 
    typename boost::graph_traits<Graph<VertexProperty, GraphProperty, EdgeListS>>::
                vertex_descriptor;

    template <class VertexProperty = boost::no_property,
        class EdgeProperty = boost::no_property,
        class GraphProperty = boost::no_property,
        class EdgeListS = boost::listS>
    using GraphEdge = 
    typename boost::graph_traits<Graph<VertexProperty, GraphProperty, EdgeListS>>::
                edge_descriptor;


    template <class VertexProperty = boost::no_property,
        class EdgeProperty = boost::no_property,
        class GraphProperty = boost::no_property,
        class EdgeListS = boost::listS>
    using MutaGraph = boost::adjacency_list<____::pmrListS, ____::pmrListS, 
        boost::bidirectionalS,
        VertexProperty, EdgeProperty, GraphProperty, EdgeListS>;

    template <class VertexProperty = boost::no_property,
        class EdgeProperty = boost::no_property,
        class GraphProperty = boost::no_property,
        class EdgeListS = boost::listS>
    using MutaGraphVertex = 
    typename boost::graph_traits<MutaGraph<VertexProperty, GraphProperty, EdgeListS>>::
                vertex_descriptor;

    template <class VertexProperty = boost::no_property,
        class EdgeProperty = boost::no_property,
        class GraphProperty = boost::no_property,
        class EdgeListS = boost::listS>
    using MutaGraphEdge = 
    typename boost::graph_traits<MutaGraph<VertexProperty, GraphProperty, EdgeListS>>::
                edge_descriptor;

    // returns the num of edges in the graph
    using boost::num_edges;
    // returns the num of vertices in the graph 
    using boost::num_vertices;
    
    // returns the in degree of a vertex
    using boost::in_degree;
    // returns the out degree of a vertex
    using boost::out_degree;
    
    // returns the target vertex of a specific edge
    using boost::target;
    // returns the source vertex of a specific edge
    using boost::source;
    
    // returns the no. vertex in the graph
    using boost::vertex;
    using boost::vertices;

    // returns the edge between A and B
    // if no active edge the bool val in pair will be false
    using boost::edge;
    using boost::edges;

    using boost::in_edges;
    using boost::out_edges;

    using boost::edge_range;

    using boost::adjacent_vertices;
    using boost::inv_adjacent_vertices;

    using boost::add_edge;
    using boost::remove_edge;
    
    using boost::add_vertex;
    using boost::clear_vertex;
    using boost::clear_out_edges;
    using boost::clear_in_edges;
    using boost::remove_vertex;

    template<typename... Ts,
        typename bidirGraph = boost::adjacency_list<Ts...>,
        typename bidirVertex = typename bidirGraph::vertex_descriptor,
        typename IndexMap = 
        typename boost::property_map<bidirGraph, boost::vertex_index_t>::type>
    auto vertex_number(bidirVertex vert, bidirGraph g)
    {
        IndexMap index = get(boost::vertex_index, g);
        return index[vert];
    }

    template<typename prop_name_t, typename... Ts,
        typename bidirGraph = boost::adjacency_list<Ts...>,
        typename bidirVertex = typename bidirGraph::vertex_descriptor,
        typename PropMap = 
        typename boost::property_map<bidirGraph, prop_name_t>::type>
    auto get_vertex_property(bidirVertex vert, bidirGraph g)
    {
        PropMap prop = get(prop_name_t(), g);
        return prop[vert];
    }
    
    template<typename prop_name_t, typename... Ts,
        typename bidirGraph = boost::adjacency_list<Ts...>,
        typename bidirEdge = typename bidirGraph::edge_descriptor,
        typename PropMap = 
        typename boost::property_map<bidirGraph, prop_name_t>::type>
    auto get_edge_property(bidirEdge vert, bidirGraph g)
    {
        PropMap prop = get(prop_name_t(), g);
        return prop[vert];
    }
}


namespace boost
{
    template<class T>
    struct container_gen<DAG::____::pmrVecS, T> 
    {
        using type = std::pmr::vector<T> ;
    };

    template<class T>
    struct container_gen<DAG::____::pmrListS, T>
    {
        using type = std::pmr::list<T>;
    };

    template<>
    struct parallel_edge_traits<DAG::____::pmrVecS> 
    {
        using type = allow_parallel_edge_tag;
    };

    template<>
    struct parallel_edge_traits<DAG::____::pmrListS>
    {
        using type = allow_parallel_edge_tag;
    };

    namespace detail
    {
        template<>
        struct is_random_access<DAG::____::pmrVecS> 
        {
            enum { value = true };
            using type = mpl::true_;
        };

        template<>
        struct is_random_access<DAG::____::pmrListS>
        {
            enum { value = true };
            using type = mpl::true_;
        };
    }
}


  

