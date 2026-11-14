#include <gtest/gtest.h>

#include "Graph.h"

TEST(GraphTest, InsertsAndFindsEdges) {
    Graph graph(4);
    EXPECT_TRUE(graph.insertEdge(0, 1, 5.5, "floor"));
    EXPECT_TRUE(graph.insertEdge(0, 2, 3.2, "door"));

    EXPECT_EQ(graph.getEdgesConnectedTo(0).size(), 2u);
    const Edge* edge = graph.getEdgeBetween(0, 2);
    ASSERT_NE(edge, nullptr);
    EXPECT_DOUBLE_EQ(edge->getWeight(), 3.2);
    EXPECT_EQ(edge->getType(), "door");
    EXPECT_EQ(graph.getEdgeBetween(2, 0), nullptr); // edges are directed
}

TEST(GraphTest, RejectsSelfLoopsAndUnknownNodes) {
    Graph graph(3);
    EXPECT_FALSE(graph.insertEdge(1, 1, 1.0, "floor"));
    EXPECT_FALSE(graph.insertEdge(0, 3, 1.0, "floor"));
    EXPECT_FALSE(graph.insertEdge(-1, 0, 1.0, "floor"));
    EXPECT_EQ(graph.getNode(3), nullptr);
    EXPECT_TRUE(graph.getEdgesConnectedTo(7).empty());
}

TEST(GraphTest, ReachabilityFollowsEdgeDirection) {
    Graph graph(4);
    graph.insertEdge(0, 1, 0, "floor");
    graph.insertEdge(1, 2, 0, "floor");
    EXPECT_TRUE(graph.isReachable(0, 2));
    EXPECT_FALSE(graph.isReachable(2, 0));
    EXPECT_FALSE(graph.isReachable(0, 3));
    EXPECT_FALSE(graph.isReachable(0, 9));
}
