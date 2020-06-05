#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

//Program to implement Kruskal's algorithm to find the Minimum Spanning Tree(MST) in an undirected weighted-edge graph

//Structure of an edge object of the graph
struct EdgeObject
{
    int edge_length;        //Length of the edge
    int starting_vertex;    //Vertex from which the given edge starts
    int ending_vertex;      //Vertex at which the given edge ends
};


//Structure of a vertex/element in the Union-Find tree
struct UnionFindObject
{
    int vertex_rank;                   //Stores the rank of the vertex represented by the object
    UnionFindObject *parent_node;      //Pointer to the parent of the vertex/element in the Union-Find tree
};

//Defines the maximum number of vertices which the graph can hold
#define MAX_SIZE 999

//Vector to store the pointer to the UnionFindObject object corresponding to each graph vertex in the Union-Find forest(collection of trees)
vector<UnionFindObject*> union_find_object_vector;

vector<EdgeObject> fetch_edge_object_vector();
EdgeObject create_edge_object(int, int, int);
bool compare_parameter(EdgeObject, EdgeObject);
vector<EdgeObject> build_minimum_spanning_tree(vector<EdgeObject>);
void initialize_union_find_object_vector(int);
UnionFindObject* connected_component(UnionFindObject*, int);
void union_find_tree_path_compressor(UnionFindObject*, UnionFindObject*);
void update_connected_component_root_vertex_rank(UnionFindObject*, int);
void merge_connected_components(UnionFindObject*, UnionFindObject*);
void post_union_updates(UnionFindObject*, UnionFindObject*);

//Driver function
int main(void)
{
    //Get the EdgeObject vector consisting of objects containing the length, starting and ending vertex corresponding to every edge in the graph
    vector<EdgeObject> edge_object_vector = fetch_edge_object_vector();
    //If there are no graph edges having a positive length, we exit the program
    if(edge_object_vector.size() == 0)
    {
        return 1;
    }
    //Sort the edge objects in the edge_object_vector in ascending order of their edge_length values
    //The structure parameter to be used for comparing any two edge objects is specified by the compare_parameter function
    sort(edge_object_vector.begin(), edge_object_vector.end(), compare_parameter);
    //Get the edge objects of the edges present in the minimum spanning tree
    vector<EdgeObject> minimum_spanning_tree = build_minimum_spanning_tree(edge_object_vector);
    //Variable to store the sum of the lengths of all the edges present in the minimum spanning tree
    int minimum_spanning_tree_total_weight(0);
    cout << "The starting and ending vertices and the edge lengths of the edges added in the minimum spanning tree are:\n";
    for(int edge_object = 0; edge_object < minimum_spanning_tree.size(); edge_object++)
    {
        cout << "Starting vertex: " << minimum_spanning_tree[edge_object].starting_vertex << " ";
        cout << "Ending vertex: " << minimum_spanning_tree[edge_object].ending_vertex << " ";
        cout << "Edge length: " << minimum_spanning_tree[edge_object].edge_length << "\n";

        //Update the total weight of the MST by adding the length of the current edge to it
        minimum_spanning_tree_total_weight += minimum_spanning_tree[edge_object].edge_length;
    }
    cout << "The total weight of the minimum spanning tree is " << minimum_spanning_tree_total_weight << "\n";
    return 0;
}

//Accepts user input and returns the EdgeObject vector containing an edge object corresponding to each edge
vector<EdgeObject> fetch_edge_object_vector()
{
    int number_of_vertices(0), number_of_edges(0), starting_vertex(-1), ending_vertex(-1), index(0), edge_length(0);
    //Vector to store the edge object corresponding to each edge present in the graph
    vector<EdgeObject> edge_object_vector;

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
                for(index = 0; index < number_of_edges; index++)
                {
                    cout << "Enter the starting and ending vertex of the edge separated by space:\n";
                    cin >> starting_vertex >> ending_vertex;
                    cout << "Enter the length of the edge:\n";
                    cin >> edge_length;
                    if(edge_length <= 0)
                    {
                        cout << "The length of any edge should be a positive non-zero integer\n";
                        return edge_object_vector;
                    }
                    //Create an edge object to store the edge specific details
                    struct EdgeObject edge_object = create_edge_object(edge_length, starting_vertex, ending_vertex);
                    //Insert the newly created edge object into the edge_object_vector
                    edge_object_vector.push_back(edge_object);
                }
            }
            initialize_union_find_object_vector(number_of_vertices);
        }
    }

    //Returns the vector containing the edge objects
    return edge_object_vector;
}

//Creates and returns a new edge object after initializing it with the given values
EdgeObject create_edge_object(int edge_length, int starting_vertex, int ending_vertex)
{
    //An object of type EdgeObject is declared
    EdgeObject edge_object;
    edge_object.edge_length = edge_length;
    edge_object.starting_vertex = starting_vertex;
    edge_object.ending_vertex = ending_vertex;

    //Returns the EdgeObject object by value
    return edge_object;
}

//Selects the structure parameter based on whose value the edge objects will be sorted by the std::sort() function
bool compare_parameter(EdgeObject a, EdgeObject b)
{
    //Edge objects will be sorted according to their edge_length value
    //Edge object having a smaller edge_length value will be placed before the objects having a larger edge_length value
    return a.edge_length < b.edge_length;
}

//Selects the edges to be added in the minimum spanning tree and returns a vector containing the edge objects corresponding
//to the edges present in the generated minimum spanning tree
vector<EdgeObject> build_minimum_spanning_tree(vector<EdgeObject> edge_object_vector)
{
    //Vector to store the edges of the minimum spanning tree
    vector<EdgeObject> minimum_spanning_tree;

    for(int edge = 0; edge < edge_object_vector.size(); edge++)
    {
        //Get the pointers to the root vertices of the connected components(UnionFindObjects) containing the vertex objects corresponding
        //to the starting and ending vertices of the edge represented by the current edge object
        UnionFindObject *starting_vertex_object = union_find_object_vector[edge_object_vector[edge].starting_vertex];
        UnionFindObject *ending_vertex_object = union_find_object_vector[edge_object_vector[edge].ending_vertex];
        UnionFindObject *starting_vertex_connected_component = connected_component(starting_vertex_object, starting_vertex_object->vertex_rank);
        UnionFindObject *ending_vertex_connected_component = connected_component(ending_vertex_object, ending_vertex_object->vertex_rank);

        //If the starting and ending vertex of the current edge being considered do not belong to the same union-find connected
        //component, then add this edge to the minimum spanning tree and merge the connected components containing both these vertices
        //If both the vertices of the current edge belong to the same connected component, then we would get a cycle in the corresponding
        //minimum spanning tree and hence we do not add such an edge to the minimum spanning tree
        if(starting_vertex_connected_component != ending_vertex_connected_component)
        {
            merge_connected_components(starting_vertex_connected_component, ending_vertex_connected_component);
            minimum_spanning_tree.push_back(edge_object_vector[edge]);
        }
    }

    //Returns the vector containing the edges present in the minimum spanning tree
    return minimum_spanning_tree;
}

//Initializes UnionFindObjects for all the vertices in the graph and inserts the pointers to those objects in the union_find_object_vector
void initialize_union_find_object_vector(int number_of_vertices)
{
    for(int vertex = 0; vertex < number_of_vertices; vertex++)
    {
        UnionFindObject *vertex_object = new UnionFindObject();
        vertex_object->vertex_rank = 1;
        vertex_object->parent_node = nullptr;
        union_find_object_vector.push_back(vertex_object);
    }
}

//Returns a pointer to the root vertex of the union-find connected component containing the given vertex and having the given
//number of vertices
//This function will perform the find operation on the union-find data structure
UnionFindObject* connected_component(UnionFindObject *vertex_object, int connected_component_total_vertices)
{
    //If the current vertex has a parent, recursively check its parent until we reach the root vertex, whose parent_node will have value nullptr
    if(vertex_object->parent_node != nullptr)
    {
        //Increment the number of vertices present in the connected component
        connected_component_total_vertices++;
        //Recursively perform the same operation on the parent of the given vertex
        UnionFindObject *connected_component_root_vertex = connected_component(vertex_object->parent_node, connected_component_total_vertices);
        //Compress the path in the connected component from the given vertex to the root vertex
        union_find_tree_path_compressor(vertex_object, connected_component_root_vertex);
        //Return the pointer to the root vertex of the connected component
        return connected_component_root_vertex;
    }
    //Update the rank of the root vertex of the connected component
    update_connected_component_root_vertex_rank(vertex_object, connected_component_total_vertices);

    //Return the pointer to the root vertex of the connected component
    return vertex_object;
}

//Updates the parent_node pointer of the given union-find vertex to point to the root vertex of its connected component
//This will ensure that eventually the root vertex of the connected component will be connected by just one edge to all the other
//vertices in the connected component
void union_find_tree_path_compressor(UnionFindObject *vertex_object, UnionFindObject *connected_component_root_vertex)
{
    vertex_object->parent_node = connected_component_root_vertex;
}

//Updates the rank of the root vertex of the connected component containing the given vertex to the given connected_component_total_vertices value
void update_connected_component_root_vertex_rank(UnionFindObject *vertex_object, int connected_component_total_vertices)
{
    vertex_object->vertex_rank = connected_component_total_vertices;
}

//Merges the connected components represented by the given root vertex pointers
//This function will perform the union operation on the union-find data structure
void merge_connected_components(UnionFindObject *starting_vertex_connected_component, UnionFindObject *ending_vertex_connected_component)
{
    //Make the parent pointer of the root vertex of the connected component having lesser number of vertices point to the
    //root vertex of the connected component having larger number of nodes and update their respective ranks
    //If the number of vertices in both the connected components is equal, then the parent pointer of any root vertex can
    //point to the other
    if(starting_vertex_connected_component->vertex_rank <= ending_vertex_connected_component->vertex_rank)
    {
        post_union_updates(starting_vertex_connected_component, ending_vertex_connected_component);
    }
    else
    {
        post_union_updates(ending_vertex_connected_component, starting_vertex_connected_component);
    }
}

//Updates the parent node pointer and vertex rank of the vertex objects represented by the given pointers after merging their respective connected components
void post_union_updates(UnionFindObject *minimum_rank_connected_component_root_vertex, UnionFindObject *maximum_rank_connected_component_root_vertex)
{
    //The higher ranked root vertex will become the parent of the lower ranked root vertex
    minimum_rank_connected_component_root_vertex->parent_node = maximum_rank_connected_component_root_vertex;
    //Update the ranks of both the root vertices
    //The rank of the older root vertex of the connected component having lesser number of vertices is set to 1
    minimum_rank_connected_component_root_vertex->vertex_rank = 1;
    //The rank of the root vertex of the newly merged connected component is equal to the total number of vertices
    //present in both the connected components whose root vertex pointers are given
    maximum_rank_connected_component_root_vertex->vertex_rank += minimum_rank_connected_component_root_vertex->vertex_rank;
}
