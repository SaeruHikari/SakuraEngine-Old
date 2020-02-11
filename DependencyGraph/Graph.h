/*
 * @This File is Part of Sakura by SaeruHikari: 
 * @Description: Wrapper of boost Graph library
 * @Version: 0.1.0
 * @Autor: SaeruHikari
 * @Date: 2020-02-11 17:28:48
 * @LastEditors  : SaeruHikari
 * @LastEditTime : 2020-02-11 20:36:42
 */
// Source page:
//   boost.org/doc/libs/1_61_0/libs/graph/doc/
//    using_adjacency_list.html#sec:adjacency-list-properties
#pragma once
#include <memory_resource>
#include <vector>
#include <boost/graph/adjacency_list.hpp>

namespace Sakura
{
    struct pmrVecS{};
    struct pmrListS{};
    template <class VertexProperty = boost::no_property,
        class EdgeProperty = boost::no_property,
        class GraphProperty = boost::no_property,
        class EdgeListS = boost::listS>
    using Graph = boost::adjacency_list<pmrVecS, pmrVecS, boost::bidirectionalS,
        VertexProperty, EdgeProperty, GraphProperty, EdgeListS>;
    
    template <class VertexProperty = boost::no_property,
        class EdgeProperty = boost::no_property,
        class GraphProperty = boost::no_property,
        class EdgeListS = boost::listS>
    using GraphMuta = boost::adjacency_list<pmrListS, pmrListS, boost::bidirectionalS,
        VertexProperty, EdgeProperty, GraphProperty, EdgeListS>;
}

namespace boost
{
    template<class T>
    struct container_gen<Sakura::pmrVecS, T> 
    {
        using type = std::pmr::vector<T> ;
    };

    template<class T>
    struct container_gen<Sakura::pmrListS, T>
    {
        using type = std::pmr::list<T>;
    };

    template<>
    struct parallel_edge_traits<Sakura::pmrVecS> 
    {
        using type = allow_parallel_edge_tag;
    };

    template<>
    struct parallel_edge_traits<Sakura::pmrListS>
    {
        using type = allow_parallel_edge_tag;
    };

    namespace detail
    {
        template<>
        struct is_random_access<Sakura::pmrVecS> 
        {
            enum { value = true };
            using type = mpl::true_;
        };

        template<>
        struct is_random_access<Sakura::pmrListS>
        {
            enum { value = true };
            using type = mpl::true_;
        };
    }
}


  

