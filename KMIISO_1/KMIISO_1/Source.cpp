#include <utility>
#include <vector>
#include <deque>
#include <limits>
#include <fstream>
#include <string>
#include <iostream>
#include <cstdint>
#include <algorithm>
#include <random>

#include "profile.h"

struct Edge {
	size_t dest = -1;
	int weight = 0;
};

struct GraphInfo {
	std::vector<int> distances;
	std::vector<int> path;
};

using Graph = std::vector<std::vector<Edge>>;

const int infinity = std::numeric_limits<int>::max();

Graph ReadGraph(const std::string& filename = "graph.txt") {
	std::ifstream fin(filename);
	size_t verticeCount = 0;
	fin >> verticeCount;
	Graph toReturn(verticeCount);

	size_t from = 0;
	size_t to = 0;
	int weight = 0;
	while (!fin.eof()) {
		fin >> from >> to >> weight;
		toReturn[from - 1].push_back(Edge{to - 1, weight});
	}

	return toReturn; // NRVO
}

const int M = 30;
const int W = (int)8e8;

Graph gen()
{
	Graph g(91);
	for (int i = 0; i < M; i++)
		g[i].push_back(Edge{ (size_t)i + 1, 0 });
	g[0].push_back(Edge{ M, W });
	size_t n = M;

	for (int i = 0; i < M; i++)
	{
		g[n].push_back(Edge{ n + 2, W >> i });
		g[n].push_back(Edge{ n + 1, 0 });
		g[n + 1].push_back(Edge{ n + 2, 0 });
		n += 2;
	}
	n++;

	std::random_device rd;
	std::mt19937 gg(rd());

	for (int i = 0; i < n; i++)
		std::shuffle(g[i].begin(), g[i].end(), gg);

	return g; // NRVO
}

std::pair<size_t, size_t> EnterVertices() {
	size_t start = 0;
	size_t dest = 0;
	std::cout << "Enter start vertice: ";
	std::cin >> start;
	std::cout << "Enter destination vertice: ";
	std::cin >> dest;
	return std::make_pair(start, dest);
}

GraphInfo FindLowestDistanceExp(const Graph& graph, size_t start, size_t dest) {
	std::vector<int> d(graph.size(), infinity);
	d[start] = 0;
	std::vector<int> id(graph.size());
	std::deque<int> q;
	q.push_back(start);
	std::vector<int> p(graph.size(), -1);

	while (!q.empty())
	{
		int v = q.front();
		q.pop_front();
		id[v] = 1;
		for (size_t i = 0; i < graph[v].size(); ++i)
		{
			int to = graph[v][i].dest, len = graph[v][i].weight;
			if (d[to] > d[v] + len)
			{
				d[to] = d[v] + len;
				if (id[to] == 0)
					q.push_back(to);
				else if (id[to] == 1)
					q.push_front(to);
				p[to] = v;
				id[to] = 1;
			}
		}
	}

	return GraphInfo{std::move(d), std::move(p)};
}

GraphInfo FindLowestDistance(const Graph& graph, size_t start, size_t dest) {
	std::vector<int> distances(graph.size(), infinity);
	distances[start] = 0;
	
	std::vector<char> state(graph.size(), '2');
	state[start] = '1';

	std::vector<int> path(graph.size(), -1);

	std::deque<size_t> mainQueue; //M1'
	std::deque<size_t> urgentQueue; //M1''

	mainQueue.push_back(start);

	while (!mainQueue.empty() || !urgentQueue.empty()) {
		size_t tempVertStart = -1;
		if (!urgentQueue.empty()) {
			tempVertStart = urgentQueue.front();
			urgentQueue.pop_front();
		}
		else {
			tempVertStart = mainQueue.front();
			mainQueue.pop_front();
		}

		for (size_t i = 0; i < graph[tempVertStart].size(); i++) {
			size_t tempVertDest = graph[tempVertStart][i].dest; // v
			int tempLength = graph[tempVertStart][i].weight;
			if (state[tempVertDest] == '2') {
				distances[tempVertDest] = distances[tempVertStart] + tempLength;
				mainQueue.push_back(tempVertDest);
				state[tempVertDest] = '1';
				path[tempVertDest] = tempVertStart;
			} 
			else if (state[tempVertDest] == '1') {
				//distances[tempVertDest] = std::min(distances[tempVertDest], distances[tempVertStart] + tempLength);
				if (distances[tempVertDest] > distances[tempVertStart] + tempLength) {
					distances[tempVertDest] = distances[tempVertStart] + tempLength;
					path[tempVertDest] = tempVertStart;
				}
			}
			else { // state[tempVertDest] == '0'
				if (distances[tempVertDest] > distances[tempVertStart] + tempLength) {
					distances[tempVertDest] = distances[tempVertStart] + tempLength;
					urgentQueue.push_back(tempVertDest);
					state[tempVertDest] = '1';
					path[tempVertDest] = tempVertStart;
				}
			}
		}

		state[tempVertStart] = '0';
	}

	return GraphInfo{std::move(distances), std::move(path)}; // copy elision
}

void PrintInfo(const std::vector<int>& distances, const std::vector<int>& path, size_t start, size_t dest) {
	std::vector<int> correctPath;
	for (int vertex = dest; vertex != -1; vertex = path[vertex])
		correctPath.push_back(vertex);
	std::reverse(correctPath.begin(), correctPath.end());
	for (size_t i = 0; i < correctPath.size(); ++i)
		std::cout << correctPath[i] << ' ';

	if (distances[dest] != std::numeric_limits<int>::max()) {
		std::cout << "The lowest distance from "
			<< start << " to " << dest << " is "
			<< distances[dest] << std::endl;
	}
	else {
		std::cout << "There's no path from "
			<< start << " to " << dest << std::endl;
	}
}

int main() {
	//Graph graph = ReadGraph();
	Graph graph = gen();
	auto [start, dest] = EnterVertices();
	{
		LOG_DURATION("My algo");
		auto [distances, path] = FindLowestDistance(graph, start, dest);
		PrintInfo(distances, path, start, dest);
	}
	{
		LOG_DURATION("Exp algo");
		auto [distances, path] = FindLowestDistanceExp(graph, start, dest);
		PrintInfo(distances, path, start, dest);
	}
	
	return 0;
}