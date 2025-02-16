struct Edge {
  int u;
  int v;
  int weight;
  bool operator<(const Edge& other) const { return weight > other.weight; }
};

bool checkConnection(const std::vector<std::vector<int>>& graph, int V) {
  // DFS для проверки связности

  std::vector<bool> visited(V, false);
  std::function<void(int)> dfs = [&](int node) {
    visited[node] = true;
    for (int neighbor : graph[node]) {
      if (!visited[neighbor]) {
        dfs(neighbor);
      }
    }
  };
  dfs(0);
  for (bool v : visited) {
    if (!v) return false;
  }
  return true;
}

std::vector<Edge> ALG_1(int V, std::vector<Edge>& edges) {
  std::sort(edges.begin(), edges.end());
  std::vector<std::vector<int>> graph(V);
  std::vector<Edge> T = edges;
  for (Edge e : edges) {
    // Удаление ребра e и проверка связности
    auto tempGraph = graph;
    // Удалить ребро из графа
    if (checkConnection(tempGraph, V)) {
      graph[e.u].push_back(e.v);
      graph[e.v].push_back(e.u);
    } else {
      // Удалить ребро из T
      T.erase(std::remove(T.begin(), T.end(), e), T.end());
    }
  }
  return T;
}