#include<iostream>
#include<vector>

using namespace std;

//Program to check whether a simple undirected graph is two-edge connected by performing Depth First Search(DFS)

//Defines the maximum number of vertices which the graph can hold
#define MAX_SIZE 999

//Assigns the vertex a label which is equal to the number of vertices visited before it during the DFS traversal
//This value refers to the position of the given vertex in the list of vertices discovered in order(successively) by the DFS
int visited_vertex_count(-1);

//visited_vertex vector contains one entry corresponding to each vertex of the graph. All are initially set to false,
//but the value corresponding to any vertex is set to true(by the depth_first_search function) when that vertex is visited
vector<bool> visited_vertex;

//Vector to store the backedge endpoint vertex present at the highest(numerically lowest) level for all the vertices in the graph
//It will store -1 for any vertex which does not have a backedge
vector<int> vertex_arrival_label;

int two_edge_connectivity(int, int, vector< vector<int> >&);

//Driver function
int main(void)
{
    int number_of_vertices(0), number_of_edges(0), starting_vertex(-1), ending_vertex(-1), source_vertex(-1), index(0);
    cout << "Enter the number of vertices to be inserted in the graph:\n";
    cin >> number_of_vertices;
    if(number_of_vertices < 1)
    {
        cout << "The undirected graph must have at least one vertex\n";
    }
    else
    {
        cout << "The vertices inserted in the graph are:\n";
        for(index = 0; index < number_of_vertices; index++)
        {
            cout << index << "\n";
        }

        //Adjacency list represented by a vector of vectors to store the vertices adjacent to each vertex of the undirected graph
        vector< vector<int> > adjacency_list(number_of_vertices);

        cout << "Enter the number of edges to be inserted in the graph:\n";
        cin >> number_of_edges;
        //If the number of edges is more than n * (n-1) / 2 i.e. the maximum number of edges which an undirected graph on n vertices could have
        if(number_of_edges > number_of_vertices * (number_of_vertices - 1) / 2)
        {
            cout << "An undirected graph on " << number_of_vertices << " vertices has at most " << number_of_vertices * (number_of_vertices - 1) / 2 << " edge(s)\n";
        }
        else
        {
            //If the number of edges to be inserted in the graph is more than 0, then get the starting and ending vertices
            //of all the edges that would be inserted in the graph. This portion of the code is bypassed when the graph has
            //only one vertex because in that case the graph will not have any edge
            if(number_of_edges > 0)
            {
                cout << "Enter the starting and ending vertex of each edge separated by space:\n";
                for(index = 0; index < number_of_edges; index++)
                {
                    cin >> starting_vertex >> ending_vertex;
                    //Insert each vertex in the adjacency list of the other vertex
                    adjacency_list[starting_vertex].push_back(ending_vertex);
                    adjacency_list[ending_vertex].push_back(starting_vertex);
                }
            }
            //Initializing every element of visited_vertex to false(0)
            visited_vertex.resize(number_of_vertices, 0);
            //Initializing highest(numerically lowest) level backedge ancestor of every vertex to -1
            vertex_arrival_label.resize(number_of_vertices, -1);
            int result = two_edge_connectivity(0, -1, adjacency_list);
            if(result == MAX_SIZE + 1)
            {
                cout << "The given undirected graph is not two-edge connected\n";
            }
            else
            {
                cout << "The given undirected graph is two-edge connected\n";
            }
        }
    }
    return 0;
}

//Recursively check if the given graph is two-edge connected by checking whether any edge in the graph is a bridge edge
//Returns the backedge endpoint vertex present at the highest(numerically lowest) level in the DFS tree if the graph is two-edge connected, MAX_SIZE + 1 otherwise
int two_edge_connectivity(int source_vertex, int parent_vertex, vector< vector<int> > &adjacency_list)
{
    //Set the entry corresponding to the given source_vertex in the visited_vertex vector to true
    visited_vertex[source_vertex] = true;

    //Assign the newly visited vertex an appropriate vertex arrival label by incrementing the visited_vertex_count
    //visited_vertex_count value tells us the number of vertices visited before visiting the current vertex
    vertex_arrival_label[source_vertex] = ++visited_vertex_count;

    //Initialize the variable to store the backedge endpoint vertex present at the highest level in the DFS tree,
    //for a backedge originating at any vertex present in the DFS subtree rooted at the current source_vertex
    int highest_level_backedge_endpoint_vertex(MAX_SIZE + 1);

    //Loop over all the vertices adjacent to the given source_vertex
    for(int index = 0; index < adjacency_list[source_vertex].size(); index++)
    {
        //adjacent_vertex variable stores one of the vertices adjacent to the given source_vertex
        int adjacent_vertex = adjacency_list[source_vertex][index];

        //Variable to store the backedge vertex at the highest level i.e. having lowest vertex label for a backedge
        //originating from any vertex the DFS subtree rooted at the current adjacent_vertex(including the backedges
        //originating from the current adjacent_vertex). We compare this value corresponding to all the adjacent_vertices other
        //than the parent_vertex and the minimum of these values is assigned to the highest_level_backedge_endpoint_vertex
        int minimum_backedge_vertex_label(vertex_arrival_label[adjacent_vertex]);

        //If the adjacent_vertex is not visited, perform DFS recursively with the adjacent_vertex as the new source_vertex
        //and the current source_vertex as the new parent_vertex
        if(!visited_vertex[adjacent_vertex])
        {
            minimum_backedge_vertex_label = two_edge_connectivity(adjacent_vertex, source_vertex, adjacency_list);
            //If the value returned is MAX_SIZE + 1, then the subgraph rooted at the current adjacent_vertex and by extension
            //the entire undirected graph contains a bridge edge and hence it's not two-edge connected
            if(minimum_backedge_vertex_label == MAX_SIZE + 1)
            {
                return minimum_backedge_vertex_label;
            }
        }
        //Update the value of highest_level_backedge_endpoint_vertex if its greater than the minimum_backedge_vertex_label of the
        //current adjacent vertex being considered(which means that the highest level backedge ancestor of the adjacent_vertex is at a
        //numerically lower level than the highest_level_backedge_endpoint_vertex discovered till now) and the adjacent_vertex is not a
        //parent of the current source_vertex whose adjacent vertices are being processed as we are checking whether the edge from the
        //current parent_vertex to the current source_vertex is a bridge edge. This is because if there are no backedges, the parent_vertex
        //will always have a lower vertex label than that of the current adjacent_vertex
        if((minimum_backedge_vertex_label < highest_level_backedge_endpoint_vertex) && (adjacent_vertex != parent_vertex))
        {
            highest_level_backedge_endpoint_vertex = minimum_backedge_vertex_label;
        }
    }
    //If the level of the backedge vertex present at the highest level in the DFS tree is greater than(means the backedge vertex
    //is present at a lower level) or equal to(means the source_vertex is the backedge endpoint vertex at the numerically
    //lowest level or all the backedges in the subtree rooted at the source_vertex end at vertices present at a numerically higher
    //level/label than that of the parent vertex, leaving the parent_vertex as the vertex present at the numerically lowest level in the
    //subtree by virtue of the tree edge from the parent_vertex to the source_vertex)that of the given source_vertex, then the edge
    //from the given source_vertex to the given parent_vertex is a bridge edge and hence the graph is not two-edge connected
    //source_vertex should not be zero because it's the minimum possible label(which is assigned to the root of the DFS tree) and any
    //label of the backedge vertex at the highest level will always be greater than or equal to zero when the source_vertex has label zero
    //(so the first condition will always be satisfied) and if we don't add the second condition, the function will declare every graph
    //to be two-edge connected
    if((highest_level_backedge_endpoint_vertex >= vertex_arrival_label[source_vertex]) && (source_vertex != 0))
    {
        return MAX_SIZE + 1;
    }
    //Return the label of the backedge vertex present at the highest level i.e. having numerically lowest label
    return highest_level_backedge_endpoint_vertex;
}
