#include<iostream>
#include<queue>
#include<vector>

using namespace std;

//Program to implement Prim's algorithm to find the Minimum Spanning Tree(MST) in an undirected weighted-edge graph

//Defines the maximum length which any edge in the graph can have
#define MAX_EDGE_LENGTH 999

//Vector containing the label corresponding to the partition to which each vertex in the undirected graph belongs
vector<int> partition_label;

vector< pair< int, pair<int,int> > > build_minimum_spanning_tree(vector< vector< pair<int,int> > >&, int);
int find_other_endpoint_vertex(vector< vector< pair<int,int> > >&, pair<int, int>);

//Driver function
int main(void)
{
    int number_of_vertices(0), number_of_edges(0), starting_vertex(-1), ending_vertex(-1), source_vertex(-1), edge_length(0);
    cout << "Enter the number of vertices to be inserted in the graph:\n";
    cin >> number_of_vertices;
    if(number_of_vertices < 1)
    {
        cout << "The undirected graph must have at least one vertex\n";
    }
    else
    {
        cout << "The vertices inserted in the graph are:\n";
        for(int vertex = 0; vertex < number_of_vertices; vertex++)
        {
            cout << vertex << "\n";
        }

        //Adjacency list represented by a vector of vectors to store the vertices adjacent to each vertex of the undirected graph
        //Each entry of this vector of vectors would be a pair of integers. The first integer will be the length of the edge from
        //the vertex represented by the corresponding vector of vector index to the vertex represented by the second integer of the pair
        vector< vector< pair<int,int> > > adjacency_list(number_of_vertices);

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
                for(int edge = 0; edge < number_of_edges; edge++)
                {
                    cout << "Enter the starting and ending vertex of the edge:\n";
                    cin >> starting_vertex >> ending_vertex;
                    cout << "Enter the length of the edge:\n";
                    cin >> edge_length;
                    if(edge_length <= 0)
                    {
                        cout << "The length of any edge should be a positive non-zero integer\n";
                        return 1;
                    }

                    //Insert the pair containing the edge length and the other edge endpoint vertex in the adjacency list of both the edge vertices
                    adjacency_list[starting_vertex].push_back(make_pair(edge_length, ending_vertex));
                    adjacency_list[ending_vertex].push_back(make_pair(edge_length, starting_vertex));
                }
            }

            //Initialize all the values in the partition_label vector to 0. This is because initially the vertices do not belong to any of
            //the two partitions. Resize the vector so that it can hold the labels of all the vertices in the graph
            partition_label.resize(number_of_vertices, 0);

            cout << "Enter the source vertex(The vertex from which the Prim's algorithm would begin):\n";
            cin >> source_vertex;

            //Since the graph vertices are numbered from 0 to number_of_vertices - 1, it is invalid if the
            //source_vertex value does not fall in this range
            if((source_vertex > number_of_vertices - 1) || (source_vertex  < 0))
            {
                cout << "The source vertex entered is invalid\n";
            }
            else
            {
                //Get the edge lengths and the endpoint vertices of the edges present in the minimum spanning tree
                vector< pair< int, pair<int,int> > > minimum_spanning_tree = build_minimum_spanning_tree(adjacency_list, source_vertex);

                //Variable to store the sum of the lengths of all the edges present in the minimum spanning tree
                int minimum_spanning_tree_total_weight(0);

                cout << "The starting and ending vertices and the edge lengths of the edges added in the minimum spanning tree are:\n";
                for(int mst_edge = 0; mst_edge < minimum_spanning_tree.size(); mst_edge++)
                {
                    cout << "Starting vertex: " << minimum_spanning_tree[mst_edge].second.first << " ";
                    cout << "Ending vertex: " << minimum_spanning_tree[mst_edge].second.second << " ";
                    cout << "Edge length: " << minimum_spanning_tree[mst_edge].first << "\n";

                    //Update the total weight of the MST by adding the length of the current edge to it
                    minimum_spanning_tree_total_weight += minimum_spanning_tree[mst_edge].first;
                }
                cout << "The total weight of the minimum spanning tree is " << minimum_spanning_tree_total_weight << "\n";
            }
        }
    }
    return 0;
}

//Selects the edges and vertices to be added in the minimum spanning tree and returns a vector containing the details corresponding
//to the edges present in the generated minimum spanning tree, given the adjacency_list of the graph and the source_vertex
vector< pair< int, pair<int,int> > > build_minimum_spanning_tree(vector< vector< pair<int,int> > > &adjacency_list, int source_vertex)
{
    //Minimum priority queue to return the edge having the smallest length among all the edges being considered
    priority_queue< pair<int, int>, vector< pair<int, int> >, greater< pair<int, int> > > min_priority_queue;
    //Vector to store the edge lengths and the endpoint vertices of the edges which are added to the MST
    vector< pair< int, pair<int,int> > > minimum_spanning_tree;

    //Insert a pair into the priority queue containing the source vertex and value of edge length as 0 initially
    min_priority_queue.push(make_pair(0, source_vertex));

    //Loop until the priority is not empty
    while(!min_priority_queue.empty())
    {
        //Get the edge in the priority queue having the minimum length
        pair<int, int> minimum_edge_length = min_priority_queue.top();
        //Remove the minimum length edge from the priority queue
        min_priority_queue.pop();

        //If the other endpoint vertex of the minimum length edge being considered is already added to the MST, then we
        //continue with the next iteration because if there are two paths between the endpoints of the edge and we include
        //the vertex which is already present in the MST, a cycle will be formed due to the other path/edge
        if(partition_label[minimum_edge_length.second] == 1)
        {
            continue;
        }
        //Else, set the entry corresponding to the endpoint vertex in the partition_label vector to 1
        //This means that the other endpoint vertex is added to the MST
        partition_label[minimum_edge_length.second] = 1;

        //The below condition is added so that we don't add the initial minimum_edge_length value of (0, source_vertex) to the MST
        //as this value does not correspond to any edge in the graph. It's just an initial value so that we start the procedure
        //from the given source_vertex
        if(minimum_edge_length.second != source_vertex)
        {
            //Find the other endpoint vertex of the selected edge having the minimum length
            int minimum_edge_mst_partition_vertex = find_other_endpoint_vertex(adjacency_list, minimum_edge_length);
            //Add the length and the endpoint vertices of the selected edge to the minimum_spanning_tree vector
            minimum_spanning_tree.push_back(make_pair(minimum_edge_length.first, make_pair(minimum_edge_mst_partition_vertex, minimum_edge_length.second)));
        }

        //Add the pairs containing other endpoint vertices adjacent to the endpoint vertex of the current smallest edge and the length
        //of the corresponding edges such that the other endpoint vertices are not yet present in the MST
        for(int index = 0; index < adjacency_list[minimum_edge_length.second].size(); index++)
        {
            pair<int, int> adjacent_vertex = adjacency_list[minimum_edge_length.second][index];

            if(partition_label[adjacent_vertex.second] == 0)
            {
                min_priority_queue.push(adjacent_vertex);
            }
        }
    }

    //Returns the vector containing the edges present in the minimum spanning tree
    return minimum_spanning_tree;
}

//Returns the other endpoint vertex of an edge having the given length other than the one already present in the
//minimum_edge_length pair containing the edge length and one endpoint vertex
int find_other_endpoint_vertex(vector< vector< pair<int,int> > > &adjacency_list, pair<int, int> minimum_edge_length)
{
    //variable to store the selected vertex
    int edge_vertex(0);

    //Loop over all the vertices adjacent to the given endpoint vertex of the edge
    for(int index = 0; index < adjacency_list[minimum_edge_length.second].size(); index++)
    {
        //If the edge length of the current pair matches the edge length of the given edge and if the other endpoint vertex
        //of the edge is already present in the MST, then store the current endpoint vertex value(second value of the current pair)
        //and break out of the loop. This other endpoint vertex(having partition label value 1) will be the same vertex as the one
        //represented by the second value of the given minimum_edge_length pair
        if((adjacency_list[minimum_edge_length.second][index].first == minimum_edge_length.first) && (partition_label[adjacency_list[minimum_edge_length.second][index].second] == 1))
        {
            edge_vertex = adjacency_list[minimum_edge_length.second][index].second;
            break;
        }
    }

    //Return the selected other endpoint vertex of the given edge
    return edge_vertex;
}
