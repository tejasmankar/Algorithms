#include<iostream>
#include<algorithm>

using namespace std;

//Dynamic Programming algorithm to solve the 0 - 1 Knapsack Problem

 int knapsack_problem_solver(int, int, int[], int[]);

//Driver function
int main(void)
{
    int number_of_items(0);

    cout << "Enter the number of items:\n";
    cin >> number_of_items;

    int value[number_of_items] = {0}, weight[number_of_items] = {0};

    cout << "Enter the values of the items:\n";
    for(int index = 0; index < number_of_items; index++)
    {
        cin >> value[index];
    }

    cout << "Enter the weights of the items:\n";
    for(int index = 0; index < number_of_items; index++)
    {
        cin >> weight[index];
    }

    int knapsack_weight(0);
    cout << "Enter the total weight that the knapsack can hold:\n";
    cin >> knapsack_weight;

    //Function call to the knapsack problem solver method
    cout << "The maximum sum of the values of the optimal choice of items that can be stored in the knapsack is " << knapsack_problem_solver(number_of_items, knapsack_weight, value, weight) << "\n";

    return 0;
}

//Outputs the optimum value(maximum sum of the given values) of all the given items that can be inserted in the knapsack which can handle at most the given weight
 int knapsack_problem_solver(int number_of_items, int knapsack_weight, int value[], int weight[])
 {
     //2D array to store the optimum values of the intermediate subproblems such that memoization_table[x][y] refers
     //to the optimum value of the first x items having total weight at most y
     int memoization_table[number_of_items + 1][knapsack_weight + 1];

     for(int item = 0; item < number_of_items + 1; item++)
     {
         for(int candidate_weight = 0; candidate_weight < knapsack_weight + 1; candidate_weight++)
         {
             //The optimal value is zero if either the number of items being considered or the total weight of the
             //items being considered is equal to zero
             if((item == 0) || (candidate_weight == 0))
             {
                 memoization_table[item][candidate_weight] = 0;
             }
             //Else the optimal value for the first item number of items is the same as the optimal value for the first item - 1
             //number of items if the weight of the current item is more than that of the current maximum weight / candidate_weight being considered
             else if(weight[item - 1] > candidate_weight)
             {
                 memoization_table[item][candidate_weight] = memoization_table[item - 1][candidate_weight];
             }
             //Else the optimal value is the maximum of the optimal value for the first item - 1 items having their total weight as the current candidate_weight and the
             //optimal value for the first item - 1 items having their total weight equal to candidate_weight - weight[item] + the value of the current item being considered
             //Here, we are comparing the maximum value among the following two cases - when the current item is not a part of the optimal solution vs when the current item
             //is a part of the optimal solution
             else
             {
                 memoization_table[item][candidate_weight] = max(memoization_table[item - 1][candidate_weight], memoization_table[item - 1][candidate_weight - weight[item]] + value[item - 1]);
             }
         }
     }

     //Return the optimal value for the complete subproblem i.e. optimal value for the first number_of_items
     //items having total weight less than or equal to knapsack_weight
     return memoization_table[number_of_items][knapsack_weight];
 }
