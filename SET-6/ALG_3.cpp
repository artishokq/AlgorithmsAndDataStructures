struct Edge {
  int u;
  int v;
  int weight;
};

class UnionFind {
 private:
  std::vector<int> parent;
  std::vector<int> rank;

 public:
  UnionFind(int size) {
    parent.resize(size);
    rank.resize(size, 0);
    for (int i = 0; i < size; ++i) {
      parent[i] = i;
    }
  }

  int find(int x) {
    if (parent[x] != x) {
      parent[x] = find(parent[x]);
    }
    return parent[x];
  }

  void unite(int x, int y) {
    int rootX = find(x);
    int rootY = find(y);
    if (rootX != rootY) {
      if (rank[rootX] > rank[rootY]) {
        parent[rootY] = rootX;
      } else if (rank[rootX] < rank[rootY]) {
        parent[rootX] = rootY;
      } else {
        parent[rootY] = rootX;
        ++rank[rootX];
      }
    }
  }
};

std::vector<Edge> ALG_3(int V, std::vector<Edge>& edges) {
  UnionFind uf(V);
  std::vector<Edge> T;
  std::srand(std::time(0));
  std::random_shuffle(edges.begin(), edges.end());
  for (Edge& e : edges) {
    uf.unite(e.u, e.v);
    T.push_back(e);
    // Проверка на цикл
    if (uf.find(e.u) == uf.find(e.v)) {
      // Найти максимальное ребро в цикле
      Edge e_max = *std::max_element(
          T.begin(), T.end(),
          [](const Edge& a, const Edge& b) { return a.weight < b.weight; });
      T.erase(std::remove(T.begin(), T.end(), e_max), T.end());
    }
  }
  return T;
}