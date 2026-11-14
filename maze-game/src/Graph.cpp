#include <iostream>
#include <vector>
#include <string>

using namespace std;

// ==========================================
// 1. Node Class
// ==========================================
class Node {
private:
    int number;
    int mark;

public:
    // Constructor that sets a number for the node
    Node(int num) {
        number = num;
        mark = 0; // Default mark value
    }

    // Getter for number
    int getNumber() const {
        return number;
    }

    // Setter for number
    void setNumber(int num) {
        number = num;
    }

    // Getter for mark
    int getMark() const {
        return mark;
    }

    // Setter for mark
    void setMark(int m) {
        mark = m;
    }
};

// ==========================================
// 2. Edge Class
// ==========================================
class Edge {

// Declaring the private variables
private:
    double weight;
    Node startPoint;
    Node endPoint;
    string type;

public:
    // Constructor that sets weight, starting point, endpoint, and type
    Edge(double w, Node start, Node end, string t) {
        weight = w;
        startPoint = start;
        endPoint = end;
        type = t;
    }

    // Getters
    double getWeight() const { return weight; }
    Node getStartPoint() const { return startPoint; }
    Node getEndPoint() const { return endPoint; }
    string getType() const { return type; }

    // Setters
    void setWeight(double w) { weight = w; }
    void setStartPoint(int start) { startPoint = start; }
    void setEndPoint(int end) { endPoint = end; }
    void setType(string t) { type = t; }
};

// ==========================================
// 3. Graph Class
// ==========================================
class Graph {
private:
    int numNodes;
    vector<vector<Edge>> adjList;
    vector<Node> nodes; // Stores node objects for easy retrieval by number

public:
    // Constructor takes the number of nodes as its parameter
    Graph(int n) {
        numNodes = n;
        adjList.resize(n);
        
        // Initialize nodes with distinct numbers (0 to n-1)
        for (int i = 0; i < n; i++) {
            nodes.push_back(Node(i));
        }
    }

    // Method to get the adjacency list
    const vector<vector<Edge>>& getAdjList() const {
        return adjList;
    }

    // Method for inserting an edge between nodes which have distinct numbers
    bool insertEdge(int u, int v, double weight, string type) {
        if (u == v) {
            cout << "Error: Nodes must have distinct numbers.\n";
            return false;
        }
        if (u < 0 || u >= numNodes || v < 0 || v >= numNodes) {
            cout << "Error: Node indices out of bounds.\n";
            return false;
        }

        // Add directed edge (or undirected by adding both ways if needed)
        adjList[u].push_back(Edge(weight, u, v, type));
        return true;
    }

    // Method for returning a node with a certain number
    Node* getNode(int nodeNumber) {
        for (auto& node : nodes) {
            if (node.getNumber() == nodeNumber) {
                return &node;
            }
        }
        return nullptr; // Return nullptr if not found
    }

    // Method for returning all the edges that connect to a given node
    vector<Edge> getEdgesConnectedTo(int nodeNumber) const {
        if (nodeNumber >= 0 && nodeNumber < numNodes) {
            return adjList[nodeNumber];
        }
        return {};
    }

    // Method for returning an edge between two nodes if it exists
    Edge* getEdgeBetween(int u, int v) {
        if (u >= 0 && u < numNodes) {
            for (auto& edge : adjList[u]) {
                if (edge.getEndPoint() == v) {
                    return &edge;
                }
            }
        }
        return nullptr; // Return nullptr if no edge exists
    }
};

// ==========================================
// 4. Main Method
// ==========================================
int main() {
    // Create a new Graph object with 4 nodes
    Graph myGraph(4);

    // Insert edges between nodes with distinct numbers
    myGraph.insertEdge(0, 1, 5.5, "Directed");
    myGraph.insertEdge(0, 2, 3.2, "Directed");
    myGraph.insertEdge(1, 3, 8.1, "Directed");

    // Test getting a node
    Node* n = myGraph.getNode(1);
    if (n != nullptr) {
        cout << "Found Node with number: " << n->getNumber() << "\n";
        n->setMark(1); // Set mark example
        cout << "Node mark set to: " << n->getMark() << "\n\n";
    }

    // Test returning all edges connected to a given node (e.g., Node 0)
    cout << "Edges connected to Node 0:\n";
    vector<Edge> edgesFrom0 = myGraph.getEdgesConnectedTo(0);
    for (const auto& edge : edgesFrom0) {
        cout << " - Edge from " << edge.getStartPoint() 
             << " to " << edge.getEndPoint() 
             << " with weight " << edge.getWeight() 
             << " (Type: " << edge.getType() << ")\n";
    }

    // Test returning an edge between two specific nodes
    Edge* foundEdge = myGraph.getEdgeBetween(0, 2);
    if (foundEdge != nullptr) {
        cout << "\nFound edge between 0 and 2 with weight: " << foundEdge->getWeight() << "\n";
    } else {
        cout << "\nNo edge found between 0 and 2.\n";
    }

    return 0;
}