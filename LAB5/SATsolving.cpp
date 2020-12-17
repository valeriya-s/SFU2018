#include <iostream>
#include <iomanip>
#include <math.h>
#include <fstream>
#include <string>

using namespace std;

int main()
{
	const int MAX_rows_cols = 32;
	ifstream inputFile;
	ofstream outputFile;
	char dataInput[100]={'\0'};
	char dataOutput[100]={'\0'};
	
	int variables;
	int clauses;
	int maxsize = 0;
	string f1;
	string f2;
	int tempVector[MAX_rows_cols] = {'\0'};
	int SAT_vector[MAX_rows_cols*MAX_rows_cols];
	int SAT_matrix[MAX_rows_cols][MAX_rows_cols];
	int solution_vector[MAX_rows_cols*MAX_rows_cols] = {'\0'};
	int temp=0;
	int temp4tempv=0;
	bool solutions[MAX_rows_cols*MAX_rows_cols] = {'\0'};
	int test = 0;
	bool test_solutions = 0;
	bool included = 0;
	bool final_test = 1;

	cout << "Enter the name of the input file: ";
	cin >> dataInput;
	inputFile.open(dataInput);

	if ( inputFile.fail() )
	{
		cerr << "ERROR: input file not opened correctly";
		return 1;
	}
	
	inputFile >> f1;
	inputFile >> f2;

	inputFile >> variables;
	inputFile >> clauses;

	for (int r=0; r<MAX_rows_cols*MAX_rows_cols; r++)
	{
		if ( !(inputFile >> SAT_vector[r]) )
			break;
		//cout << SAT_vector[r] << " ";
		maxsize++;
	}

	// in the next part im going to seperate the clauses into rows

	for (int r=0; r<maxsize; r++)
	{
		if (SAT_vector[r] != 0)
		{
			temp++;
		}
		else if (SAT_vector[r] == 0)
		{
			tempVector[temp4tempv]=temp;
			//cout << tempVector[temp4tempv];
			temp4tempv++;
			temp=0;
		}
	}
	
	for (int r=0; r<clauses; r++)
	{
		for (int c=0; c<tempVector[r]; c++)
		{
			SAT_matrix[r][c] = SAT_vector[temp];
			temp++;
			//cout << SAT_matrix[r][c] << " ";
		}
		temp++;
		//cout << endl;
	}

	// a few things to keep in mind:
	//		tempVector[n][0] == # variables in nth row

	temp=0;
	
	// what i wanna do here is put in the variables into my solution but i have to make sure its not already in there
	// i think i can do that by taking the variable and then checking inside the vector and then if its not in there putting it in.

	for (int i=0; i<variables+temp; i++)
	{
		int temp_col=0;
		for (int r=i; r<clauses; r++)
		{
			while (temp_col < tempVector[r])
			{
				for (int j=0; j<variables+temp; j++)
				{
					//if (SAT_matrix[r][temp_col] == solution_vector[j] || SAT_matrix[r][temp_col] == -1*solution_vector[j])
					if (SAT_matrix[r][temp_col] == solution_vector[j] )
					{
						included = true;
						temp++;
					}
					if (SAT_matrix[r][temp_col] == -1*solution_vector[j])
					{
						test_solutions=1;
					}
				}
				// at this point if there is a match i think u should go to the next variable in the clause
				if (test_solutions)
				{
					temp_col = temp_col + 1;
				}
				else break;
				test_solutions=0;
			}
			if (temp_col != tempVector[r] && included == false)
			{
				solution_vector[i] = SAT_matrix[r][temp_col];
				break;
			}
		}
		//if (solution_vector[i] == 0)
		//{
			//temp++;
		//}
		included = false;
	}

	for (int r=0; r<clauses; r++)
	{
		for (int c=0; c<tempVector[r]; c++)
		{
			for (int test=0; test<MAX_rows_cols; test++)
			{
				if (SAT_matrix[r][c] == solution_vector[test])
				{
					solutions[r] = true;
					//cout << SAT_matrix[r][c] << " and "<< solution_vector[test] << " ";
				}
			}
		}
		//cout << solutions[r] << endl;
		//cout << endl << r << endl;
	}

	for (int i=0; i<clauses; i++)
	{
		//cout << solutions[i];
		if (solutions[i] == false)
		{
			cout << "Case is unsolvable, all clauses can't be satisfied";
			final_test = false;
			break;
		}
	}

	if (final_test == true)
	{
		cout << "The solution is: " << endl;
		for (int i=0; i<variables+temp; i++)
		{
			if (solution_vector[i] != 0)
			{
				cout << solution_vector[i] << endl;
			}
		}
	}

	inputFile.close();

	return 0;

}
