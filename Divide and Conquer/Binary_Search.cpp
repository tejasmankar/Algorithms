#include<iostream>

using namespace std;

//Program to implement Binary Search

int array_binary_search(int[], int, int, int);

//Driver function
int main(void)
{
    int number_of_values(0);

    cout << "Enter the number of values to be inserted in the array:\n";
    cin >> number_of_values;

    //Array to store the values
    int values[number_of_values] = {0};

    cout << "Enter the values to be inserted in the array in ascending order:\n";
    for(int index = 0; index < number_of_values; index++)
    {
        cin >> values[index];
    }

    //Variable to store the value to be searched
    int search_value(0);

    cout << "Enter the value to be searched:\n";
    cin >> search_value;

    //Function call to find the index corresponding to the value being searched in the given array
    int index = array_binary_search(values, 0, number_of_values - 1, search_value);

    //If the function performing binary search returns -1, then the value being searched is not present in the given array
    if(index == -1)
    {
        cout << "The given value " << search_value << " is not present in the array\n";
    }
    //Else, the desired value is present in the given array
    else
    {
        cout << "The given value " << search_value << " is present in the array at index " << index << "\n";
    }

    return 0;
}

//Recursively searches for the given search_value in the sub-array of the given values array which starts at start_index and ends at end_index
int array_binary_search(int values[], int start_index, int end_index, int search_value)
{
    //Store the index of the middle element of the given sub-array
    int middle_index((start_index + end_index) / 2);

    //If the middle element is equal to the element to be searched, return its index
    if(values[middle_index] == search_value)
    {
        return middle_index;
    }
    else
    {
        //If the middle element is not equal to the value to be searched and is either the first or last element of the
        //current sub-array, then return -1 as the element to be searched is not present in the current sub-array
        if((middle_index == start_index) && (middle_index == end_index))
        {
            return -1;
        }
        //If the element to be searched is less than the middle element, search it in the left half of the given sub-array
        if(search_value < values[middle_index])
        {
            return array_binary_search(values, start_index, middle_index - 1, search_value);
        }
        //Else, search it in the right half of the given sub-array
        else
        {
            return array_binary_search(values, middle_index + 1, end_index, search_value);
        }
    }
}
