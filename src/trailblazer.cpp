// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: write comment header for this file; remove this comment

#include "costs.h"
#include "trailblazer.h"
#include "queue.h"
#include "pqueue.h"


static bool canReachEnd(Vertex *start, Vertex *end) {
    if (start->visited) return false;
    start->visited = true;
    start->setColor(GREEN);
    if (start == end) return true;
    for (Edge *edge : start->edges) {
        if (canReachEnd(edge->finish, end)) {
            edge->finish->previous = start;
            return true;
        }
    }
    return false;
}

Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    start->previous = NULL;
    if (canReachEnd(start, end)) {
        Vertex *current = end;
        while (current != NULL) {
            path.insert(0, current);
            current = current->previous;
        }
    }
    return path;
}

Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex *> path;
    Queue<Vertex *> queue;
    start->setColor(YELLOW);
    queue.enqueue(start);
    while (!queue.isEmpty()) {
        Vertex *current = queue.dequeue();
        current->setColor(GREEN);
        current->visited = true;
        if (current == end) {
            while (current != NULL) {
                path.insert(0, current);
                current = current->previous;
            }
            return path;
        }
        for (Edge *edge : current->edges) {
            if (!edge->finish->visited) {
                edge->finish->setColor(YELLOW);
                edge->finish->previous = current;
                queue.enqueue(edge->finish);
            }
        }
        current->setColor(GRAY);
    }
    return path;
}


Vector<Vertex *> dijkstraTemplate(BasicGraph& graph, Vertex *start, Vertex *end, double 
                                 (*heuristicFnPtr)(Vertex *, Vertex *)) {
    graph.resetData();
    Vector<Vertex*> path;
    PriorityQueue<Vertex *> pqueue;
    start->setColor(YELLOW);
    start->cost = 0.0;
    pqueue.enqueue(start, start->cost);
    while (!pqueue.isEmpty()) {
        Vertex *current = pqueue.dequeue();
        current->setColor(GREEN);
        current->visited = true;
        if (current == end) {
            while (current != NULL) {
                path.insert(0, current);
                current = current->previous;
            }
            break;
        }
        for (Edge *edge : current->edges) {
            if (!edge->finish->visited) {
                double candidateCost = current->cost + edge->cost;
                double distance;
                if (heuristicFnPtr == NULL) {
                    distance = 0;
                } else {
                    distance = (*heuristicFnPtr)(edge->finish, end);
                }

                if (edge->finish->getColor() != YELLOW) {
                    edge->finish->cost = candidateCost;
                    edge->finish->setColor(YELLOW);
                    edge->finish->previous = current;
                    pqueue.enqueue(edge->finish, candidateCost + distance);
                }
                else if (candidateCost < edge->finish->cost) {
                    edge->finish->cost = candidateCost;
                    edge->finish->previous = current;
                    pqueue.enqueue(edge->finish, candidateCost + distance);
                }
            }
        }
    }
    return path;
}


Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    return dijkstraTemplate(graph, start, end, NULL);
}

Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    return dijkstraTemplate(graph, start, end, &heuristicFunction);
}

Set<Edge*> kruskal(BasicGraph& graph) {
    Set<Edge*> mst;
    Map<Vertex *, Set<Vertex *> > clusters;
    PriorityQueue<Edge *> pqueue;
    Set<Vertex *> allVertices;
    Set<Edge *> allEdges;
    allVertices = graph.getVertexSet();
    allEdges = graph.getEdgeSet();
    for (Vertex *vertex : allVertices) {
        Set<Vertex *> cluster;
        cluster.add(vertex);
        clusters.put(vertex, cluster);
    }
    for (Edge *edge : allEdges) {
        pqueue.enqueue(edge, edge->cost);
    }
    while (!pqueue.isEmpty()) {
        Edge *edge = pqueue.dequeue();
        if (!clusters[edge->start].contains(edge->finish)) {
            Set<Vertex *> combinedCluster;
            combinedCluster = clusters[edge->start] + clusters[edge->finish];
            for (Vertex *vertex : clusters) {
                if (combinedCluster.contains(vertex)) clusters[vertex] = combinedCluster;
            }
            mst.add(edge);
        }
    }
    return mst;
}
