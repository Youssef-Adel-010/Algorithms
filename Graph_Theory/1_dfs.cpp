#include <bits/stdc++.h>

using namespace std;

vector<vector<int>> graph;
vector<bool> visited;
vector<int> topsort;

void dfs(int node) {
  visited[node] = true;
  for(auto &nei: graph[node]) {
    if (!visited[nei]) {
      dfs(nei);
    }
  }
  // in case of DAG (Directed Acyclic Graph)
  topsort.push_back(node);
}

vector<int> start;
vector<int> finish;
int timer = 0;
void dfs_edge_class(int node) {
  start[node] = timer++;
  for (auto &nei: graph[node]){
    if (!start[nei]) {
      dfs_edge_class(nei);
    }
    else {
      if (!finish[nei]) {
        // cycle edge
      }
      else if (start[node] < start[nei]) {
        // forward edge
      }
      else {
        // cross edge
      }
    }
  }
}

int main() {
  int n, e;
  cin >> n >> e;

  graph = vector<vector<int>>(n);
  visited = vector<bool>(n);

  while(e--) {
    int from, to;
    cin >> from >> to;
    graph[from].push_back(to);
    graph[to].push_back(from);
  }
  // cout << diameter(1, -1).first << endl;
}