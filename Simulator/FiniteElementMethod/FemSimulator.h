#pragma once
#ifndef FINITE_ELEMENT_METHOD_SIMULATOR_H
#define FINITE_ELEMENT_METHOD_SIMULATOR_H

#include "Simulator\BaseSimulator.h"
#include "Commom\FileIO.h"
#include "MatrixCore.h"
#include "FemModel.h"
#include "Commom\SparseMatrixTopology.h"
#include "ImplicitNewMarkSolverConfig.h"
#include "Commom\PolynomialSolver.h"
#include "Commom\AutoFlipSVD.h"

namespace FiniteElementMethod
{
	class FemSimulator : public BaseSimulator
	{
	public:
		FemSimulator(std::string simName = "fem_simulator", RunPlatform runPlatform = RunPlatform::CPU):
			BaseSimulator(simName, runPlatform)
		{
			solverConfig = new ImplicitNewMarkSolverConfig(_timeStep);
		}
		~FemSimulator() { deleteAllPointer(); }

		FemModel* getModel(const int id) { return dynamic_cast<FemModel*> (models[id]); }

		ImplicitNewMarkSolverConfig* getSolver() { return solverConfig; }
		//
		virtual bool addModelFromConfigFile(const std::string filename, TiXmlElement* item);

		virtual bool addStaticModelFromConfigFile(const std::string filename, TiXmlElement* item);
		//
		void nullAllPointer();

		void deleteAllPointer();

		virtual void saveFile();
		virtual void saveSimulator();
		virtual void initialization();
		virtual void run(int frame = 0);
		virtual void postRun();
		virtual void reset();

		virtual void handleSurfacePointsSelectedEvent();
		virtual void handleTetPointsSelectedEvent();

		virtual void addTetPointsConstraint();
		void createMapByConstraints();
		void preFillSysStiffnessMatrix();

		virtual void handleTetPointsConstraint();

		virtual void computeGravityForce();
		virtual void computeExternalForce();
		virtual void computeInternalForce();
		virtual void computeStiffnessMatrix();
		virtual void computePartialPF(VectorX& displacement);

		virtual void computeElasticsEnergy(qeal& energy, VectorX& X, VectorX& V);

		virtual void computeElasticsEnergy(qeal& energy, SparseMatrix& A, VectorX& rhs, VectorX& X);

		virtual void computeNewtonSolverEnergy(qeal& E, qeal& Eprev, SparseMatrix& A, VectorX& rhs, VectorX& x);

		virtual void computeNewtonSolverEnergy(qeal& E, qeal& Eprev, MatrixX& A, VectorX& rhs, VectorX& x, qeal linearSearch = 1.0);

		virtual void computeNewtonSolverEnergy(qeal& E, MatrixX& A, VectorX& rhs, VectorX& x);

		virtual qeal computeNeoHookeanEnergy(int eid, BaseTetMeshHandle* tet, Matrix3& F);
		virtual qeal computeNeoHookeanEnergy(VectorX& displacement);

		virtual void computeElasticsEnergy(VectorX& X, VectorX& Xtilde, SparseMatrix& Mass, qeal& h, qeal& E);

		virtual void stableNeoHookean(int eid, BaseTetMeshHandle* tet, Matrix3& F, Matrix3& FPK, MatrixX&PF);

		virtual void commonNeoHookean(int eid, BaseTetMeshHandle* tet, Matrix3& F, Matrix3& FPK, MatrixX&PF);

		virtual void computeCommonNeoHookeanPartialPF(VectorX& displacement);


		virtual void doTime(int frame = 0);

		virtual void doTime2(int frame = 0);

		virtual void doComomNHTime(int frame = 0);

		virtual void NewtonLineSearch(qeal& E, qeal& Eprev, qeal& alpha, SparseMatrix& A, VectorX& rhs, VectorX& x);

		virtual void setTimeStep(qeal t) { _timeStep = t; solverConfig->updateAlphas(_timeStep); }

		virtual void saveFrameStatus(int frame, std::ofstream& fout);

		virtual void recoverFrameStatus(int frame, std::ifstream& fin);


		//

		virtual void computeCommonNHElasticsEnergy(VectorX& X, VectorX& Xtilde, SparseMatrix& Mass, qeal& h, qeal& E);

		virtual qeal computeCommonNeoHookeanEnergy(VectorX& displacement);

		virtual qeal computeCommonNeoHookeanEnergy(int eid, BaseTetMeshHandle* tet, Matrix3& F);

		void compute_dP_div_dF(const AutoFlipSVD<Matrix3>& svd,
			qeal miu, qeal lambda,
			MatrixX& dP_div_dF, bool projectSPD = true);

		void compute_dE_div_dsigma(Vector3& singularValues, qeal miu, qeal lambda, Vector3& dE_div_dsigma);

		void compute_d2E_div_dsigma2(Vector3& singularValues, qeal miu, qeal lambda, Matrix3& d2E_div_dsigma2);

		void compute_BLeftCoef(Vector3& singularValues, qeal miu, qeal lambda, VectorX& BLeftCoef);

		void compute_dE_div_dF(Matrix3& F, const AutoFlipSVD<Matrix3>& svd, qeal miu, qeal lambda, Matrix3& dE_div_dF); // FPK

		void computeElementCNHEnergy(Vector3& singularValues, qeal miu, qeal lambda, qeal& E);


		void computeCofactorMtr(const Matrix3& F, Matrix3& A);

		void makePD3d(Matrix3& symMtr);
		void makePD2d(Matrix2& symMtr);


		qeal getEnergy(VectorX & xn, VectorX & x_tilde);
	protected:

		ImplicitNewMarkSolverConfig* solverConfig;
		Eigen::SimplicialLLT<SparseMatrix> _sparseLDLT;

		int _constrainedTetPointsNum;
		std::set<int> _constrainedTetPoints;

		SparseMatrix _sysMatrix;
		VectorX _sysDir;

		VectorX _sysRhs;
		VectorX _sysX;
		VectorX _sysXtilde;

		VectorX _sysXn;
		VectorX _sysVn;
		VectorX _sysAccn;

		SparseMatrix _sysSubMatrix;
		VectorX _sysSubRhs;
		VectorX _sysSubXtilde;

		SparseMatrix _sysMassMatrix;		
		SparseMatrix _sysInverseMassMatrix;
		SparseMatrix _sysStiffnessMatrix;
		SparseMatrix _sysDampingMatrix;

		SparseMatrix _sysSubMassMatrix;
		SparseMatrix _sysSubStiffnessMatrix;
		SparseMatrix _sysSubDampingMatrix;

		VectorX _sysSurfaceOriginalPosition;
		VectorX _sysOriginalPosition;
		VectorX _sysCurrentPosition;
		VectorX _sysVelocity;
		VectorX _sysAccrelation;
		VectorX _sysExternalForce;
		VectorX _sysInternalForce;
		VectorX _sysDampingForce;
		VectorX _sysGravityForce;
		VectorX _sysGravity;
		VectorX _sysMouseForce;
		VectorX _sysCollideForce;
		VectorX _meshCollisionForce;

		VectorX _tetElementInternalForce;

		std::vector<MatrixX> dPdF;
		std::vector<Matrix3> elementsInternalForce;

		std::vector<int> _mapOldNew;
		std::vector<int> _mapSparseMatrixEntryOldNew;
		int **_row;
		int **_column;
		SparseMatrix* _stiffnessMatrixTopology;

		void createSparseMapbyTopology(SparseMatrix& target, SparseMatrix& result);
		void sparseMatrixRemoveRows(SparseMatrix& target, SparseMatrix& result);
		void vectorRemoveRows(VectorX& target, VectorX& result);
		void vectorInsertRows(VectorX& target, VectorX& result);
		void resetSparseMatrix(SparseMatrix& sparseMatrix);
		void getSparseMatrixTopologyTYPE(SparseMatrix& sparesMatrixTopology);

		Vector3 _maxBbox;
		Vector3 _minBbox;
		qeal _diagLen;
	};
}



#endif