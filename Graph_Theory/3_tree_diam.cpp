#include<bits/stdc++.h>
using namespace std;

int n, e;
vector<vector<int>> graph;

/*
  * Tree Diameter → Is the longest path in the tree from any node to any node.
  * Graph Diameter → Is the longest shortest path from any node to any node.
  * We can get Tree Diameter using BFS and DFS both in O(N).
*/

/*
  * DFS:
  * Main idea is → The diameter might pass throw a node and might not.
  * If pass from the node take the sum of the highest 2 heights of their children.
  * If not pass then the diameter totally in one of its children e.g. in a subtree.
  * Make a DFS and every time we save the best Child Diameter and save the highest height.
  * Then we maximize on them and get the diameter.
*/

// We send parent as -1 initially
// (childDiameter, childHeight)
vector<int> diameterPath;
pair<int, int> diameter(int node, int parent) {
  int maxDiameter = 0;
  int maxHeights[] = {0, 0, 0};

  for (auto &nei : graph[node]) {
    if (nei == parent) continue;

    auto [childDiameter, childHeight] = diameter(nei, node);
    maxDiameter = max(maxDiameter, childDiameter);

    maxHeights[0] = childHeight + 1;
    sort(maxHeights, maxHeights + 3);
  }

  maxDiameter = max(maxDiameter, maxHeights[1] + maxHeights[2]);
  return {maxDiameter, maxHeights[2]};
}
/*
  * BFS:
  * Select a random node A
  * Run BFS on this node to find the furthermost node from A. name this node as S.
  * Now run BFS starting from S, find the furthermost node from S, name it D.
  * Path between S and D is diameter of the tree.
*/

vector<int> len;
int bfs(int node) {
  queue<int> q;
  len.assign(n, -1);

  len[node] = 0;
  q.push(node);
  int current;

  while(!q.empty()) {
    current = q.front();
    q.pop();
    for(auto &nei: graph[current]) {
      if (len[nei] == -1) {
        q.push(nei);
        len[nei] = len[current] + 1;
      }
    }
  }
  return current;
}

/*
  * Tree Center → The node/s in the tree that minimize the length of the longest path to any other node.
  * Tree Center is the middle of Tree Diameter.
  * Tree Center is 1 or 2 ONLY.
  * Even length diameter → 1 Centers.
  * Odd length diameter → 2 Centers.
  * We can get the Tree Center using two methods:
*/

/*
  * Using Diameter:
  * Get the diameter path.
  * Get the mid nodes.
*/

/*
  * Leaf Removal using BFS:
  * Save the degrees of all nodes.
  * Nodes with degree 1 are the leaves.
  * Start using BFS to shrink level by level.
  * Stop when the remaining <= 2.
  * The queue has the centers.
*/
vector<int> leafRemoval(int node) {
  vector<int> degree(n);
  queue<int> q;

  for (int i = 0; i < n; ++i) {
    degree[i] = graph[i].size();
    if(degree[i] <= 1) {
      q.push(i);
    }
  }

  int remaining = n;

  while(remaining > 2) {
    int sz = q.size();
    remaining -= sz;

    while(sz--) {
      int leaf = q.front();
      q.pop();
      for(auto &nei: graph[leaf]) {
        if(--degree[nei] <= 1) {
          q.push(nei);
        }
      }
    }
  }

  vector<int> centers;
  while(!q.empty()) {
    centers.push_back(q.front());
    q.pop();
  }
  return centers;
}

/*
  * Worst Nodes → Is the nodes that result in the maximum possible height of the tree.
  * Basically it is the two ends of the diameter.
*/


/*
  * Tree Isomorphism → Means two trees has same structure, no matter what is values.
  * We can check if two trees isomorphic with DFS and canonical form.
  * It will only work if you have a given roots.
  * If you don't have roots?? then find roots using Tree Centers and here is two options:
  * To Use BFS Leaf Removal to get centers, then use DFS Canonical.
  * Or to use BFS Leaf Removal Directly.
*/
string isoDFSCanonical(int node, int parent) {
  vector<string> children;

  for(auto &nei: graph[node]) {
    if (nei == parent)
      continue;
    children.push_back(isoDFSCanonical(nei, node));
  }

  sort(children.begin(), children.end());

  string res = "(";
  for(auto &c: children)
    res += c;
  res += ")";

  return res;
}

string isoBFSCanonical() {
  vector<int> degrees(n);
  vector<vector<string>> canonicals(n);
  queue<int> q;

  for (int i = 0; i < n; ++i) {
    degrees[i] = graph[i].size();
    if(degrees[i] <= 1)
      q.push(i);
  }

  int remaining = n;
  while(remaining > 2) {
    int sz = q.size();
    remaining -= sz;
    while(sz--) {
      int leaf = q.front();
      q.pop();

      string nodeRep = getNodeCanonicalForm(leaf, canonicals);
      for(auto &nei: graph[leaf]) {
        canonicals[nei].push_back(nodeRep);
        if (--degrees[nei] <= 1)
          q.push(nei);
      }
    }
  }
  vector<int> centers;
  while(!q.empty()) {
    centers.push_back(q.front());
    q.pop();
  }

  string can1 = getNodeCanonicalForm(centers[0], canonicals);

  if (centers.size() == 1)
    return can1;

  string can2 = getNodeCanonicalForm(centers[1], canonicals);
  canonicals[centers[0]].push_back(can2);
  canonicals[centers[1]].push_back(can1);

  return min(getNodeCanonicalForm(centers[0], canonicals), getNodeCanonicalForm(centers[1], canonicals));
}
string getNodeCanonicalForm(int leaf, vector<vector<string>> &canonicals) {
  sort(canonicals[leaf].begin(), canonicals[leaf].end());
  string nodeRep = "(";
  for (auto &s: canonicals[leaf])
    nodeRep += s;
  nodeRep += ")";

  return nodeRep;
}


int main() {
  cin >> n >> e;
  graph = vector<vector<int>>(n);
  len = vector<int>(n, -1);

  while(e--) {
    int f, t;
    cin >> f >> t;
    graph[f].push_back(t);
    graph[t].push_back(f);
  }
  int a = bfs(1);
  int b = bfs(a);
  cout << len[b];
}
