#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>

using namespace std;

//Program to find the diameter of an undirected graph
//The diameter of a graph is the largest of all the shortest distance values between any two vertices in the graph

void breadth_first_search(int, vector<int>&, vector< vector<int> >&);

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

            //Variable to store the value of the diameter of the graph
            int graph_diameter(-1);

            //Loop over all the vertices present in the graph because we need to perform BFS on every individual
            //vertex to find the diameter of the graph
            for(index = 0; index < number_of_vertices; index++)
            {
                //shortest_distance_from_source_vertex vector stores the shortest distance of each vertex from the source_vertex
                //All the entries of this vector are initialized to zero
                vector<int> shortest_distance_from_source_vertex(number_of_vertices);
                breadth_first_search(index, shortest_distance_from_source_vertex, adjacency_list);
                //Calculate the maximum shortest distance of any vertex from the given source vertex
                int maximum_distance = *max_element(shortest_distance_from_source_vertex.begin(), shortest_distance_from_source_vertex.end());
                //If the maximum_distance value is greater than the current graph_diameter value, set the graph_diameter value to be equal to the maximum_distance value
                if(maximum_distance > graph_diameter)
                {
                    graph_diameter = maximum_distance;
                }
            }
            cout << "The diameter of the graph is " << graph_diameter << "\n";
        }
    }
    return 0;
}

//Updates the given shortest_distance_from_source_vertex vector by assigning each vertex present in the graph(which is represented
//by an individual index of the shortest_distance_from_source_vertex vector), the shortest distance of itself from the given source_vertex
void breadth_first_search(int source_vertex, vector<int> &shortest_distance_from_source_vertex, vector< vector<int> > &adjacency_list)
{
    //Queue in which the graph vertices are added for processing
    queue<int> vertex_queue;

    //visited_vertex vector contains one entry corresponding to each vertex of the graph. All are initially set to false,
    //but the value corresponding to any vertex is set to true when that vertex is discovered i.e. when that vertex is
    //inserted in the queue
    vector<bool> visited_vertex(adjacency_list.size());

    //Initially, insert just the source_vertex in the queue, set the corresponding entry in visited_vertex to true
    //and update its shortest distance value to 0
    vertex_queue.push(source_vertex);
    visited_vertex[source_vertex] = true;
    shortest_distance_from_source_vertex[source_vertex] = 0;

    //Process while the queue is not empty
    while(!vertex_queue.empty())
    {
        //Get the element present at the front of the queue
        int queue_front_vertex = vertex_queue.front();
        //For all the vertices adjacent to the queue_front_vertex which are not visited yet, insert that
        //vertex in the queue, set its entry in visited_vertex to true and store its shortest distance from
        //the source_vertex value in the shortest_distance_from_source_vertex vector
        for(int index = 0; index < adjacency_list[queue_front_vertex].size(); index++)
        {
            //adjacent_vertex variable stores one of the vertices adjacent to the queue_front_vertex
            int adjacent_vertex = adjacency_list[queue_front_vertex][index];
            //If the adjacent_vertex is not visited
            if(!visited_vertex[adjacent_vertex])
            {
                vertex_queue.push(adjacent_vertex);
                visited_vertex[adjacent_vertex] = true;
                //The shortest distance of the adjacent_vertex from the given source_vertex is one more than the shortest
                //distance of the queue_front_vertex from the source_vertex(or the vertex due to which the adjacent_vertex
                //was inserted in the vertex_queue)
                shortest_distance_from_source_vertex[adjacent_vertex] = shortest_distance_from_source_vertex[queue_front_vertex] + 1;
            }
        }
        //Remove the front element of the queue
        vertex_queue.pop();
    }
}
