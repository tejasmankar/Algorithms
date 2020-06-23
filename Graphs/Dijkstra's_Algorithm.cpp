#include<iostream>
#include<queue>
#include<vector>
#include<stack>

using namespace std;

//Program to implement Dijkstra's algorithm to individually find the shortest path between the given source vertex and all the other vertices in a weighted directed graph

//Defines the maximum length which any edge in the graph can have. The length of any edge is greater than or equal to zero
#define MAX_EDGE_LENGTH 999

//Defines the initial length of the shortest path between any two vertices in the directed graph
#define INFINITY 32767

//Vector containing the shortest distance values between the source vertex and all the other vertices in the graph
vector<int> shortest_distance_label;

vector< pair< int, pair<int, int> > > find_shortest_path(vector< vector< pair<int, pair<int, int> > > >&, int);
void print_shortest_path(vector< pair< int, pair<int, int> > >, int, int, int);

//Driver function
int main(void)
{
    int number_of_vertices(0), number_of_edges(0), starting_vertex(-1), ending_vertex(-1), source_vertex(-1), destination_vertex(-1), edge_length(0);
    cout << "Enter the number of vertices to be inserted in the graph:\n";
    cin >> number_of_vertices;
    if(number_of_vertices < 1)
    {
        cout << "The directed graph must have at least one vertex\n";
    }
    else
    {
        cout << "The vertices inserted in the graph are:\n";
        for(int vertex = 0; vertex < number_of_vertices; vertex++)
        {
            cout << vertex << "\n";
        }

        //Adjacency list represented by a vector of vectors to store the vertices adjacent to each vertex of the directed graph
        //Each entry of this vector of vectors would contain three integers. The first integer will be the length of the edge from
        //the vertex represented by the second integer to the vertex represented by the third integer of the pair
        vector< vector< pair<int, pair<int, int> > > > adjacency_list(number_of_vertices);

        cout << "Enter the number of edges to be inserted in the graph:\n";
        cin >> number_of_edges;
        //If the number of edges is more than n * (n-1) i.e. the maximum number of edges which an undirected graph on n vertices could have
        if(number_of_edges > number_of_vertices * (number_of_vertices - 1))
        {
            cout << "A directed graph on " << number_of_vertices << " vertices has at most " << number_of_vertices * (number_of_vertices - 1) << " edge(s)\n";
        }
        //The given directed graph should have at least one edge
        else if(number_of_edges < 1)
        {
            cout << "The graph should have at least two edges\n";
        }
        else
        {
            //If the number of edges to be inserted in the graph is more than 0, then get the starting and ending vertices
            //of all the edges that would be inserted in the graph. This portion of the code is bypassed when the graph has
            //only one vertex because in that case the graph will not have any edge
            if(number_of_edges > 0)
            {
                for(int edge = 0; edge < number_of_edges; edge++)
                {
                    cout << "Enter the starting and ending vertex of the edge:\n";
                    cin >> starting_vertex >> ending_vertex;
                    cout << "Enter the length of the edge:\n";
                    cin >> edge_length;
                    //The length of any edge should not be negative i.e. the edge length should be at least zero
                    if(edge_length <= 0)
                    {
                        cout << "The length of any edge should be a positive non-zero integer\n";
                        return 1;
                    }

                    //Insert the pair containing the edge length and the pair of starting and ending vertex of the edge in the adjacency list
                    adjacency_list[starting_vertex].push_back(make_pair(edge_length, make_pair(starting_vertex, ending_vertex)));
                }
            }

            //Initialize all the values in the shortest_distance_label vector to INFINITY. This is because initially we do not know of
            //any path between any pair of vertices. Resize the vector so that it can hold the labels of all the vertices in the graph
            shortest_distance_label.resize(number_of_vertices, INFINITY);

            cout << "Enter the source vertex from which the shortest path is to be found:\n";
            cin >> source_vertex;

            //Flag variable to identify if the source or destination vertex is invalid. It is set to 1 if any of the two vertices is invalid
            int flag(0);

            //Since the graph vertices are numbered from 0 to number_of_vertices - 1, it is invalid if the source_vertex
            //value does not fall in this range
            if((source_vertex > number_of_vertices - 1) || (source_vertex  < 0))
            {
                cout << "The source vertex entered is invalid\n";
                flag = 1;
            }

            //If the source vertex value is not invalid
            if(!flag)
            {
                //Get the edge lengths and the endpoint vertices of the edges present in the shortest path between the source and the destination vertex
                vector< pair< int, pair<int, int> > > shortest_path = find_shortest_path(adjacency_list, source_vertex);
                //Print the shortest path and the total length of the shortest path
                for(destination_vertex = 0; destination_vertex < number_of_vertices; destination_vertex++)
                {
                    print_shortest_path(shortest_path, number_of_vertices, source_vertex, destination_vertex);
                }
            }
        }
    }
    return 0;
}

//Selects the edges and vertices to be added to the shortest path and returns a vector containing the details corresponding
//to the edges present in the generated shortest path, given the adjacency_list of the graph and the source_vertex
vector< pair< int, pair<int, int> > > find_shortest_path(vector< vector< pair<int, pair<int, int> > > > &adjacency_list, int source_vertex)
{
    //Minimum priority queue to return the edge having the smallest length among all the edges being considered
    priority_queue< pair<int, pair<int, int> >, vector< pair<int, pair<int, int> > >, greater< pair<int, pair<int, int> > > > min_priority_queue;
    //Vector to store the edge lengths and the endpoint vertices of the edges which are added to the shortest path
    //This vector is of length number_of_vertices since we are adding entries to it indexed by the destination vertex label
    vector< pair< int, pair<int, int> > > shortest_path(adjacency_list.size());

    //Insert a pair into the priority queue containing the edge length and a pair containing a placeholder vertex and the given source vertex
    //The value of this edge length is 0 since we are starting at the source vertex directly
    min_priority_queue.push(make_pair(0, make_pair(adjacency_list.size(), source_vertex)));

    //Assign the source vertex a shortest distance label of 0
    shortest_distance_label[source_vertex] = 0;

    //Insert the source vertex into the shortest path vector
    shortest_path[source_vertex] = make_pair(0, make_pair(adjacency_list.size(), source_vertex));

    //Loop until the priority queue is not empty
    while(!min_priority_queue.empty())
    {
        //Get the vertex in the priority queue having the minimum shortest distance label value
        pair< int, pair<int, int> > minimum_shortest_distance_label = min_priority_queue.top();
        //Remove the vertex having the minimum shortest distance label value from the priority queue
        min_priority_queue.pop();

        //Check if a shorter path exists between the source vertex and each of the out-adjacent vertices of the newly added vertex containing the edge
        //between the newly added vertex and the current adjacent vertex being considered
        for(int index = 0; index < adjacency_list[minimum_shortest_distance_label.second.second].size(); index++)
        {
            pair< int, pair<int, int> > adjacent_vertex = adjacency_list[minimum_shortest_distance_label.second.second][index];

            //If the length of the path from the source vertex to the current adjacent vertex containing the edge between the vertex which is newly
            //added to the shortest path and the current adjacent vertex is less than the length of the current shortest path between the source vertex
            //and the current adjacent vertex(distance label of the current adjacent vertex), then update the distance label of the current adjacent vertex
            //and add the updated distance label value in the priority queue
            if(shortest_distance_label[minimum_shortest_distance_label.second.second] + adjacent_vertex.first < shortest_distance_label[adjacent_vertex.second.second])
            {
                shortest_distance_label[adjacent_vertex.second.second] = shortest_distance_label[minimum_shortest_distance_label.second.second] + adjacent_vertex.first;
                //Add the updated shortest length and the endpoint vertices of the selected edge and vertex at the index corresponding to the destination(second)
                //vertex label of the edge in the shortest_path vector
                shortest_path[adjacent_vertex.second.second] = make_pair(adjacent_vertex.first, make_pair(adjacent_vertex.second.first, adjacent_vertex.second.second));
                min_priority_queue.push(adjacent_vertex);
            }
        }
    }

    //Returns the vector containing the edges present in the shortest path
    return shortest_path;
}

//Prints the vertices and the lengths of the edges present in the shortest path and the total length of the shortest path between the given source_vertex and destination_vertex
void print_shortest_path(vector< pair< int, pair<int, int> > > shortest_path, int number_of_vertices, int source_vertex, int destination_vertex)
{
    //If the shortest_distance_label of the destination vertex is INFINITY, then there is no path in the graph between the source and the destination vertex
    if(shortest_distance_label[destination_vertex] == INFINITY)
    {
        cout << "There is no path connecting the source and destination vertex in the directed graph\n";
    }
    else
    {
        //Variable storing the immediate predecessor of a vertex in the shortest path
        pair< int, pair<int, int> > parent_vertex(shortest_path[destination_vertex]);
        //Stack to print the vertices present in the shortest path in the proper order from the source to the destination vertex
        stack< pair< int, pair<int, int> > > shortest_path_vertices;

        //Push the successive predecessor vertices of all the vertices in the shortest path, beginning from the destination_vertex to the source vertex into the stack
        //This is done so as to print the vertices and lengths of the edges in the shortest path in the proper order from the source to the destination vertex
        //while popping the elements out of the stack
        //We are comparing with the first vertex of every element with the number_of_vertices(or adjacency_list.size()) value because we do not want to add
        //the vector element containing the pair of placeholder vertex and the given source vertex
        while(parent_vertex.second.first != number_of_vertices)
        {
            shortest_path_vertices.push(parent_vertex);
            parent_vertex = shortest_path[parent_vertex.second.first];
        }
        cout << "The starting and ending vertices and the edge lengths of the edges present in the shortest path between " << source_vertex << " and " << destination_vertex << " are:\n";
        while(!shortest_path_vertices.empty())
        {
            cout << "Starting vertex: " << shortest_path_vertices.top().second.first << " ";
            cout << "Ending vertex: " << shortest_path_vertices.top().second.second << " ";
            cout << "Edge length: " << shortest_path_vertices.top().first << "\n";

            //Pop the top element out of the stack
            shortest_path_vertices.pop();
        }
        cout << "The total length of the shortest path between " << source_vertex << " and " << destination_vertex << " is " << shortest_distance_label[destination_vertex] << "\n";
    }
}
