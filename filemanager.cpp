#include "filemanager.h"
#include <QFileDialog>

using namespace std;
using namespace filemanager;

// returns double or NaN
double file::isNum(const std::string test) const{
	double conversion;
	try{
		conversion = stod(test);
	}
	catch (const invalid_argument){
		return std::numeric_limits<double>::quiet_NaN();
	}
	return conversion;
}

// returns number of dimensions from dimensionality line
int file::dimcount(istream &is){
	string line;
	getline(is, line);
	stringstream input(line);
	string temp;
	int dim;
	input >> temp >> dim;
	return dim;
}

// reads labels from list of labels
vector<std::string> file::labelstore(const int dim, istream &is){
	vector<std::string> labels;
	string line;
	getline(is, line);
	for (int i = 0; i < dim; i++){
		getline(is, line);
		labels.push_back(line);
	}
	return labels;
}

// reads data
// copies whitespacve delimited data, ignores empty lines
vector<vector<double>> file::datastore(istream &is){
	vector<vector<double>> data;
	string line;
	while (getline(is,line)){
		if (line.empty() == false){
			vector<double> linedata;
			stringstream linestream(line);
			string temp;
			while (getline(linestream, temp, ' ')){
				if (temp.empty() == false){
					linedata.push_back(isNum(temp));
				}
			}
			data.push_back(linedata);
		}
	}

	return data;
}

//default constructor calls file dialogue
// creates dummy data on failure to open file
file::file(){
    string name = (QFileDialog::getOpenFileName(NULL,"open file","/home","(*.txt *.csv)")).toStdString();
    ifstream stream;

    stream.open(name);

    if (stream.is_open()){
        filename = name;

        dimensionality = dimcount(stream);
        labels = labelstore(dimensionality, stream);
        data = datastore(stream);
        count = data.size();
    }
    else{
        filename = "INVALID";
        dimensionality = 1;
        vector<string> emptylabel = { "NULL" };
        labels = emptylabel;
        count = 1;
        vector<vector<double>> emptydata = { { std::numeric_limits<double>::quiet_NaN() } };
        data = emptydata;
    }

    stream.close();
}

// opens file @ name
file::file(const std::string name){
	ifstream stream;

	stream.open(name);

	if (stream.is_open()){
		filename = name;

		dimensionality = dimcount(stream);
		labels = labelstore(dimensionality, stream);
		data = datastore(stream);
		count = data.size();
	}
	else{
		filename = "INVALID";
		//perror("Error: ");
		dimensionality = 1;
		vector<string> emptylabel = { "NULL" };
		labels = emptylabel;
		count = 1;
		vector<vector<double>> emptydata = { { std::numeric_limits<double>::quiet_NaN() } };
		data = emptydata;
	}

	stream.close();
}

// return specific label
std::string file::getlabel(const int index) const{
	if (index < dimensionality){
		return labels[index];
	}
	else{
		return "Invalid index";
	}
}

// return specific datapoint, or dummy point if out of range
vector<double> file::getdatapoint(const int index) const{
	if (index < count){
		return data[index];
	}
	else{
		vector<double> fakepoint;
		for (int i = 0; i < dimensionality; i++){
			fakepoint.push_back(std::numeric_limits<double>::quiet_NaN());
		}
		return fakepoint;
	}
}

// return specific coordinate of specirfic datapoint, or NaN on fail
double file::getdatapointcoord(const int pointindex, const int dimensionindex) const{
	if ((pointindex < count) && (dimensionindex < dimensionality)){
		return data[pointindex][dimensionindex];
	}
	else{
		return std::numeric_limits<double>::quiet_NaN();
	}
}
