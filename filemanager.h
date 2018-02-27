#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>
#include <limits>
#include <fstream>
#include <sstream>

using namespace std;

namespace filemanager{

	// base class for shared handling
	// NOT IDIOTPROOFED
	class file{
	protected:
		// class variables
		std::string filename;
		int dimensionality;
		int count;
		vector<std::string> labels;
		vector < vector<double> > data;

		//checks if a value is a double, otherwise returns NaN
		double isNum(std::string test) const;

		//// might be useful
		//int lineCount(const string filename);

		//functions to take in data
		int dimcount(istream &is);
		vector<std::string> labelstore(const int dim, istream &is);
		vector < vector<double> > datastore(istream &is);

	public:
		// constructor reads file
		file(const std::string name);
		~file(){}

		// getters for variables and sub-variables
		//indices start at 0 where appropriate
		// eg. loops over (i=0; i < dim/count; i++)
		string getfilename() const { return filename; }
		int getdim() const {return dimensionality;}
		vector<std::string> getlabels() const { return labels; }
		std::string getlabel(const int index) const;
		vector<vector<double>> getdata() const { return data; }
		vector<double> getdatapoint(const int index) const;
		double getdatapointcoord(const int pointindex, const int dimensionindex) const;
		int getcount() const { return count; }

	};

}
#endif