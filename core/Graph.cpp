#include "Graph.h"

#include <stdexcept>
#include <utility>

namespace {
constexpr int kUnvisited = 0;
constexpr int kVisited = 1;
} // namespace

Node::Node(int number) : number(number), mark(kUnvisited) {}

int Node::getNumber() const {
    return number;
}

int Node::getMark() const {
    return mark;
}

void Node::setMark(int newMark) {
    mark = newMark;
}

Edge::Edge(int startPoint, int endPoint, double weight, std::string type)
    : startPoint(startPoint), endPoint(endPoint), weight(weight), type(std::move(type)) {}

int Edge::getStartPoint() const {
    return startPoint;
}

int Edge::getEndPoint() const {
    return endPoint;
}

double Edge::getWeight() const {
    return weight;
}

const std::string& Edge::getType() const {
    return type;
}

Graph::Graph(int numNodes) {
    if (numNodes < 0) {
        throw std::invalid_argument("A graph cannot have a negative number of nodes");
    }
    nodes.reserve(numNodes);
    for (int i = 0; i < numNodes; ++i) {
        nodes.emplace_back(i);
    }
    adjList.resize(numNodes);
}

int Graph::getNumNodes() const {
    return static_cast<int>(nodes.size());
}

bool Graph::insertEdge(int u, int v, double weight, const std::string& type) {
    if (u == v || !isValidNode(u) || !isValidNode(v)) {
        return false;
    }
    adjList[u].emplace_back(u, v, weight, type);
    return true;
}

Node* Graph::getNode(int nodeNumber) {
    return isValidNode(nodeNumber) ? &nodes[nodeNumber] : nullptr;
}

const Node* Graph::getNode(int nodeNumber) const {
    return isValidNode(nodeNumber) ? &nodes[nodeNumber] : nullptr;
}

std::vector<Edge> Graph::getEdgesConnectedTo(int nodeNumber) const {
    if (!isValidNode(nodeNumber)) {
        return {};
    }
    return adjList[nodeNumber];
}

const Edge* Graph::getEdgeBetween(int u, int v) const {
    if (!isValidNode(u)) {
        return nullptr;
    }
    for (const Edge& edge : adjList[u]) {
        if (edge.getEndPoint() == v) {
            return &edge;
        }
    }
    return nullptr;
}

bool Graph::isReachable(int start, int goal) {
    if (!isValidNode(start) || !isValidNode(goal)) {
        return false;
    }
    clearMarks();

    // Iterative DFS so large mazes cannot overflow the call stack.
    std::vector<int> toVisit{start};
    nodes[start].setMark(kVisited);
    while (!toVisit.empty()) {
        const int current = toVisit.back();
        toVisit.pop_back();
        if (current == goal) {
            return true;
        }
        for (const Edge& edge : adjList[current]) {
            Node& next = nodes[edge.getEndPoint()];
            if (next.getMark() == kUnvisited) {
                next.setMark(kVisited);
                toVisit.push_back(next.getNumber());
            }
        }
    }
    return false;
}

bool Graph::isValidNode(int nodeNumber) const {
    return nodeNumber >= 0 && nodeNumber < getNumNodes();
}

void Graph::clearMarks() {
    for (Node& node : nodes) {
        node.setMark(kUnvisited);
    }
}
