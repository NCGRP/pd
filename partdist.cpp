#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include "partdist.hpp"
using namespace std;

//to compile: g++ partdist.cpp Lmunkres.cpp BipartiteGraph.cpp Hungarian.cpp -o partdist


/***************GLOBAL VARIABLES*****************/


/***************SUBROUTINES*****************/

//test whether values in command line are integers
bool isnum(const std::string& s)
{
    std::string str1 = s;
    str1.erase(std::remove(str1.begin(), str1.end(), ' '),
               str1.end());
 
	return !str1.empty() && str1.find_first_not_of("0123456789") == std::string::npos;
}

//convert the string defining the partition from the command line into a vector containing the same.
vector<int> strtovec(std::string foo)
{
	//replace the commas with spaces in partitions
	std::replace(foo.begin(), foo.end(), ',', ' ');
	
	//test that all values are integers
	
	if ( isnum(foo) )
	{
		//stream foo into vector 'result' using iterators and back inserter trick
		std::vector<int> result;
		std::istringstream inputStream(foo);
		std::copy(std::istream_iterator<int>(inputStream),
			  std::istream_iterator<int>(),
			  std::back_inserter(result));

		return result;
	}
	//proceed with the conversion of string to vector
	else
	{
		std::vector<int> errvec; //on error from integer test, returns a vector with error codes
		errvec.push_back(-9);
		errvec.push_back(-9);
		return errvec;  
	}
	

}


//change a partition that starts at >1 to a partition that starts at 0
//then make all class values consecutive, i.e. 0,1,2,3, not 0,1,3,4, so that the vector.size()
//is the same as max_element() + 1
vector<int> zeroInd(vector<int> gen)
{
	int correctionfactor;
	vector<int> genZ;

	int amin = ( *min_element(gen.begin(),gen.end()) );
	correctionfactor = (0 - amin);
	
	//zero index the class values 
	for (unsigned int i=0;i<gen.size();i++)
	{
		genZ.push_back( gen[i]+correctionfactor );
	}
	
	//test whether values are consecutive
	vector<int> genZSort = genZ;
	sort( genZSort.begin(), genZSort.end() ); //sort copy of vector genZ

	vector<int> genZUniq = genZSort;
	genZUniq.erase( unique( genZUniq.begin(), genZUniq.end() ), genZUniq.end() ); //reduce to unique values
	
	int amax = ( *max_element(genZUniq.begin(), genZUniq.end()) ); //get the maximum value of the zero indexed vector

	vector<int> translateVec = genZ;  //holds zeroed partition with consecutive values
	if ( (amax + 1) != genZUniq.size() ) //if largest value +1 does not equal vector size, they can't be consecutive
	{
		//clear out genZ values from translateVec
		translateVec.clear();
		for (unsigned int i=0;i<genZ.size();i++)
		{
			for (unsigned int j=0;j<genZUniq.size();j++)
			{
				if (genZ[i] == genZUniq[j]) 
				{
					translateVec.push_back(j);
					break;
				}
			}
		
		}		
	
	}
	
	return translateVec;
}

//sensibly label the partitions, using the restrictive growth function
int rgf(vector<int>& a, vector<int>& b)
{
	int x; //x is current new number
	int z = 0; //z is the replacement value, which will index up
	vector<int> reg; //holds list of values that have been processed so should be ignored
	
	//rgf vector a
	for (int i=0;i<=a.size();++i)
	{
		x = a[i];
		//if x present in reg, ignore, if not relabel
		if ( std::find(reg.begin(), reg.end(), x) != reg.end() ) continue;
		else
		{
			reg.push_back(z); //add the replacement value to the list of values to ignore in the future
			std::replace(a.begin(), a.end(), z, -9);
			std::replace(a.begin(), a.end(), x, z);
			std::replace(a.begin(), a.end(), -9, x);
			++z;
		}
	}
	//rgf vector b
	z = 0;
	vector<int>().swap(reg); //clear register of values to ignore
	for (int i=0;i<=b.size();++i)
	{
		x = b[i];
		//if x present in reg, ignore, if not relabel
		if ( std::find(reg.begin(), reg.end(), x) != reg.end() ) continue;
		else
		{
			reg.push_back(z); //add the replacement value to the list of values to ignore in the future
			std::replace(b.begin(), b.end(), z, -9);
			std::replace(b.begin(), b.end(), x, z);
			std::replace(b.begin(), b.end(), -9, x);
			++z;
		}
	}

	/* concatenate input vectors so that sets in partition have consistent 'names'
	// this might be useful in some circumstances, but is irrelevant to calculation of the pd
	//vector<int>s received are zero indexed but not in order
	//combine into one vector
	vector<int> c = a; 
	c.insert(c.end(), b.begin(), b.end());
	
	//perform replacements
	for (int i=0;i<=c.size();++i)
	{
		x = c[i];
		//if x present in reg, ignore, if not relabel
		if ( std::find(reg.begin(), reg.end(), x) != reg.end() ) continue;
		else
		{
			reg.push_back(z); //add the replacement value to the list of values to ignore in the future
			std::replace(c.begin(), c.end(), z, -9);
			std::replace(c.begin(), c.end(), x, z);
			std::replace(c.begin(), c.end(), -9, x);
			++z;
		}
	}
	
	//extract original vectors to update by reference
	vector<int>::const_iterator ab = c.begin(); //beginning of a in concatenated vector c
	vector<int>::const_iterator ae = c.begin() + a.size(); //end of a
	vector<int>::const_iterator bb = c.begin() + a.size(); //begin of b
	vector<int>::const_iterator be = c.end(); //end of c
	
	vector<int> anew(ab, ae);
	vector<int> bnew(bb, be);
	a = anew;
	b = bnew;
	*/
	
	/*
	cout << "c: ";
	for (unsigned int j=0;j<c.size();++j) cout << c[j];
	cout << "\n";
	cout << "a.size()=" << a.size() << " b.size()=" << b.size() << "\n";
	*/
	
	return 0;
}




//normalize pairwise partition distance matrix using maximum partition distance possible
//uses formulas for maximum "transfer distance" from Charon et al. 2006, Journal of Classification 23:103
double normpd(vector<pair<int, int> > pqmatrix, int n, int cost)
{
	int envmax, genmax;
	double p, q, c, pdmax;
		
	//determine p and q
	envmax = pqmatrix[0].first;
	genmax = pqmatrix[0].second;
	
	if (envmax < genmax)
	{
		p = double(envmax);
		q = double(genmax);
	}
	else
	{
		p = double(genmax);
		q = double(envmax);
	}
	
	//cout << "p=" << p << " q=" << q << " n=" << n << "\n";

	//calculate max possible partition distance, 3 cases
	if (n <= (p + q - 2)) 
	{
		pdmax = ( (2 * n) - p - q); 
		//cout << "case 1, pdmax=" << pdmax << endl;
	}											 
	else if ( ((p + q - 1) <= n) && (n <= ((p - 1) * q)) )
	{
		pdmax = ( n - (ceil ((n + q - p) / q)) );
		//cout << "case 2, pdmax=" << pdmax << endl;
	}
	else if ( ((p - 1) * q) < n  )
	{
		pdmax = ( n - (ceil (n / q)) );
		//cout << "case 3, pdmax=" << pdmax << "n/q=" << n/q <<endl;
	}
	else 
	{
		cout << "Conditions for normalizing the partition distance not met.  Quitting...\n";
		return EXIT_FAILURE;
	}
	
	//cout << "pdmax=" << pdmax << "\n";
	
	//normalize the pd
	c = ( cost / pdmax );
						

	return c;

}

/***************MAIN*****************/
int main( int argc, char* argv[] )
{
	//declare initial variables
	std::string DoNorm = "no";
	std::string BeSensible = "no";
	const char* e;
	const char* g;
	
	//parse the command line for options
	for (int i=0;i<argc;i++)
	{
		if ( string(argv[i]) == "-n" ) 
		{
			DoNorm = "yes"; //key to normalize or not
		}
		if ( string(argv[i]) == "-l" )
		{
			BeSensible = "yes"; //key to relabel or not
		}
	}
	
	//load the partitions
	if ( (DoNorm == "yes" && BeSensible == "yes") )
	{
		e = argv[3]; //partition 1
		g = argv[4]; //partition 2
	}
	else if ( (DoNorm == "yes" && BeSensible == "no") || (DoNorm == "no" && BeSensible == "yes") )
	{
		e = argv[2];
		g = argv[3];
	}
	else
	{
		e = argv[1];
		g = argv[2];
	}
	
	//extract the partitions presented on the command line into vectors
	//convert to string
	std::string ein(e);
	std::string gin(g);

	/*
	cout << "ein>" << ein << "<\n";
	cout << "gin>" << gin << "<\n";
	*/
	
	
	//load the comma-delimited string into a vector<int>
	std::vector<int> gen = strtovec(gin);
	std::vector<int> env = strtovec(ein);
	int nsamp = gen.size(); //nsamp is the number of samples in the partition

	//test whether the vectors contain an error code, indicating that not all input was in integer form
	if ( ( gen[1] == -9 ) || ( env[1] == -9 ) )
	{
		cout << "ERROR: There are non-integer values in the partition definitions. Quitting...\n";
		return EXIT_FAILURE;
	}
	
	//test whether the partitions hold the same number of values. if not, quit, because it
	//makes no sense to calculate pd in this case
	if (gen.size() != env.size())
	{
		cout << "ERROR: Partitions do not hold the same number of elements. Quitting...\n";
		return EXIT_FAILURE;
	}	

	/*
	cout << "env.size()>" << env.size() << "<\n";
	for (unsigned int i=0; i<env.size(); ++i) {cout << env[i] << " : ";}
	cout << "\n";

	cout << "gen.size()>" << gen.size() << "<\n";
	for (unsigned int i=0; i<gen.size(); ++i) {cout << gen[i] << " : ";}
	cout << "\n";
	*/
	
	//zero index partition coding in env, gen.  make values consecutive.
	vector<int> b = zeroInd(gen);
	vector<int> a = zeroInd(env);

	/*
		//print out
		for (unsigned int k=0;k<a.size();k++) cout << a[k];
		cout << " ";
		for (unsigned int k=0;k<b.size();k++) cout << b[k];
		cout << "\n";
	*/

	//apply restrictive growth function to relabel partitions, updates a/b as reference
	if ( BeSensible == "yes" )
	{
		rgf(a,b);
		//print out
		for (unsigned int k=0;k<a.size();k++) cout << a[k];
		cout << " ";
		for (unsigned int k=0;k<b.size();k++) cout << b[k];
		cout << " ";
	}

	/*
	cout << "env  ";
	for (unsigned int k=0;k<a.size();k++) cout << a[k];
	cout << endl << "gen  ";
	for (unsigned int k=0;k<b.size();k++) cout << b[k];
	cout << endl;
	*/
	
	//identify largest cluster number in either partition (this is the maximum number of clusters, minus one, because partition is now zero-indexed)
	int maxP;
	int amax = ( *max_element(a.begin(),a.end()) ); //get maximum value of vectors a, b.  must * (dereference) to get value instead of iterator
	int bmax = ( *max_element(b.begin(),b.end()) ); 
	if (amax >= bmax) maxP = amax;
	else maxP = bmax;
	
	//test whether both partitions have only one cluster defined in them. quit if so, because
	//it doesn't make sense to compute the pd in this case.
	if (maxP == 0)
	{
		cout << "ERROR: Both partitions have only one cluster. Quitting...\n";
		return EXIT_FAILURE;
	}
 
	//convert single string partition coding to multiple string binary coding
	vector<vector<int> > A(maxP+1);
	vector<vector<int> > B(maxP+1);
	
	for (unsigned int j=0; j<(maxP+1); j++)
	{
		for (unsigned int i=0; i<a.size(); i++)
		{
			if (a[i] == j) 
			{
				A[j].push_back(1);
				//cout << "a[i]=" << a[i] << "   1	" << A[j][i] << "\n";
			}
			else 
			{
				A[j].push_back(0);
				//cout << "a[i]=" << a[i] << "   0	" << A[j][i] << "\n";
			}
				
			if (b[i] == j) 
			{
				B[j].push_back(1);
				//cout << "b[i]=" << b[i] << "   0	" << B[j][i] << "\n";
			}
			else 
			{
				B[j].push_back(0);
				//cout << "b[i]=" << b[i] << "   0	" << B[j][i] << "\n";
			}
		}
	}
	
	//get Bbar, the complement (inverse) of B
	vector<vector<int> > Bbar(maxP+1);
	for (unsigned int i=0;i<Bbar.size();i++)
	{
		for (unsigned int j=0;j<a.size();j++)
		{
			if (B[i][j] == 0) Bbar[i].push_back(1);
			else Bbar[i].push_back(0);
		}
	}
	
	//get the intersection of A and Bbar
	//vector<vector<vector<int> > > intx(maxP+1, maxP+1); //3D vector
	vector<vector<vector<int> > > intx(maxP+1, vector<vector<int> >(maxP+1)); //3D vector
	for (unsigned int i=0;i<A.size();i++)
	{
		for (unsigned int j=0;j<Bbar.size();j++)
		{
			for (unsigned int k=0;k<a.size();k++)
			{
				if (A[i][k] == Bbar[j][k] && A[i][k] == 1) 
				{
					intx[i][j].push_back(1);
				}
				else intx[i][j].push_back(0);
			}
		}
	}
	
	//sum the values in the intersection of A and Bbar
	int m;
	vector<vector<int> > sumx(maxP+1);
	for (unsigned int i=0;i<A.size();i++)
	{
		for (unsigned int j=0;j<Bbar.size();j++)
		{
			m=0;
			for (unsigned int k=0;k<a.size();k++)
			{
				if (intx[i][j][k] == 1) m++;	
			}
			sumx[i].push_back(m);
		}
	}

	//Solve the minimum weighted assignment problem using Liu's program which
	//uses the bipartite graph approach to Kuhn-Munkres algorithm
	int cost;
	cost = Lmunkres(sumx);
	
	if ( DoNorm == "yes" )
	{
		//normalize the partition distance to the maximum value possible given the partitions
		//add paired max number of subsets to pqmatrix 
		vector<pair<int, int> > pqmatrix;
		pqmatrix.push_back( make_pair(amax+1, bmax+1) );
		//calculate the max number of partitions, then normalize the pd
		double npd = normpd(pqmatrix, nsamp, cost);
	
		cout << npd << "\n";
	}
	else
	{
		cout << cost << "\n";
	}


	return 0;
}
