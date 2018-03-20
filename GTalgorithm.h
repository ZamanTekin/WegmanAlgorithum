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
        // stores initial data and metadata
		Eigen::MatrixXd initialdata;
		int dim;
		int count;
        Eigen::MatrixXd initialbases;
        // number of dimensions to allow projection into - omnly worth setting below max if speed gain really needed?
		int proj;
        // N = number of rotation planes for the current projection
		int N;
        // I = allowed combinations of basis vectors
		vector<vector<int>> I;
        // lambda = N linearly independent numbers
		vector<double> lambda;
        // iteration count and stepsize
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

		// lose of optimisation pass probably - change filemanager to store as matrix?
		// could lose latter arguments and read from vector
		Eigen::MatrixXd datatomatrix(const vector<vector<double>> data);


        // very veyr basic functions, can probably be built into iterate
		Eigen::MatrixXd rotatebasis() const;
		Eigen::MatrixXd projectdata() const;

	public:
        // defaults to only 2 projection dimensions
		GTalgorithm(const vector<vector<double>> data, const int projs = 2, const double step = exp(-5));
		~GTalgorithm(){}

        // returns next iteration, with changeable increment
        Eigen::MatrixXd iterate(const int di = 1);
        // returns specified iteration, without losing current place
		Eigen::MatrixXd iterateabsolute(const size_t i);

        // resets algorithm, or sets to specific iteration
        Eigen::MatrixXd set(const int i=0);
	};



	// child class with memory of previous few frames?

}
#endif
