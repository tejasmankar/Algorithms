#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>

using namespace std;

//Program to check whether the given undirected graph is bipartite

bool is_bipartite(int, vector<int>&, vector< vector<int> >&);

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
            //If the number of edges in the graph is zero
            if(number_of_edges == 0)
            {
                if(number_of_vertices == 2)
                {
                    cout << "The graph is trivially bipartite\n";
                }
                else
                {
                    cout << "The graph is not bipartite\n";
                }
            }
            //If the number of edges to be inserted in the graph is more than 0, then get the starting and ending vertices
            //of all the edges that would be inserted in the graph. This portion of the code is bypassed when the graph has
            //only one vertex because in that case the graph will not have any edge
            else if(number_of_edges > 0)
            {
                cout << "Enter the starting and ending vertex of each edge separated by space:\n";
                for(index = 0; index < number_of_edges; index++)
                {
                    cin >> starting_vertex >> ending_vertex;
                    //Insert each vertex in the adjacency list of the other vertex
                    adjacency_list[starting_vertex].push_back(ending_vertex);
                    adjacency_list[ending_vertex].push_back(starting_vertex);
                }

                //Vector to store the label corresponding to the set to which each vertex in the graph belongs
                vector<int> vertex_label(number_of_vertices);
                //Initializing the set labels of all the vertices to -1
                //Vertices belonging to either of the two independent sets are labeled l or 2
                for(index = 0; index < number_of_vertices; index++)
                {
                    vertex_label[index] = -1;
                }

                //Passing the first vertex in the graph(by index) as the source vertex(We can pass any other vertex as well)
                if(is_bipartite(0, vertex_label, adjacency_list))
                {
                    cout << "The graph is bipartite\n";
                    //Print the set to which each vertex of the bipartite graph belongs
                    for(index = 0; index < number_of_vertices; index++)
                    {
                        cout << "Vertex " << index << " belongs to set " << vertex_label[index] << "\n";
                    }
                }
                else
                {
                    cout << "The graph is not bipartite\n";
                }
            }
        }
    }
    return 0;
}

//Returns true if the graph is bipartite, false otherwise. This function is a slight modification of the BFS algorithm
//Updates the given vertex_label vector by assigning each vertex present in the graph(which is represented by an
//individual index of the vertex_label vector), a label(0 or 1) corresponding to the vertex set to which it belongs
bool is_bipartite(int source_vertex, vector<int> &vertex_label, vector< vector<int> > &adjacency_list)
{
    //Queue in which the graph vertices are added for processing
    queue<int> vertex_queue;

    //Initially, insert just the source_vertex in the queue, set the corresponding entry in vertex_label to 0
    vertex_queue.push(source_vertex);
    vertex_label[source_vertex] = 0;

    //Process while the queue is not empty
    while(!vertex_queue.empty())
    {
        //Get the element present at the front of the queue
        int queue_front_vertex = vertex_queue.front();
        //For all the vertices adjacent to the queue_front_vertex which are not visited yet, insert that vertex in the
        //queue, set its entry in visited_vertex to true and store its vertex label in the vertex_label vector
        for(int index = 0; index < adjacency_list[queue_front_vertex].size(); index++)
        {
            //adjacent_vertex variable stores one of the vertices adjacent to the queue_front_vertex
            int adjacent_vertex = adjacency_list[queue_front_vertex][index];
            //If the adjacent_vertex is not visited
            if(vertex_label[adjacent_vertex] == -1)
            {
                //Push the adjacent_vertex in the queue
                vertex_queue.push(adjacent_vertex);
                //Assign the adjacent_vertex a label other than that of the queue_front_vertex as the two vertices are adjacent
                (vertex_label[queue_front_vertex] == 0) ? (vertex_label[adjacent_vertex] = 1) : (vertex_label[adjacent_vertex] = 0);
            }
            //If both the queue_front_vertex and its adjacent vertex have the same vertex_label, then the graph is not bipartite
            else if(vertex_label[adjacent_vertex] == vertex_label[queue_front_vertex])
            {
                return false;
            }
        }
        //Remove the front element of the queue
        vertex_queue.pop();
    }
    //If none of the adjacent vertices have the same vertex_label, then the graph is bipartite
    return true;
}
