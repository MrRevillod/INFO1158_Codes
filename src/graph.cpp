
#include "graph.hpp"
#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <stdexcept>
#include <string>

Color::Modifier red(Color::FG_RED);
Color::Modifier green(Color::FG_GREEN);
Color::Modifier blue(Color::FG_BLUE);
Color::Modifier def(Color::FG_DEFAULT);

void graph::bfs(vertex *start) {

    std::vector<vertex *> frontier;
    std::unordered_map<vertex *, bool> visited;
    std::vector<std::pair<vertex *, int>> paths;

    frontier.push_back(start);
    visited[start] = true;
    paths.push_back({start, 0});

    int i = 1;

    while (!frontier.empty()) {

        std::vector<vertex *> next;

        for (const vertex *v : frontier) {
            for (const auto edge : v->adj) {

                if (!visited[edge.first]) {
                    vertex *to = edge.first;
                    next.push_back(to);
                    paths.push_back({to, i});
                    visited[to] = true;
                }
            }
        }

        frontier = next;
        i++;
    }

    /* Mostrar recorrido y niveles del grafo */

    std::cout << " " << std::endl;
    std::cout << green << "Algoritmo: " << def << "BFS" << std::endl;
    std::cout << " " << std::endl;

    std::cout << "Recorrido en anchura:  [ ";

    for (const auto path : paths) {
        std::cout << path.first->name << " ";
    }

    std::cout << "]" << std::endl;
    std::cout << "Niveles del recorrido: [ ";

    for (const auto path : paths) {
        std::cout << path.second << " ";
    }

    std::cout << "]" << std::endl;
}

void graph::dfs(vertex *start) {

    std::unordered_map<vertex *, bool> visited;
    std::unordered_map<vertex *, vertex *> path;

    std::cout << " " << std::endl;
    std::cout << green << "Algoritmo: " << def << "DFS" << std::endl;
    std::cout << " " << std::endl;

    for (vertex *v : vertices) {
        visited[v] = false;
    }

    dfs_recursive(start, visited, path);

    std::cout << "Recorrido en profundidad: [ ";

    for (const auto path : path) {
        std::cout << path.first->name << " ";
    }

    std::cout << "]" << std::endl;
}

void graph::dfs_recursive(vertex *s, std::unordered_map<vertex *, bool> &visited, std::unordered_map<vertex *, vertex *> &parents) {

    visited[s] = true;

    for (const auto edge : s->adj) {
        if (!visited[edge.first]) {
            parents[edge.first] = s;
            dfs_recursive(edge.first, visited, parents);
        }
    }
}

void graph::dijkstra(vertex *start) {

    std::unordered_map<vertex *, int> distances;
    std::unordered_map<vertex *, std::vector<vertex *>> paths;

    for (vertex *v : vertices) {
        distances[v] = oo;
    }

    distances[start] = 0;
    paths[start] = {start};

    std::priority_queue<std::pair<vertex *, int>> pq;
    pq.push({start, 0});

    while (!pq.empty()) {

        vertex *u = pq.top().first;
        pq.pop();

        for (const auto edge : u->adj) {

            vertex *v = edge.first;
            int weight = edge.second;
            int distance = distances[u] + weight;

            if (distance < distances[v]) {
                distances[v] = distance;
                paths[v] = paths[u];
                paths[v].push_back(v);
                pq.push({v, -distance});
            }
        }
    }

    /* Mostrar recorrido y caminos mínimos */

    std::cout << " " << std::endl;
    std::cout << green << "Algoritmo: " << def << "Dijkstra" << std::endl;
    std::cout << " " << std::endl;
    std::cout << "Distancias mínimas desde el vertice " << start->name << " a:";

    for (vertex *v : vertices) {
        std::cout << "" << std::endl;

        if (distances[v] != oo && v != start) {
            std::cout << "Vertice " << v->name << " - Camino: [ ";

            for (vertex *p : paths[v]) {
                std::cout << p->name << " ";
            }

            std::cout << "] - Costo: " << (distances[v] == oo ? "∞" : std::to_string(distances[v])) << std::endl;
        }
    }
}

void graph::topological_sort() {

    std::unordered_map<vertex *, bool> visited;
    std::stack<vertex *> topological_stack;

    for (vertex *v : vertices) {
        visited[v] = false;
    }

    for (vertex *v : vertices) {
        if (!visited[v]) {
            topological_sort_recursive(v, visited, topological_stack);
        }
    }

    std::cout << " " << std::endl;
    std::cout << green << "Algoritmo: " << def << "Ordenamiento topológico" << std::endl;
    std::cout << " " << std::endl;
    std::cout << "Ordenamiento: ";

    while (!topological_stack.empty()) {
        std::cout << topological_stack.top()->name << " ";
        topological_stack.pop();
    }

    std::cout << "" << std::endl;
}

void graph::topological_sort_recursive(vertex *v, std::unordered_map<vertex *, bool> &visited, std::stack<vertex *> &topological_stack) {

    visited[v] = true;

    for (const auto edge : v->adj) {
        if (!visited[edge.first]) {
            topological_sort_recursive(edge.first, visited, topological_stack);
        }
    }

    topological_stack.push(v);
}
