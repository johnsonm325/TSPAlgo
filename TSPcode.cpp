/************************************
* CS 325
* Final Project
* Joshua Louis Ceciliani - the man
* Jeremy Einhorn - the myth
* Michael Johnson - the legend
************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <climits>
#include <math.h>

using std::cout;
using std::cin;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::ios;
using std::string;
using std::vector;

struct Cities
{
	int city;
	int x;
	int y;
};

float distance(float, float, float, float);
void TSPalgo(int**, int, int);
int minPath(int**, int**, int**, int, int, int, int);
void getPath(int**, int, int, int);

/*****************************************************
** Calculate distance for adjacency matrix
*****************************************************/

float distance(float x1, float x2, float y1, float y2)
{
    float x = x1 - x2;
    float y = y1 - y2;
    float xx = x*x;
    float yy = y*y;
    float value = sqrt(xx + yy);
    return round(value);
}

/*****************************************************
**
*****************************************************/

void TSPalgo(int** adjMat, int numCities, int citySet)
{
    // citSet is representation of the set of cities via a masked binary number
    // full set of cites = 1 1 1 1 0 = 30 (since the first number is the starting point
    // 4th city is visited (out of 5) = 1 0 1 1 0 = 22

    int **memoMinDist = new int*[numCities]; // memoized shortest path graph
	for (int i = 0; i < numCities; i++)
		memoMinDist[i] = new int[citySet];

    int **path = new int*[numCities]; // uses bit masked number to determine optimal path
	for (int i = 0; i < numCities; i++)
		path[i] = new int[citySet];


    for (int i = 0; i < numCities; i++)
    {
        for (int j = 0; j < citySet; j++)
        {
            // set to -1 so we can compare if == -1
            // set the first line from adjacency matrix
            if (j == 0)
            {
                memoMinDist[i][j] = adjMat[i][j];
            }

            else
            {
                memoMinDist[i][j] = -1;
            }
            path[i][j] = -1;
        }
    }

	int result = minPath(memoMinDist, path, adjMat, 0, citySet - 2, numCities, citySet);

	cout << "Minimum cost: " << result << endl;

	getPath(path, 0, citySet - 2, citySet);

	for (int i = 0; i < numCities; i++)
	{
		delete[] memoMinDist[i];
	}
	delete[] memoMinDist;

	for (int i = 0; i < numCities; i++)
	{
		delete[] path[i];
	}
	delete[] path;
}

int minPath(int **memoPath, int **optPath, int **adjMat, int currCity, int remainingSet, int numCit, int setCities)
{
    // currCity = current city we are measuring distance from
    // remainingSet = the set of cities that haven't been visited
    // numCit = the number of cities (remains constant)
    // setCities = the binary representation of the cities that have been visited thus far

    int mask,
        citiesMasked,
		nextCity,
		res = std::numeric_limits<int>::max();

    if (memoPath[currCity][remainingSet] != -1)
    {
        return memoPath[currCity][remainingSet];
    }

    for (int i = 0; i < numCit; i++)
    {
        mask = (setCities - 1) - (1 << i); // create mask for computing cost to visit city i from current city
        // 1 << i = shifts the bit over that many spaces
        // 00001 << 3 = 01000
        // 1 0 1 1 0 (setCities) - 1 =
        //   1 0 1 1 0
        // - 0 0 0 0 1
        // = 1 0 1 0 1

        // 10101 - 00100

        // use & operator to create the set showing the city that would be visited next
        //   1 1 1 1 0
        // + 1 0 1 1 0
        // = 1 0 1 1 0 this is an example of the
        citiesMasked = remainingSet&mask;

		if (citiesMasked != remainingSet)
		{
			int temp = adjMat[currCity][i] + minPath(memoPath, optPath, adjMat, currCity, citiesMasked, numCit, setCities); //FIX: Potential correction needed on values
			if (temp < res)
			{
				res = temp;
				optPath[currCity][remainingSet] = i;
				nextCity = i;
			}
		}
    }
	currCity = nextCity;

	return memoPath[currCity][remainingSet] = res;
} 

/*****************************************************
**
*****************************************************/
void getPath(int **optPath, int currCity, int remainingSet, int setCities)
{
	if (optPath[currCity][remainingSet] == -1) //reached the null set 
	{
		return;
	}

	int temp = optPath[currCity][remainingSet];
	int mask = (setCities - 1) - (1 << temp);
	int citiesMasked = remainingSet&mask;

	cout << temp << "  ";

	getPath(optPath, temp, citiesMasked, setCities);

}


int main()
{
	ifstream		 inputFile;			//create input file object
	string			 fileName,
                     str;
    int              count = 0;
	vector<Cities>   citiesList;

	cout << "What file do you want to read from?" << endl;
	getline(cin, fileName);

	//open the file
	cout << "Opening input file." << endl;
	inputFile.open(fileName);

	//if it doesn't open, close the program
	if (!inputFile)
	{
		cout << "File open error!" << endl;
		cin.get();
		return 0;
	}

	while (getline(inputFile, str, '\n'))
	{
        citiesList.push_back(Cities());
        istringstream num(str);
        num >> citiesList[count].city;
        num >> citiesList[count].x;
        num >> citiesList[count].y;
        count++;
    }

	int **adjMatrix = new int*[citiesList.size()];
	for (size_t i = 0; i < citiesList.size(); i++)
		adjMatrix[i] = new int[citiesList.size()];

	for (size_t i = 0; i < citiesList.size(); i++)
	{
        for (size_t j = 0; j < citiesList.size(); j++)
        {
            adjMatrix[i][j] = distance(citiesList[i].x, citiesList[j].x, citiesList[i].y, citiesList[j].y); // FIX: 'argument': conversion from 'int' to 'float', possible loss of data
        }
	}


	for (size_t i = 0; i < citiesList.size(); i++)
	{
        for (size_t j = 0; j < citiesList.size(); j++)
        {
            cout << adjMatrix[i][j] << " ";
        }
        cout << endl;
	}
	cout << endl;

	//close the input file
	inputFile.close();

	cout << "File closed." << endl;

	int sizePow = (int) pow(2, count);

	TSPalgo(adjMatrix, count, sizePow);

	cin.get();
    cin.get();

	for (int i = 0; i < count; i++)
	{
		delete[] adjMatrix[i];
	}
	delete[] adjMatrix;
	

	return 0;
}
