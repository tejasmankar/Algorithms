#include<iostream>
#include<vector>

using namespace std;

//Program to check whether a directed graph contains a cycle using Depth First Search(DFS)

//Defines the maximum number of vertices which the graph can hold
#define MAX_SIZE 999

//Counter to keep track of the departure label/time to be assigned to individual vertices after completing DFS for the
//subtree rooted at those vertices
int vertex_departure_label_counter(0);

//visited_vertex vector contains one entry corresponding to each vertex of the graph. All are initially set to false,
//but the value corresponding to any vertex is set to true(by the depth_first_search function) when that vertex is visited
vector<bool> visited_vertex;

//vertex_departure_label vector stores the time at which the DFS procedure departed from a vertex after completing DFS for the
//subtree rooted at that vertex for all the vertices in the directed graph. We assign label 1 to the vertex from which we departed
//first and so on. We depart from a vertex after all its adjacent vertices are visited
vector<int> vertex_departure_label;

bool cycle_presence_check(int, vector< vector<int> >&);

//Driver function
int main()
{
    int number_of_vertices(0), number_of_edges(0), starting_vertex(-1), ending_vertex(-1), source_vertex(-1), index(0);
    cout << "Enter the number of vertices to be inserted in the graph:\n";
    cin >> number_of_vertices;
    if(number_of_vertices < 1)
    {
        cout << "The directed graph must have at least one vertex\n";
    }
    else
    {
        cout << "The vertices inserted in the graph are:\n";
        for(index = 0; index < number_of_vertices; index++)
        {
            cout << index << "\n";
        }

        //Adjacency list represented by a vector of vectors to store the vertices adjacent to each vertex of the directed graph
        vector< vector<int> > adjacency_list(number_of_vertices);

        cout << "Enter the number of edges to be inserted in the graph:\n";
        cin >> number_of_edges;
        //If the number of edges is more than n * (n-1) i.e. the maximum number of edges which a directed graph on n vertices could have
        if(number_of_edges > number_of_vertices * (number_of_vertices - 1))
        {
            cout << "A directed graph on " << number_of_vertices << " vertices has at most " << number_of_vertices * (number_of_vertices - 1) / 2 << " edge(s)\n";
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
                }
            }

            //Set the entry corresponding to each graph vertex in the visited_vertex vector to false and initialize the size of
            //the visited_vertex vector
            visited_vertex.resize(number_of_vertices, false);
            //Initialize the entry corresponding to each graph vertex in the vertex_departure_label vector to MAX_SIZE and initialize
            //the size of the vertex_departure_label vector
            vertex_departure_label.resize(number_of_vertices, MAX_SIZE);

            //Passing each vertex in the graph as the source vertex to check if the given vertex is a part of any cycle
            //We are passing every vertex to the cycle_presence_check function since there is a possibility of having a
            //vertex which is unreachable from some vertex in the graph and is also a part of a cycle. otherwise, we may
            //not be able to visit such a vertex
            for(source_vertex = 0; source_vertex < number_of_vertices; source_vertex++)
            {
                if(!visited_vertex[source_vertex])
                {
                    if(cycle_presence_check(source_vertex, adjacency_list))
                    {
                        cout << "The given directed graph contains a cycle\n";
                        return 0;
                    }
                }
            }
            cout << "The given directed graph does not contain a cycle\n";
        }
    }
    return 0;
}

//Returns true if the given directed graph contains a cycle or false otherwise
bool cycle_presence_check(int source_vertex, vector< vector<int> > &adjacency_list)
{
    //Set the entry corresponding to the given source_vertex in the visited_vertex vector to true
    visited_vertex[source_vertex] = true;

    //Loop over all the vertices adjacent to the given source_vertex
    for(int index = 0; index < adjacency_list[source_vertex].size(); index++)
    {
        //adjacent_vertex variable stores one of the vertices adjacent to the given source_vertex
        int adjacent_vertex = adjacency_list[source_vertex][index];

        //If the adjacent_vertex is not visited, perform DFS recursively with the adjacent_vertex as the new source_vertex
        //This is to determine if any edge of the subtree rooted at adjacent_vertex is a backedge because if any edge is a
        //backedge which connects a vertex having a larger vertex departure label/time, then we can conclude that the directed
        //graph contains a cycle
        if(!visited_vertex[adjacent_vertex])
        {
            return cycle_presence_check(adjacent_vertex, adjacency_list);
        }
        //If the adjacent_vertex is already visited, then it is the destination vertex of a backedge if it's vertex departure
        //label is greater than that of the current source_vertex(in whose subtree the given adjacent_vertex is present)
        //In this case, the backedge vertex is a part of a cycle in the directed graph
        else
        {
            if(vertex_departure_label[adjacent_vertex] >= vertex_departure_label[source_vertex])
            {
                return true;
            }
        }
    }

    //Update the vertex_departure_label of the current source_vertex by incrementing the vertex_departure_label_counter
    vertex_departure_label[source_vertex] = vertex_departure_label_counter++;

    //Returning false indicates that the subtree rooted at the given source_vertex does not contain any edge which forms
    //or is a part of a cycle
    return false;
}
