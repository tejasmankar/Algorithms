#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>

using namespace std;

//Program to find the total number of connected components in the graph and also the connected component in the
//graph to which each vertex belongs

void modified_breadth_first_search(int, int, vector<int>&, vector< vector<int> >&);

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
            //Vector to store the connected component label corresponding to the connected component to which each vertex in the graph belongs
            //All values in this vector are initialized to 0 by default
            vector<int> connected_component_label(number_of_vertices);

            //Numbering of the connected components will begin from 1
            int vertex_connected_component_label(1);

            //We will call the modified BFS function for each vertex of the graph
            for(index = 0; index < number_of_vertices; index++)
            {
                //If the current source vertex being considered for the BFS procedure has not been assigned a connected component label yet
                if(connected_component_label[index] == 0)
                {
                    //Perform BFS on the graph with the vertex represented by the current index as the source vertex
                    modified_breadth_first_search(index, vertex_connected_component_label, connected_component_label, adjacency_list);
                    //Increment the connected component label value so that the vertices belonging to different connected components will have distinct labels
                    vertex_connected_component_label++;
                }
            }
            cout << "Total number of connected components in the graph is " << *max_element(connected_component_label.begin(), connected_component_label.end()) << "\n";
            for(index = 0; index < number_of_vertices; index++)
            {
                cout << "The vertex " << index << " belongs to the connected component " << connected_component_label[index] << "\n";
            }
        }
    }
    return 0;
}

//Updates the given connected_component_label vector by assigning all the vertices present in the same connected component as that of
//the given source_vertex the same connected component label which is equal to the given vertex_connected_component_label value
void modified_breadth_first_search(int source_vertex, int vertex_connected_component_label, vector<int> &connected_component_label, vector< vector<int> > &adjacency_list)
{
    //Queue in which the graph vertices are added for processing
    queue<int> vertex_queue;

    //visited_vertex vector contains one entry corresponding to each vertex of the graph. All are initially set to false,
    //but the value corresponding to any vertex is set to true when that vertex is discovered i.e. when that vertex is
    //inserted in the queue
    vector<bool> visited_vertex(adjacency_list.size());

    //Initially, insert just the source_vertex in the queue, set the corresponding entry in visited_vertex to true and
    //assign the given connected component label value(vertex_connected_component_label) to the source_vertex being considered
    vertex_queue.push(source_vertex);
    visited_vertex[source_vertex] = true;
    connected_component_label[source_vertex] = vertex_connected_component_label;

    //Process while the queue is not empty
    while(!vertex_queue.empty())
    {
        //Get the element present at the front of the queue
        int queue_front_vertex = vertex_queue.front();
        //For all the vertices adjacent to the queue_front_vertex which are not visited yet, insert that
        //vertex in the queue, set its entry in visited_vertex to true and store the connected component
        //label corresponding to each adjacent_vertex in the connected_component_label vector
        for(int index = 0; index < adjacency_list[queue_front_vertex].size(); index++)
        {
            //adjacent_vertex variable stores one of the vertices adjacent to the queue_front_vertex
            int adjacent_vertex = adjacency_list[queue_front_vertex][index];
            //If the adjacent_vertex is not visited
            if(!visited_vertex[adjacent_vertex])
            {
                vertex_queue.push(adjacent_vertex);
                visited_vertex[adjacent_vertex] = true;
                //Each newly discovered vertex is assigned the same connected_component_label as that of the current source_vertex
                //as all the vertices discovered by performing BFS on the source_vertex are present in the same connected component
                //as that of the source_vertex
                connected_component_label[adjacent_vertex] = vertex_connected_component_label;
            }
        }
        //Remove the front element of the queue
        vertex_queue.pop();
    }
}
