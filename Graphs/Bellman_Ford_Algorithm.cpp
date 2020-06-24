#include<iostream>
#include<vector>
#include<stack>

using namespace std;

//Program to implement Bellman-Ford algorithm to individually find the shortest path between the given source vertex and all the other vertices in a weighted directed graph
//The edge lengths of the graph can be negative but the graph should not contain any negative cycle

//Defines the maximum length which any edge in the graph can have
#define MAX_EDGE_LENGTH 999

//Defines the initial length of the shortest path between any two vertices in the directed graph
#define INFINITY 16000

//Vector containing the shortest path distance values between the source vertex and all the other vertices in the graph
vector<int> shortest_distance_label;

vector< pair< int, pair<int, int> > > find_shortest_path(vector< vector< pair<int, pair<int, int> > > >&, int);
void print_shortest_path(vector< pair< int, pair<int, int> > >, int, int, int);

//Driver function
int main(void)
{
    int number_of_vertices(0), number_of_edges(0), starting_vertex(-1), ending_vertex(-1), source_vertex(-1), edge_length(0);
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
        cout << "The graph should not contain any negative cycle\n";
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

                    //Insert the pair containing the edge length and the pair of starting and ending vertex of the edge in the adjacency list
                    //location having the index value equal to the ending_vertex value
                    adjacency_list[ending_vertex].push_back(make_pair(edge_length, make_pair(starting_vertex, ending_vertex)));
                }
            }

            //Initialize all the values in the shortest_distance_label vector to INFINITY. This is because initially we do not know of
            //any path between any pair of vertices. Resize the vector so that it can hold the labels of all the vertices in the graph
            shortest_distance_label.resize(number_of_vertices, INFINITY);

            cout << "Enter the source vertex from which the shortest paths are to be found:\n";
            cin >> source_vertex;

            //Flag variable to identify if the given source vertex is invalid. It is set to 1 if the source vertex value is invalid
            int flag(0);

            //Since the graph vertices are numbered from 0 to number_of_vertices - 1, it is invalid if the source_vertex value does not fall in this range
            if((source_vertex > number_of_vertices - 1) || (source_vertex  < 0))
            {
                cout << "The source vertex entered is invalid\n";
                flag = 1;
            }

            //If the source vertex value is not invalid
            if(!flag)
            {
                //Get the edge lengths and the endpoint vertices of the edges present in the shortest path between the source vertex and all the other vertices
                vector< pair< int, pair<int, int> > > shortest_path = find_shortest_path(adjacency_list, source_vertex);
                //Print the shortest path and the total length of the shortest path between the source vertex and every other vertex
                for(int destination_vertex = 0; destination_vertex < number_of_vertices; destination_vertex++)
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
    //Vector to store the edge lengths and the endpoint vertices of the edges which are added to the shortest path
    //This vector is of length number_of_vertices since we are adding entries to it indexed by the destination vertex label
    vector< pair< int, pair<int, int> > > updated_shortest_path_length(adjacency_list.size(), make_pair(INFINITY, make_pair(0, 0)));
    //Vector to store the initial edge lengths and the endpoint vertices of the edges which are added to the shortest path.
    //It also stores the intermediate values of the shortest path lengths during the process iterations
    vector< pair< int, pair<int, int> > > initial_shortest_path_length(adjacency_list.size(), make_pair(INFINITY, make_pair(0, 0)));

    //Insert a pair into the initial_shortest_path_length and the updated_shortest_path_length vectors containing the edge length and a
    //pair containing a placeholder vertex and the given source vertex
    //The value of this edge length is 0 since we are starting at the source vertex directly
    initial_shortest_path_length[source_vertex] = make_pair(0, make_pair(adjacency_list.size(), source_vertex));
    updated_shortest_path_length[source_vertex] = make_pair(0, make_pair(adjacency_list.size(), source_vertex));

    //Assign the source vertex a shortest distance label of 0
    shortest_distance_label[source_vertex] = 0;

    //Iterate adjacency_list.size() - 1 times since the shortest path between the source and all the other vertices will have at most that many number of edges
    for(int iteration = 1; iteration < adjacency_list.size(); iteration++)
    {
        //Iterate over all the vertices in the directed graph
        for(int edge_end_vertex = 0; edge_end_vertex < adjacency_list.size(); edge_end_vertex++)
        {
            //Iterate over all the edges in-adjacent to the current edge_end_vertex
            for(int index = 0; index < adjacency_list[edge_end_vertex].size(); index++)
            {
                pair< int, pair<int, int> > adjacent_vertex = adjacency_list[edge_end_vertex][index];

                //If the length of the path from the source vertex to the current edge_end_vertex through the current adjacent vertex being considered
                //is less than the length of the current shortest path between the source vertex and the current adjacent vertex, then update the distance
                //label of the edge_end_vertex and add the edge between the adjacent vertex and the edge_end_vertex in the updated_shortest_path_length vector
                if(initial_shortest_path_length[adjacent_vertex.second.first].first + adjacent_vertex.first < initial_shortest_path_length[edge_end_vertex].first)
                {
                    //Add the updated shortest length in the shortest_distance_label vector and the endpoint vertices of the selected edge in the updated_shortest_path
                    //vector at the index corresponding to the destination(second) vertex/edge_end_vertex label of the edge
                    updated_shortest_path_length[edge_end_vertex] = make_pair(adjacent_vertex.first, make_pair(adjacent_vertex.second.first, edge_end_vertex));
                    shortest_distance_label[edge_end_vertex] = shortest_distance_label[adjacent_vertex.second.first] + adjacent_vertex.first;
                }
            }
        }
        initial_shortest_path_length = updated_shortest_path_length;
    }

    //Returns the vector containing the edges present in the shortest path
    //We can return updated_shortest_path_length as well since it will be equal to initial_shortest_path_length after all the iterations
    return initial_shortest_path_length;
}

//Prints the vertices and the lengths of the edges present in the shortest path and the total length of the shortest path between the given source_vertex and destination_vertex
void print_shortest_path(vector< pair< int, pair<int, int> > > shortest_path, int number_of_vertices, int source_vertex, int destination_vertex)
{
    //If the shortest_distance_label of the destination vertex is INFINITY, then there is no path in the graph between the source and the destination vertex
    if(shortest_path[destination_vertex].first == INFINITY)
    {
        cout << "There is no path connecting " << source_vertex << " and " << destination_vertex << " in the directed graph\n";
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
        //If both the source and the destination vertex are the same, then there is no edge connecting the same vertices in the directed graph
        if(source_vertex != destination_vertex)
        {
            cout << "The starting and ending vertices and the edge lengths of the edges present in the shortest path between " << source_vertex << " and " << destination_vertex << " are:\n";
            while(!shortest_path_vertices.empty())
            {
                cout << "Starting vertex: " << shortest_path_vertices.top().second.first << " ";
                cout << "Ending vertex: " << shortest_path_vertices.top().second.second << " ";
                cout << "Edge length: " << shortest_path_vertices.top().first << "\n";

                //Pop the top element out of the stack
                shortest_path_vertices.pop();
            }
        }
        cout << "The total length of the shortest path between " << source_vertex << " and " << destination_vertex << " is " << shortest_distance_label[destination_vertex] << "\n";
    }
}
