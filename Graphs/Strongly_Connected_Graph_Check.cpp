#include<iostream>
#include<vector>

using namespace std;

//Program to check whether a directed graph is strongly connected using Depth First Search(DFS)

//Defines the maximum number of vertices which the graph can hold
#define MAX_SIZE 999

//Assigns the vertex a label which is equal to the number of vertices visited before it during the DFS traversal
//This value refers to the position of the given vertex in the list of vertices discovered in order(successively) by the DFS
int visited_vertex_count(-1);

//visited_vertex vector contains one entry corresponding to each vertex of the graph. All are initially set to false,
//but the value corresponding to any vertex is set to true(by the depth_first_search function) when that vertex is visited
vector<bool> visited_vertex;

//Vector to store the backedge endpoint vertex present at the highest(numerically lowest) level for all the vertices in the graph
//It will store MAX_SIZE for any vertex which does not have a backedge
vector<int> vertex_arrival_label;

int minimum_backedge_vertex_arrival_label(int, vector< vector<int> >&);

//Driver function
int main()
{
    int number_of_vertices(0), number_of_edges(0), starting_vertex(-1), ending_vertex(-1), index(0);
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
            vertex_arrival_label.resize(number_of_vertices, MAX_SIZE);

            if(minimum_backedge_vertex_arrival_label(0, adjacency_list) == MAX_SIZE + 1)
            {
                //A directed graph having just one vertex is strongly connected
                if(number_of_vertices == 1)
                {
                    cout << "The given directed graph is strongly connected\n";
                }
                else
                {
                    cout << "The given directed graph is not strongly connected\n";
                }
            }
            else
            {
                //If the function does not return MAX_SIZE + 1, then the directed graph is strongly connected
                //only when all its vertices are visited. If even a single vertex is not visited, then the directed
                //graph is not strongly connected
                for(index = 0; index < number_of_vertices; index++)
                {
                    if(!visited_vertex[index])
                    {
                        cout << "The given directed graph is not strongly connected\n";
                        return 0;
                    }
                }
                cout << "The given directed graph is strongly connected\n";
            }
        }
    }
    return 0;
}

//Returns the arrival label corresponding to the vertex having the minimum such label and which is adjacent to the given
//source_vertex or returns MAX_SIZE + 1 if the given directed graph is not strongly connected
int minimum_backedge_vertex_arrival_label(int source_vertex, vector< vector<int> > &adjacency_list)
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
        //This is to determine if any edge of the subtree rooted at adjacent_vertex is a backedge
        if(!visited_vertex[adjacent_vertex])
        {
            minimum_backedge_vertex_label = minimum_backedge_vertex_arrival_label(adjacent_vertex, adjacency_list);
            //If the value returned is MAX_SIZE + 1, then the subgraph rooted at the current adjacent_vertex and by extension
            //the entire directed graph is not strongly connected
            if(minimum_backedge_vertex_label == MAX_SIZE + 1)
            {
                return minimum_backedge_vertex_label;
            }
        }

        //Update the value of highest_level_backedge_endpoint_vertex if its greater than the minimum_backedge_vertex_label of the
        //current adjacent vertex being considered(which means that the highest level backedge ancestor of the adjacent_vertex is at a
        //numerically lower level than the highest_level_backedge_endpoint_vertex discovered till now)
        if(minimum_backedge_vertex_label < highest_level_backedge_endpoint_vertex)
        {
            highest_level_backedge_endpoint_vertex = minimum_backedge_vertex_label;
        }
    }
    //If the level of the backedge vertex present at the highest level in the DFS tree is greater than(means the backedge vertex is present
    //at a lower level) that of the given source_vertex, then the subtree rooted at the given source_vertex is not strongly connected
    //source_vertex should not be zero because it's the minimum possible label(which is assigned to the root of the DFS tree) and any
    //label of the backedge vertex at the highest level will always be greater than or equal to zero when the source_vertex has label zero
    //(so the first condition will always be satisfied) and if we don't add the second condition, the function will declare every graph
    //to be strongly connected
    if((highest_level_backedge_endpoint_vertex > vertex_arrival_label[source_vertex]) && (source_vertex != 0))
    {
        return MAX_SIZE + 1;
    }

    //Return the minimum arrival label value corresponding to any adjacent vertex of the given source_vertex
    return highest_level_backedge_endpoint_vertex;
}
