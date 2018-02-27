#include "filemanager.h"

using namespace std;
using namespace filemanager;

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

//// might be useful - uses iostream
//int file::lineCount(const string filename){
//	// returns an error on failure to open
//	ifstream myfile(filename);
//	if (!myfile.good()) {
//		cout << "file failed to open";
//		return -1;
//	}
//
//	int linecount(0);
//	string line;
//
//	// iterates over file until end reached, counting only lines that can be converted to doubles.
//	myfile >> line;  // prime read to prevent eof duplicating last line
//	while (!myfile.eof()){
//		if (isNum(line) != 0){
//			linecount++;
//		}
//		myfile >> line;
//	}
//	myfile.close();
//	return linecount;
//}

int file::dimcount(istream &is){
	string line;
	getline(is, line);
	stringstream input(line);
	string temp;
	int dim;
	input >> temp >> dim;
	return dim;
}

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

//index runs from 0
std::string file::getlabel(const int index) const{
	if (index < dimensionality){
		return labels[index];
	}
	else{
		return "Invalid index";
	}
}

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
double file::getdatapointcoord(const int pointindex, const int dimensionindex) const{
	if ((pointindex < count) && (dimensionindex < dimensionality)){
		return data[pointindex][dimensionindex];
	}
	else{
		return std::numeric_limits<double>::quiet_NaN();
	}
}