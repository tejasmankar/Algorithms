#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>

using namespace std;

//Program to implement Depth First Search(DFS) on an undirected graph

void depth_first_search(int, vector<bool>&, vector< vector<int> >&);

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

            cout << "Enter the source vertex(The vertex from which the DFS algorithm would begin):\n";
            cin >> source_vertex;

            //Since the graph vertices are numbered from 0 to number_of_vertices - 1, it is invalid if the
            //source_vertex value does not fall in this range
            if((source_vertex > number_of_vertices - 1) || (source_vertex  < 0))
            {
                cout << "The source vertex entered is invalid\n";
            }
            else
            {
                //visited_vertex vector contains one entry corresponding to each vertex of the graph. All are initially set to false,
                //but the value corresponding to any vertex is set to true(by the depth_first_search function) when that vertex is visited
                vector<bool> visited_vertex(number_of_vertices);

                cout << "The vertices visited in the DFS traversal of the graph are:\n";
                depth_first_search(source_vertex, visited_vertex, adjacency_list);
            }
        }
    }
    return 0;
}

//Prints the given source_vertex and then recursively performs DFS on the graph beginning at any vertex adjacent to the given
//source_vertex which has not been visited yet
void depth_first_search(int source_vertex, vector<bool> &visited_vertex, vector< vector<int> > &adjacency_list)
{
    //Set the entry corresponding to the given source_vertex in the visited_vertex vector to true
    visited_vertex[source_vertex] = true;

    //Print the given source_vertex
    cout << source_vertex << "\n";

    //Loop over all the vertices adjacent to the given source_vertex
    for(int index = 0; index < adjacency_list[source_vertex].size(); index++)
    {
        //adjacent_vertex variable stores one of the vertices adjacent to the given source_vertex
        int adjacent_vertex = adjacency_list[source_vertex][index];
        //If the adjacent_vertex is not visited, perform DFS recursively with the adjacent_vertex as the new source_vertex
        if(!visited_vertex[adjacent_vertex])
        {
            depth_first_search(adjacent_vertex, visited_vertex, adjacency_list);
        }
    }
}
