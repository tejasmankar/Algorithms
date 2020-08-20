#include<iostream>

using namespace std;

//Program to implement the Merge Sort algorithm

void populate_array(int[], int[], int, int);
void merge_sort(int[], int, int);
void merge(int[], int, int, int);

int main(void)
{
    int number_of_values(0);

    cout << "Enter the number of values to be sorted:\n";
    cin >> number_of_values;  

    //Array to store the values to be sorted
    int values[number_of_values] = {0};

    cout << "Enter the values to be sorted:\n";
    for(int index = 0; index < number_of_values; index++)
    {
        cin >> values[index];
    }

    //Function call to sort the given array in ascending order
    merge_sort(values, 0, number_of_values - 1);

    cout << "The values in sorted(ascending) order are as follows:\n";
    for(int index = 0; index < number_of_values; index++)
    {
        cout << values[index] << " ";
    }
    cout << "\n";

    return 0;
}

//Populates the given target_array by copying the elements of the given source_array, starting from the given start_index to the given end_index to itself
void populate_array(int source_array[], int target_array[], int start_index, int end_index)
{
    for(int index = start_index; index <= end_index; index++)
    {
        target_array[index - start_index] = source_array[index];
    }
}

//Recursively sorts the subarray of the given array starting from the given start and end index in ascending order
void merge_sort(int values[], int start_index, int end_index)
{
    //If the subarray from start_index to end_index has size at least two
    if(start_index < end_index)
    {
        //Calculate the middle index of the given subarray
        int middle_index((start_index + end_index) / 2);

        //Recursively sort the left half of the given subarray
        merge_sort(values, start_index, middle_index);

        //Recursively sort the right half of the given subarray
        merge_sort(values, middle_index + 1, end_index);

        //Merge the sorted left and right halves of the given subarray in a way such that the entire subarray is sorted
        merge(values, start_index, middle_index, end_index);
    }
}

//Merges the sorted subarrays of the given array which start from the given start_index to the middle_index and from
//middle_index + 1 to the given end_index such that entire subarray from the start_index to the end_index is sorted
void merge(int values[], int start_index, int middle_index, int end_index)
{
    //Arrays to store the left and right subarrays of the given subarray starting from the start_index to the end_index
    int left_subarray[middle_index - start_index + 1] = {0};
    int right_subarray[end_index - middle_index] = {0};

    //Indices to iterate the left and right subarrays respectively
    int left_subarray_index(0);
    int right_subarray_index(0);

    //Copy the appropriate values in the left and right subarrays
    populate_array(values, left_subarray, start_index, middle_index);
    populate_array(values, right_subarray, middle_index + 1, end_index);

    //Index to iterate over the original array which will contain the sorted values
    int original_array_index = start_index;

    //Iterate over both the left and right halves/subarrays of the given original array 
    while((left_subarray_index <= middle_index - start_index) && (right_subarray_index <= end_index - middle_index - 1))
    {
        //If the element in the left subarray is lesser than the one in the right, add the left subarray element to the original array
        if(left_subarray[left_subarray_index] < right_subarray[right_subarray_index])
        {
            values[original_array_index++] = left_subarray[left_subarray_index++];
        }
        //Else add the right subarray element to the original array
        else
        {
            values[original_array_index++] = right_subarray[right_subarray_index++];
        }       
    }

    //If the entire right subarray is traversed, add the remaining elements of the left subarray starting from the left_subarray_index
    //into the original array
    while(left_subarray_index <= middle_index - start_index)
    {
        values[original_array_index++] = left_subarray[left_subarray_index++];
    }

    //If the entire left subarray is traversed, add the remaining elements of the right subarray starting from the right_subarray_index
    //into the original array
    while(right_subarray_index <= end_index - middle_index - 1)
    {
        values[original_array_index++] = right_subarray[right_subarray_index++];
    }
}