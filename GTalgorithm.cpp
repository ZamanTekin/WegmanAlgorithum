#include "GTalgorithm.h"
//#include <iostream>

using namespace std;
using namespace GT;

////for testing
//template<typename T>
//void printvec(vector<T> v){
//	cout << v[0];
//	for (size_t i = 1; i < v.size(); i++){
//		cout << ',' << v[i];
//	}
//	cout << endl;
//}
//template<typename T>
//void print2d(vector<vector<T>> v){
//	for (size_t i = 0; i < v.size(); i++){
//		printvec(v[i]);
//	}
//}








//indexmax&min start at 1
// generates rotation matrices
Eigen::SparseMatrix<double> GTalgorithm::getrot(const size_t comb) const{
	double angle = iteration*lambda[comb] * stepsize;
	int indexmin = I[comb][0] - 1;
	int indexmax = I[comb][1] - 1;
	Eigen::SparseMatrix<double> M(dim, dim);
	std::vector<Eigen::Triplet<double>> tripletList;
	tripletList.reserve(dim + 2);

	for (int i = 0; i < dim; i++)
	{
		if (i != indexmin && i != indexmax){
			tripletList.push_back(Eigen::Triplet<double>(i, i, 1));
		}
	}
	tripletList.push_back(Eigen::Triplet<double>(indexmin, indexmin, cos(angle)));
	tripletList.push_back(Eigen::Triplet<double>(indexmax, indexmax, cos(angle)));
	tripletList.push_back(Eigen::Triplet<double>(indexmin, indexmax, -sin(angle)));
	tripletList.push_back(Eigen::Triplet<double>(indexmax, indexmin, sin(angle)));


	M.setFromTriplets(tripletList.begin(), tripletList.end());

	return M;
}

// generates product of rotation matrices at particular step
// I' = {{I0,J0},.....{I(N-1),J(N-1)}  with I & J the matlab equivalent. ie. I vector of allowed combinations of basis vectors.
Eigen::SparseMatrix<double>  GTalgorithm::rotprod() const{
	Eigen::SparseMatrix<double> Q = getrot(0);
	for (int i = 1; i < N; i++){
		Eigen::SparseMatrix<double> R = getrot(i);
		Q = Q*R;
	}
	return Q;
}


// generates vector of lambda values
vector<double> GTalgorithm::genlambda(){
	vector<double> lambda;
	for (int i = 1; i <= N; i++){
		lambda.push_back(fmod(exp(i), 1));
	}
	return lambda;
}

//generate vector of combinations of bases needed. indexing starts at 1, matches getrot (should probably be changed later)
vector<vector<int>> GTalgorithm::genI(){
	vector<int> comb(2, 0);
	vector<vector<int>> I;
	for (int i = 1; i <= proj; i++){
		for (int j = i + 1; j <= dim; j++){
			comb[0] = i;
			comb[1] = j;
			I.push_back(comb);
		}
	}
	return I;
}

// generates N
int GTalgorithm::genN(){
	int N = 0;
	for (int i = 1; i <= proj; i++){
		N += (dim - i);
	}
	return N;
}


// lose of optimisation pass probably - change filemanager to store as matrix?
// could lose latter arguments and read from vector
Eigen::MatrixXd GTalgorithm::datatomatrix(const vector<vector<double>> data){
	Eigen::MatrixXd M(count, dim);
	for (int i = 0; i < count; i++){
		for (int j = 0; j < dim; j++){
			M(i, j) = data[i][j];
		}
	}
	return M;
}





//GTalgorithm::GTalgorithm(const vector<vector<double>> data, const double step){
//	iteration = 0;
//	stepsize = step;
//	proj = 2;
//	count = data.size();
//	dim = data[0].size();
//	initialdata = datatomatrix(data);
//	initialbases = Eigen::MatrixXd::Identity(dim, proj);
//	N = genN();
//	lambda = genlambda();
//	I = genI();
//
//}

/*GTalgorithm::GTalgorithm(){
    iteration = 0;
    stepsize = exp(-5);
    proj = 2;
    vector<vector<double>> data {{0.0}};
    count = data.size();
    dim = data[0].size();
    initialdata = datatomatrix(data);
    initialbases = Eigen::MatrixXd::Identity(dim, proj);
    N = genN();
    lambda = genlambda();
    I = genI();
}*/

GTalgorithm::GTalgorithm(const vector<vector<double>> data, const int projs, const double step){
	iteration = 0;
	stepsize = step;
	proj = projs;
	count = data.size();
	dim = data[0].size();
	initialdata = datatomatrix(data);
	initialbases = Eigen::MatrixXd::Identity(dim, proj);
	N = genN();
	lambda = genlambda();
	I = genI();
}



Eigen::MatrixXd GTalgorithm::rotatebasis() const{
	return rotprod()*initialbases;
}
Eigen::MatrixXd GTalgorithm::projectdata() const{
	return initialdata*rotatebasis();
}



Eigen::MatrixXd GTalgorithm::iterate(const int di){
	iteration += di;
	return projectdata();
}
//hacky as hell
Eigen::MatrixXd GTalgorithm::iterateabsolute(const size_t i){
	size_t tempit = iteration;
	iteration = i;
	Eigen::MatrixXd newdata = projectdata();
	iteration = tempit;
	return newdata;
}

void GTalgorithm::reset(){
	iteration = 0;
}
