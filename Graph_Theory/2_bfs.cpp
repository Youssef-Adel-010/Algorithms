#include <bits/stdc++.h>

using namespace std;

// BFS is a SPA iff: costs are equal.
vector<vector<int>> graph;
int n, e;
vector<int> bfs(int node) {
  vector<int> len(n, -1);
  queue<int> q;

  q.push(node);
  len[node] = 0;

  while (!q.empty()) {
    int lvlsize = q.size();
    while(lvlsize--) {
      int current = q.front();
      q.pop();
      for (auto &nei : graph[current]) {
        if(len[nei] == -1) {
          q.push(nei);
          len[nei] = len[current] + 1;
        }
      }
    }
    // After the lvlsize loop → we are sure that we processed a complete level.
  }
  return len;
}

vector<int> bfsPath(int s, int d) {
  vector<int> len(n, -1);
  vector<int> parent(n, -1);
  queue<int> q;

  len[s] = 0;
  q.push(s);

  bool ok = false;
  while(!ok && !q.empty()) {
    int lvl = q.size();
    while(lvl--) {
      int current = q.front();
      q.pop();
      for (auto &nei: graph[current]) {
        if(len[nei] == -1) {
          q.push(nei);
          parent[nei] = current;
          len[nei] = len[current] + 1;
          if (nei == d) {
            ok = true;
            break;
          }
        }
      }
    }
  }
  vector<int> path;
  while(d != -1) {
    path.push_back(d);
    d = parent[d];
  }
  reverse(path.begin(), path.end());

  return path;
}

bool isBi() {
  vector<int> color(n, -1);
  queue<int> q;

  for (int i = 0; i < n; ++i) {
    if(color[i] == -1) {
      color[i] = 0;
      q.push(i);

      while(!q.empty()) {
        int current = q.front();
        q.pop();
        for(auto &nei: graph[current]) {
          if(color[nei] == -1) {
            color[nei] = 1 - color[current];
            q.push(nei);
          }
          else if (color[nei] == color[current])
            return false;
        }
      }
    }
  }
  return true;
}
vector<int> m2mBfs(vector<int> sources) {
  vector<int> dist(n, -1);
  queue<int> q;

  for(auto &s: sources) {
    dist[s] = 0;
    q.push(s);
  }

  while(!q.empty()) {
    int current = q.front();
    q.pop();
    for(auto &nei: graph[current]) {
      if(dist[nei] == -1) {
        dist[nei] = dist[current] + 1;
        q.push(nei);
      }
    }
  }
  return dist;
}
int main() {
  cin >> n >> e;
  graph = vector<vector<int>>(n);
  while(e--) {
    int f, t;
    cin >> f >> t;
    graph[f].push_back(t);
    graph[t].push_back(f); // undirected graph
  }
vector<int> sources = {1, 2, 3};
vector<int> dist = m2mBfs({2});
// for(int i = 1; i <= 12; ++i) {
//     cout << "Distance from 1 to " << i << " is " << dist[i] << endl;
// }

  // vector<int> path = bfsPath(1, 10);
  //  for (int i = 0; i < n; ++i) {
  //    cout << i << " -> " << len[i] << endl;
  //  }
  // for(auto &p: path)
  //   cout << p << endl;
}
