#include "filemanager.h"
#include "GTalgorithm.h"

//#include <Eigen/Sparse>
//#include <math.h>
//#include <Eigen/Dense>

#include <iostream>

#include <ctime>

using namespace std;
using namespace filemanager;
using namespace GT;

template<typename T>
void printvec(vector<T> v){
	cout << v[0];
	for (size_t i = 1; i < v.size(); i++){
		cout << ',' << v[i];
	}
	cout << endl;
}
template<typename T>
void print2d(vector<vector<T>> v){
	for (size_t i = 0; i < v.size(); i++){
		printvec(v[i]);
	}
}

int main(){
	//tests


	//speedcheck
	file pollen("F:\\MPhys2\\WegmanTour\\WegmanTour\\CrystalDemoData\\pollen.txt");

	GTalgorithm tour(pollen.getdata(),5);
	vector<clock_t> times;

	for (size_t i = 0; i < 1000; i++){
		clock_t start = clock();
		tour.iterate();
		clock_t end = clock();
		clock_t dif = end - start;
		times.push_back(dif);
		cout << i << ")  " << dif << endl;
	}

	clock_t sum = 0;
	for (auto it = times.begin(); it < times.end(); it++){
		sum += *(it);
	}

	cout << "Mean = " << (sum / times.size()) << endl;
	cout << "Max = " << *max_element(times.begin(), times.end()) << endl;
	cout << "Min = " << *min_element(times.begin(), times.end()) << endl;







	//accuracy check pt1

	//ofstream write("F:\\MPhys2\\c++\\compare\\c++.txt");

	//if (write.is_open())
	//{
	//	for (size_t i = 0; i < 50; i++){
	//		write << tour.iterate() << endl;
	//	}
	//	write.close();
	//}
	//else cout << "Unable to open file";


	//accuracy check pt 2

	//ifstream c("F:\\MPhys2\\c++\\compare\\c++.txt");
	//ifstream m("F:\\MPhys2\\c++\\compare\\matlab.txt");

	//vector<double> cnum;
	//vector<double> mnum;

	//double num;
	//while (c >> num) {
	//	cnum.push_back(num);
	//}
	//while (m >> num) {
	//	mnum.push_back(num);
	//}

	//c.close();
	//m.close();


	//vector<double> difs;
	//for (size_t i = 0; i < cnum.size(); i++){
	//	difs.push_back(cnum[i] - mnum[i]);
	//}

	//double sum = 0;
	//for (auto it = difs.begin(); it < difs.end(); it++){
	//	sum += *(it);
	//}

	//cout << "Mean = " << (sum / difs.size()) << endl;
	//cout << "Max = " << *max_element(difs.begin(), difs.end()) << endl;
	//cout << "Min = " << *min_element(difs.begin(), difs.end()) << endl;

	return 0;
}