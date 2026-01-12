#include<bits/stdc++.h>
using namespace std;
/*
 * Shortest Path Algorithms:
 *
 * 1) Single Source Shortest Path (SSSP)
 *    - Dijkstra
 *    - Bellman-Ford
 *
 * 2) All Pairs Shortest Path (APSP)
 *    - Floyd-Warshall
 */

const int MAXN = 20;
const int INF  = 1e9;
int n;
int adj[MAXN][MAXN];

/*
 * WRONG approach: Cyclic Recurrence
 * This recursion has cycles and no clear base restriction.
*/
int shortestPathCyclic(int i, int j) {
  int best = adj[i][j];

  for (int k = 0; k < n; ++k) {
    if (k != i && k != j) {
      best = min(best, shortestPathCyclic(i, k) + shortestPathCyclic(k, j));
    }
  }
  return best;
}

/*
 * Correct Floyd Recurrence (Top-Down Definition)
*/
int floydRec(int i, int j, int k) {
  if (k == 0)
    return adj[i][j];

  return min(
    floydRec(i, j, k - 1),
    floydRec(i, k, k - 1) +
    floydRec(k, j, k - 1)
  );
}

/*
  * Memoized Floyd (Top-Down DP)
*/
int memo[MAXN + 1][MAXN][MAXN];
int floydMemo(int i, int j, int k) {
  if(k == 0) {
    return adj[i][j];
  }

  int &ret = memo[k][i][j];
  if(ret != -1)
    return ret;

  ret = min(
    floydMemo(i, j, k - 1),
    floydMemo(i, k, k - 1) +
    floydMemo(k, j, k - 1)
  );

  return ret;
}
// Floyd → 3D DP (Bottom-Up)
/*
  * Why dp dimension is [n + 1] ?
  *
  * dp[k][i][j] represents the shortest path from i to j
  * using only nodes {0 .. k-1} as intermediate nodes.
  *
  * Base case:
  *   dp[0][i][j] → no intermediate nodes allowed.
  *
  * Since k represents the COUNT of allowed intermediate nodes,
  * and we allow nodes 0, 1, ..., n-1,
  * we need n + 1 layers: k = 0 .. n.
  * k is not a node index.
  * k is how many intermediate nodes are allowed.
  *
  * dp[0]  -> no nodes allowed
  * dp[1]  -> node 0 allowed
  * dp[2]  -> nodes 0,1 allowed
  * ...
  * dp[n]  -> nodes 0..n-1 allowed
  *
  * Hence dp size = n + 1
  * After runnning floyd:
  * dp[n][i][j] is the sp from i to j.
  * max(dp[n][i][j]) is graph diameter.
*/
int dp[MAXN + 1][MAXN][MAXN];
void floydDP3D() {
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      dp[0][i][j] = adj[i][j];
    }
  }

  for (int k = 0; k < n; ++k) {
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        dp[k + 1][i][j] = min(
          dp[k][i][j],
          dp[k][i][k] +
          dp[k][k][j]);
      }
    }
  }
}

/*
 * Floyd-Warshall In-Place (2D)
 * This is the practical implementation
*/
void FloydWarshall() {
  for (int k = 0; k < n; ++k) {
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        if (adj[i][k] < INF && adj[k][j] < INF) {
          adj[i][j] = min(
            adj[i][j],
            adj[i][k] + adj[k][j]
          );
        }
      }
    }
  }
}
/*
  * Path Tracking (i, j):
  * Intermediate Node Matrix → Saving the mid node between i and j.
  * Previous Node Matrix → Saving the previous node befor j.
*/
int mid[MAXN][MAXN];
int prevv[MAXN][MAXN];
void floydPath() {
  for (int k = 0; k < n; ++k) {
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        int throughK = adj[i][k] + adj[k][j];
        if (throughK < adj[i][j]) {
          adj[i][j] = throughK;
          mid[i][j] = k;              // first method
          prevv[i][j] = prevv[i][k];  // second method
        }
      }
    }
  }
}
/*
  * Intermediate Node Matrix
*/
// memset(mid, -1, sizeof(mid)); // Initializing
void printPathMid(int i, int j) {
  if(mid[i][j] == -1) {
    cout << i << " ";
    if (i != j)
    cout << j << " ";
    return;
  }

  int k = mid[i][j];

  printPathMid(i, k);
  printPathMid(k, j);
}
/*
  * Previous Node Matrix
*/
void initialize() {
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (adj[i][j] < INF)
        prevv[i][j] = i;
      else
        prevv[i][j] = -1;
    }
  }
}
void printPathPrev(int i, int j) {
  if (prevv[i][j] == -1) {
    cout << "No path\n";
    return;
  }

  if (i != j)
    printPathPrev(i, prevv[i][j]);

  cout << j << " ";
}

/*
  * Floyd-Warshall Applications:
  * Transitive Closure → Is there a path or not.
  * Minimax → Minimize the maximum edge on the path.
  * Maximin → Maximize the minimum edge on the path.
  * Longest Path → Only in DAG.
  * Count Paths:
    * Only in DAG.
    * Can use Topological Sort + DP to count paths.
    * What if the graph is not DAG? if(adj[i][i] > 0) → i has a cycle path on it.
    * For every node v that has a cycle, if i reaches v and v reaches j
    * then the count of (i, j) is useless (INFINITY) and the remaining adj[][] is valid.
  * Negative Cycle Detection
  * NOTE:
    * For shortest path → negative cycle affects, positive cycle not.
    * For longest path → positive cycle affects, negative cycle not.
    * For count paths → any cycle affects.
    * affects means undefined.
  * Graph Diameter → Highest shortest path in the graph.
  * Strongly Connected Component (SCC) after using Floyd or Transitive Closure.
*/
void TransitiveClosure() {
  for (int k = 0; k < n; ++k) {
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        adj[i][j] |= adj[i][k] && adj[k][j];
        // adj[i][j] = adj[i][j] || (adj[i][k] && adj[k][j]);
      }
    }
  }
}

void MinMax() {
  for (int k = 0; k < n; ++k) {
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        adj[i][j] = min(adj[i][j], max(adj[i][k], adj[k][j]));
      }
    }
  }
}

void Maximin() {
  for (int k = 0; k < n; ++k) {
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        adj[i][j] = max(adj[i][j], min(adj[i][k], adj[k][j]));
      }
    }
  }

}

bool NegativeCycle() {
  // After Running Floyd
  for (int i = 0; i < n; ++i) {
    if(adj[i][i] < 0)
    return true;
  }
  return false;
}
bool isNegativeCycleEffectiveOn(int src, int dest) {
  for (int i = 0; i < n; ++i) {
    if (adj[i][i] < 0 && adj[src][i] < INF && adj[i][dest] < INF)
      return true;
    return false; // there is a finite path although cycles if any.
  }
}

int GraphDiameter() {
  FloydWarshall();
  int diam = 0;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (adj[i][j] < INF) {
        diam = max(diam, adj[i][j]);
      }
    }
  }
  return diam;
}

vector<vector<int>> SCC() {
  vector<int> comp(n, -1);
  int cnt = 0;

  for (int i = 0; i < n; ++i) {
    if(comp[i] == -1) {
      comp[i] = cnt;
      for (int j = 0; j < n; ++j) {
        if(adj[i][j] < INF && adj[j][i] < INF) {
          comp[j] = cnt;
        }
      }
      cnt++;
    }
  }

  vector<vector<int>> compGraph(cnt, vector<int>(cnt, 0));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (adj[i][j] < INF) {
        compGraph[comp[i]][comp[j]] = 1;
      }
    }
  }
  return compGraph;
}

int main() {

}
