#ifndef COINPATH_GRAPH_H
#define COINPATH_GRAPH_H

#include <string>
#include <vector>

// Graph, Node and Edge - a weighted directed graph stored as an adjacency
// list (DP1). The maze is turned into one of these so that graph algorithms
// can run on it: a reachability check when a maze is built, and the
// solution-path search after a timeout (FR6, Deliverable 3).

// A vertex. The mark is scratch space for traversals (e.g. visited flags).
class Node {
public:
    explicit Node(int number);

    int getNumber() const;
    int getMark() const;
    void setMark(int mark);

private:
    int number;
    int mark;
};

// A directed edge from one node number to another. The weight is the coin
// cost of following it, and the type says what the edge represents
// (for example "floor" or "door").
class Edge {
public:
    Edge(int startPoint, int endPoint, double weight, std::string type);

    int getStartPoint() const;
    int getEndPoint() const;
    double getWeight() const;
    const std::string& getType() const;

private:
    int startPoint;
    int endPoint;
    double weight;
    std::string type;
};

class Graph {
public:
    // Creates nodes numbered 0 to numNodes - 1.
    // Throws std::invalid_argument if numNodes is negative.
    explicit Graph(int numNodes);

    int getNumNodes() const;

    // Adds a directed edge u -> v. Returns false for a self-loop or a node
    // number outside the graph.
    bool insertEdge(int u, int v, double weight, const std::string& type);

    // The node with this number, or nullptr if there is none.
    Node* getNode(int nodeNumber);
    const Node* getNode(int nodeNumber) const;

    // Every edge leaving the node; empty for an unknown node.
    std::vector<Edge> getEdgesConnectedTo(int nodeNumber) const;

    // The edge u -> v, or nullptr if it does not exist.
    const Edge* getEdgeBetween(int u, int v) const;

    // Depth-first search from start, using node marks as visited flags.
    // Returns false if either node does not exist.
    bool isReachable(int start, int goal);

private:
    bool isValidNode(int nodeNumber) const;
    void clearMarks();

    std::vector<Node> nodes;               // nodes[i] has number i
    std::vector<std::vector<Edge>> adjList; // adjList[i] = edges leaving node i
};

#endif // COINPATH_GRAPH_H
