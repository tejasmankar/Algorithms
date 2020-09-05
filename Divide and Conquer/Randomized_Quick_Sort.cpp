#include<iostream>
//#include<random>

using namespace std;

//Program to implement the Randomized Quick Sort algorithm

void quick_sort(int[], int, int);
int select_random_pivot(int, int);
int partition_array(int[], int, int, int);

//Driver function
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
    quick_sort(values, 0, number_of_values - 1);

    cout << "The values in sorted(ascending) order are as follows:\n";
    for(int index = 0; index < number_of_values; index++)
    {
        cout << values[index] << " ";
    }
    cout << "\n";

    return 0;
}

//Recursively sorts the sub-array of the given array starting from the given start index to the given end index in ascending order
void quick_sort(int values[], int start_index, int end_index)
{
    //If the size of the sub-array is less than or equal to 1, return from the function
    if(start_index >= end_index)
    {
        return;
    }
    //Else partition the given sub-array around the pivot and recursively sort the left and right sub-arrays of the resultant sub-array
    else
    {
        //Select any random index of the current sub-array and consider the element stored at that index to be the pivot
        int pivot_current_index = select_random_pivot(start_index, end_index);

        //Partition the given sub-array around the pivot and store the index which the selected pivot would have in the sorted version of the input array
        int pivot_final_index = partition_array(values, start_index, end_index, pivot_current_index);

        //Recursively sort the left portion of the given sub-array which spans from start_index to pivot_final_index - 1
        quick_sort(values, start_index, pivot_final_index - 1);

        //Recursively sort the right portion of the given sub-array which spans from pivot_final_index + 1 to end_index
        quick_sort(values, pivot_final_index + 1, end_index);
    }
}

//Returns a random integer between the given start_index and the given end_index, both inclusive
int select_random_pivot(int start_index, int end_index)
{
    return start_index + (rand() % (end_index - start_index + 1));
}

//Partitions the given sub-array spanning from the given start_index to the given end_index around the element stored at the given pivot_current_index(or the pivot)
//All elements in the sub-array smaller than the pivot are placed before it in the sub-array and all the elements greater than the pivot are placed after it
int partition_array(int values[], int start_index, int end_index, int pivot_current_index)
{
    //Copy the pivot element to the first index of the given sub-array by swapping the elements present at the given start_index and the given pivot_current index
    swap(values[start_index], values[pivot_current_index]);

    //Initialize the variable to store the index following the index which the given pivot would have in a sorted version of the input array
    int pivot_final_index = start_index + 1;

    //Loop over all the elements of the given sub-array starting from the second element of the sub-array
    for(int index = start_index + 1; index <= end_index; index++)
    {
        //If the current element is less than the pivot, swap it with the element present at pivot_final_index so that it is present before the pivot in the left sub-array
        if(values[index] < values[start_index])
        {
            swap(values[index], values[pivot_final_index]);

            //Increment the pivot_final_index by 1 as we have discovered an element which is smaller than the given pivot
            pivot_final_index++;
        }
    }

    //Swap the given pivot which is present at the start_index with the element present at pivot_final_index - 1
    //This is because pivot_final_index - 1 is the index which will contain the given pivot in the sorted version of the input array
    //as all the elements greater than the pivot are stored at indices starting from pivot_final_index and all the elements smaller
    //than the pivot are present before pivot_final_index - 1
    swap(values[start_index], values[pivot_final_index - 1]);

    //Return the index which would contain the given pivot in the sorted version of the input array
    return pivot_final_index - 1;
}
