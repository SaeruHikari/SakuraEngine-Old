#pragma once
#include <algorithm>
#include <queue>
#include <cmath>
#include <numeric>
#include "gsl/span"
namespace core
{
	namespace algo
	{
		template<class Point>
		class kdtree
		{
			using Distance = typename Point::value_type;
		public:
			kdtree() {}
			kdtree(std::vector<Point>&& points)
				: points(points) 
			{
				std::vector<int> indices(points.size());
				std::iota(indices.begin(), indices.end(), 0);
				nodes.reserve(points.size());
				build_recursive(indices, 0);
			}

			void initialize(std::vector<Point>&& inPoints)
			{
				points = std::move(inPoints);
				std::vector<int> indices(points.size());
				std::iota(indices.begin(), indices.end(), 0);
				nodes.reserve(points.size());
				build_recursive(indices, 0);
			}
			
			const Point& operator[](size_t i) const
			{
				return points[i];
			}

			void search_radius(const Point& query, Distance radius, std::vector<int>& indices) const
			{
				if (nodes.empty())
					return;
				search_radius_recursive(query, &nodes[0], indices, radius * radius);
			}
			using sorted_vec = std::vector<std::pair<Distance, int>>;
			void search_k_radius(const Point& query, Distance radius, int k, sorted_vec& indices) const
			{
				if (nodes.empty())
					return;
				search_k_radius_recursive(query, &nodes[0], indices, radius * radius, k);
			}
			int search_nearest(const Point& query) const
			{
				if (nodes.empty())
					return -1;
				int index = -1;
				Distance nearestDist = std::numeric_limits<Distance>::max();
				search_nearest_recursive(query, &nodes[0], index, nearestDist);
				return index;
			}
		private:
			struct node
			{
				int index = -1;
				node* children[2] = { nullptr, nullptr };
				int axis = -1;
			};

			node* build_recursive(gsl::span<int> indices, int depth)
			{
				if (indices.empty())
					return nullptr;
				const int axis = depth % Point::dim;
				const size_t mid = (indices.size() - 1) / 2;
				std::nth_element(indices.begin(), indices.begin() + mid, indices.end(), [&](int lhs, int rhs)
					{
						return points[lhs][axis] < points[rhs][axis];
					});
				nodes.push_back({});
				node& n = nodes.back();
				n.index = indices[mid];
				n.axis = axis;
				n.children[0] = build_recursive({ indices.data(), mid }, depth + 1);
				n.children[1] = build_recursive({ indices.data() + mid + 1, indices.size() - mid - 1 }, depth + 1);
				return &n;
			}

			static Distance sdistance(const Point& lhs, const Point& rhs)
			{
				Distance dist = 0;
				for (size_t i = 0; i < Point::dim; i++)
					dist += (lhs[i] - rhs[i]) * (lhs[i] - rhs[i]);
				return dist;
			}

			void search_radius_recursive(const Point& query, const node* n, std::vector<int>& indices, Distance sradius) const
			{
				if (n == nullptr)
					return;
				const Point& train = points[n->index];
				const Distance sdist = sdistance(query, train);
				if (sdist < sradius)
					indices.push_back(n->index);
				const int axis = n->axis;
				const Distance axisDist = query[axis] - train[axis];
				const int child = axisDist < 0 ? 0 : 1;
				search_radius_recursive(query, n->children[child], indices, sradius);
				if(axisDist * axisDist < sradius) // crossing
					search_radius_recursive(query, n->children[1 - child], indices, sradius);
			}
			void search_k_radius_recursive(const Point& query, const node* n, sorted_vec& queue, Distance sradius, int k) const
			{
				if (n == nullptr)
					return;
				const Point& train = points[n->index];
				const Distance sdist = sdistance(query, train);
				if (sdist < (queue.empty() ? sradius : queue.back().first))
				{
					auto pair = std::make_pair(sdist, n->index);
					if (queue.size() == k)
						queue.pop_back();
					auto it = std::upper_bound(queue.begin(), queue.end(), pair);
					queue.insert(it, pair);
				}
				const int axis = n->axis;
				const Distance axisDist = query[axis] - train[axis];
				const int child = axisDist < 0 ? 0 : 1;
				search_k_radius_recursive(query, n->children[child], queue, sradius, k);
				if (axisDist * axisDist < (queue.empty() ? sradius : queue.back().first)) // crossing
					search_k_radius_recursive(query, n->children[1 - child], queue, sradius, k);
			}
			void search_nearest_recursive(const Point& query, const node* n, int& index, Distance& nearestDist) const
			{
				if (n == nullptr)
					return;
				const Point& train = points[n->index];
				const Distance sdist = sdistance(query, train);
				if (sdist < nearestDist)
				{
					index = n->index;
					nearestDist = sdist;
				}
				const int axis = n->axis;
				const Distance axisDist = query[axis] - train[axis];
				const int child = axisDist < 0 ? 0 : 1;
				search_nearest_recursive(query, n->children[child], index, nearestDist);
				if (axisDist * axisDist < nearestDist) // crossing
					search_nearest_recursive(query, n->children[1 - child], index, nearestDist);
			}
			std::vector<node> nodes;
			std::vector<Point> points;
		};
	}
}