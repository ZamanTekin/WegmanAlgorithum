#ifndef GTALGORITHM_H
#define GTALGORITHM_H

#include <string>
#include <vector>
#include <Eigen/Sparse>
#include <math.h>
#include <Eigen/Dense>

using namespace std;

namespace GT{

	// base class for algorithm
	// NOT IDIOTPROOFED
	class GTalgorithm{
	protected:
		// class variables
		Eigen::MatrixXd initialdata;
		int dim;
		int count;
		int proj;
		Eigen::MatrixXd initialbases;
		int N;
		vector<vector<int>> I;
		vector<double> lambda;
		size_t iteration;
		double stepsize;


		//indexmax&min start at 1
		// generates rotation matrices
		Eigen::SparseMatrix<double> getrot(const size_t comb) const;

		// generates product of rotation matrices at particular step
		// I' = {{I0,J0},.....{I(N-1),J(N-1)}  with I & J the matlab equivalent. ie. I vector of allowed combinations of basis vectors.
		Eigen::SparseMatrix<double>  rotprod() const;

		// generates vector of lambda values
		vector<double> genlambda();

		//generate vector of combinations of bases needed. indexing starts at 1, matches getrot (should probably be changed later)
		vector<vector<int>> genI();

		// generates N
		int genN();

		// generate original basis matrix. loses all 0 values after single rotation so minimal justification to use sparse.
		// synonymous function, lose in class
        //Eigen::MatrixXd genbasis();

		// lose of optimisation pass probably - change filemanager to store as matrix?
		// could lose latter arguments and read from vector
		Eigen::MatrixXd datatomatrix(const vector<vector<double>> data);



		Eigen::MatrixXd rotatebasis() const;
		Eigen::MatrixXd projectdata() const;

	public:
		//GTalgorithm(const vector<vector<double>> data, const double step = exp(-5));
        //GTalgorithm();
		GTalgorithm(const vector<vector<double>> data, const int projs = 2, const double step = exp(-5));
		~GTalgorithm(){}


        Eigen::MatrixXd iterate(const int di = 1);
		Eigen::MatrixXd iterateabsolute(const size_t i);

		void reset();
	};



	// child class with memory of previous few frames?

}
#endif
