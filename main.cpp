#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>

class Graph {
public:
    // Initialize the graph with a given number of vertices.
    explicit Graph(int num_vertices) : adj_list_(num_vertices) {}

    // Add an edge to the graph.
    void addEdge(int u, int v) {
        adj_list_[u].push_back(v);
        adj_list_[v].push_back(u);
    }

    // Check if the graph is a block graph.
    bool isBlock() const {
        // For each vertex, check if it is the root of a biconnected component.
        // If it is, check if the component is complete.
        for (int i = 0; i < adj_list_.size(); ++i) {
            if (visited_.count(i) == 0) {
                std::unordered_map<int, int> low_link;
                if (!dfs(i, -1, low_link)) {
                    return false;
                }
            }
        }
        return true;
    }

private:
    // Depth-first search helper function to find biconnected components.
    bool dfs(int u, int parent, std::unordered_map<int, int>& low_link) const {
        visited_.insert(u);
        low_link[u] = u;
        bool is_root = true;
        int num_children = 0;
        for (int v : adj_list_[u]) {
            if (v == parent) {
                continue;
            }
            if (visited_.count(v) == 0) {
                if (!dfs(v, u, low_link)) {
                    return false;
                }
                low_link[u] = std::min(low_link[u], low_link[v]);
                if (low_link[v] >= u) {
                    ++num_children;
                    if (parent != -1 || num_children > 1) {
                        // u is not the root of a biconnected component.
                        is_root = false;
                    }
                    if (num_children > 2) {
                        // The biconnected component is not complete.
                        return false;
                    }
                }
            } else {
                low_link[u] = std::min(low_link[u], v);
            }
        }
        return is_root;
    }
    std::vector<std::vector<int>> adj_list_;
    mutable std::unordered_set<int> visited_;
};

int main() {
    Graph g(6);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(3, 1);
    g.addEdge(3, 4);
    g.addEdge(3, 5);

    std::cout << "Is block: " << std::boolalpha << g.isBlock() << std::endl;

    return 0;
}