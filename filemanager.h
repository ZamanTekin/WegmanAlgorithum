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

    // takes format:
    // variables: DIMENSIONCOUNT
    // labels:
    // LABEL1
    // LABEL2...
    // X1 X2 X3 X4 ....
    // X1 X2 X3 X4 ....

	class file{
	protected:
		// class variables
		std::string filename;
		int dimensionality;
		int count;
		vector<std::string> labels;
        // possibly should be changed to Dense Matrix, though only really slows on starting new data set
		vector < vector<double> > data;

		//checks if a value is a double, otherwise returns NaN
		double isNum(std::string test) const;

		//functions to take in data
        // reads first line for dimensions
		int dimcount(istream &is);
        // reads label list
		vector<std::string> labelstore(const int dim, istream &is);
        // reads body of data
		vector < vector<double> > datastore(istream &is);

	public:
		// constructor reads file
        file(); //calls file dialog - uses QT
		file(const std::string name);
		~file(){}

        // reset file data, should really write assiugnemnt constructo instead
        void newfile();
        void newfile(const std::string name);

		// getters for variables and sub-variables
		//indices start at 0 where appropriate
        // eg. loops over (i=0; i < dim or count; i++)
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
