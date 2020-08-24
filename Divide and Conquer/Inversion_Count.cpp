#include<iostream>

using namespace std;

//Program to count the number of inversions in an array using the divide and conquer algorithm
//An array a[] contains an inversion if there are two indices i and j such that i < j and a[i] > a[j]

void populate_array(int[], int[], int, int);
int inversion_count(int[], int, int);
int split_inversion_count(int[], int, int, int);

int main(void)
{
    int number_of_values(0);

    cout << "Enter the number of values to be inserted in the array:\n";
    cin >> number_of_values;

    //Array to store the values
    int values[number_of_values] = {0};

    cout << "Enter the values to be inserted in the array:\n";
    for(int index = 0; index < number_of_values; index++)
    {
        cin >> values[index];
    }

    //Function call to find the number of inversions in the given array
    int number_of_inversions = inversion_count(values, 0, number_of_values - 1);

    cout << "The given array has " << number_of_inversions << " inversion(s)\n";

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

//Recursively sorts the sub-array of the given array starting from the given start and end index in ascending order
//and returns the number of inversions present within that sub-array
int inversion_count(int values[], int start_index, int end_index)
{
    int left_subarray_inversions(0), right_subarray_inversions(0), split_inversions(0);

    //If the sub-array from start_index to end_index has size at least two
    if(start_index < end_index)
    {
        //Calculate the middle index of the given sub-array
        int middle_index((start_index + end_index) / 2);

        //Recursively sort the left half of the given sub-array and store the number of inversions that lie entirely in the left sub-array
        left_subarray_inversions = inversion_count(values, start_index, middle_index);

        //Recursively sort the right half of the given sub-array and store the number of inversions that lie entirely in the right sub-array
        right_subarray_inversions = inversion_count(values, middle_index + 1, end_index);

        //Merge the sorted left and right halves of the given sub-array in a way such that the entire sub-array is sorted
        //Also, calculate the number of inversions whose first/greater element lies in the left sub-array and the second/smaller
        //element lies in the right sub-array
        split_inversions = split_inversion_count(values, start_index, middle_index, end_index);
    }

    //Return the number of inversions present in the sub-array starting from the given start_index to the given end_index
    return left_subarray_inversions + right_subarray_inversions + split_inversions;
}

//Merges the sorted sub-arrays of the given array which start from the given start_index to the middle_index and from
//middle_index + 1 to the given end_index such that entire sub-array from the start_index to the end_index is sorted.
//Also counts the number of split inversions present in the sub-array starting from the given start_index to the given end_index
int split_inversion_count(int values[], int start_index, int middle_index, int end_index)
{
    //Arrays to store the left and right sub-arrays of the given sub-array starting from the start_index to the end_index
    int left_subarray[middle_index - start_index + 1] = {0};
    int right_subarray[end_index - middle_index] = {0};

    //Indices to iterate the left and right sub-arrays respectively
    int left_subarray_index(0);
    int right_subarray_index(0);

    //Copy the appropriate values in the left and right sub-arrays
    populate_array(values, left_subarray, start_index, middle_index);
    populate_array(values, right_subarray, middle_index + 1, end_index);

    //Index to iterate over the original array which will store the sorted values
    int original_array_index(start_index);

    //Variable to store the total number of inversions
    int inversion_counter(0);

    //Iterate over both the left and right halves/sub-arrays of the given original array
    while((left_subarray_index <= middle_index - start_index) && (right_subarray_index <= end_index - middle_index - 1))
    {
        //If the element in the left sub-array is lesser than the one in the right, add the left sub-array element to the original array
        if(left_subarray[left_subarray_index] < right_subarray[right_subarray_index])
        {
            values[original_array_index++] = left_subarray[left_subarray_index++];
        }
        //Else, update the inversion_counter to account for the total number of inversions in the current sub-array which have the
        //element pointed by the right_subarray_index as it's smaller element and add the right sub-array element to the original array.
        //The number of inversions satisfying this condition = length of the left sub-array - the current value of left_subarray_index
        else
        {
            inversion_counter += (middle_index - start_index - left_subarray_index + 1);
            values[original_array_index++] = right_subarray[right_subarray_index++];
        }
    }

    //If the entire right sub-array is traversed, add the remaining elements of the left sub-array starting from the left_subarray_index
    //into the original array
    while(left_subarray_index <= middle_index - start_index)
    {
        values[original_array_index++] = left_subarray[left_subarray_index++];
    }

    //If the entire left sub-array is traversed, add the remaining elements of the right sub-array starting from the right_subarray_index
    //into the original array
    while(right_subarray_index <= end_index - middle_index - 1)
    {
        values[original_array_index++] = right_subarray[right_subarray_index++];
    }

    //Return the value corresponding to the total number of split inversions present in the given sub-array
    return inversion_counter;
}
