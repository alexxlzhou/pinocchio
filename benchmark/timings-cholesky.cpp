//
// Copyright (c) 2018 CNRS
//
// This file is part of Pinocchio
// Pinocchio is free software: you can redistribute it
// and/or modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation, either version
// 3 of the License, or (at your option) any later version.
//
// Pinocchio is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Lesser Public License for more details. You should have
// received a copy of the GNU Lesser General Public License along with
// Pinocchio If not, see
// <http://www.gnu.org/licenses/>.

#include "pinocchio/spatial/fwd.hpp"
#include "pinocchio/spatial/se3.hpp"
#include "pinocchio/multibody/visitor.hpp"
#include "pinocchio/multibody/model.hpp"
#include "pinocchio/multibody/data.hpp"
#include "pinocchio/algorithm/crba.hpp"
#include "pinocchio/algorithm/aba.hpp"
#include "pinocchio/algorithm/cholesky.hpp"
#include "pinocchio/parsers/urdf.hpp"
#include "pinocchio/parsers/sample-models.hpp"

#include <iostream>

#include "pinocchio/utils/timer.hpp"

#include <Eigen/StdVector>
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Eigen::VectorXd)

int main(int argc, const char ** argv)
{
  using namespace Eigen;
  using namespace pinocchio;

  PinocchioTicToc timer(PinocchioTicToc::US);
  #ifdef NDEBUG
  const int NBT = 1000*100;
  #else
    const int NBT = 1;
    std::cout << "(the time score in debug mode is not relevant) " << std::endl;
  #endif
    
  pinocchio::Model model;

  std::string filename = PINOCCHIO_SOURCE_DIR"/models/simple_humanoid.urdf";
  if(argc>1) filename = argv[1];
  if( filename == "HS") 
    pinocchio::buildModels::humanoidRandom(model,true);
  else if( filename == "H2" )
    pinocchio::buildModels::humanoid2d(model);
  else
    pinocchio::urdf::buildModel(filename,JointModelFreeFlyer(),model);
  std::cout << "nq = " << model.nq << std::endl;

  pinocchio::Data data(model);
  VectorXd q = VectorXd::Random(model.nq);
  VectorXd qdot = VectorXd::Random(model.nv);
  VectorXd qddot = VectorXd::Random(model.nv);
  
  MatrixXd A(model.nv,model.nv), B(model.nv,model.nv);
  A.setZero(); B.setRandom();

  std::vector<VectorXd> qs     (NBT);
  std::vector<VectorXd> lhs  (NBT);
  std::vector<VectorXd> rhs (NBT);
  for(size_t i=0;i<NBT;++i)
  {
    qs[i] = Eigen::VectorXd::Random(model.nq);
    lhs[i] = Eigen::VectorXd::Zero(model.nv);
    rhs[i] = Eigen::VectorXd::Random(model.nv);
  }
  
  double total = 0;
  SMOOTH(NBT)
    {
      crba(model,data,qs[_smooth]);
      timer.tic();
      cholesky::decompose(model,data);
      total += timer.toc(timer.DEFAULT_UNIT);
    }
  std::cout << "Cholesky = \t" << (total/NBT) 
	    << " " << timer.unitName(timer.DEFAULT_UNIT) <<std::endl;

  total = 0;
  Eigen::LDLT<Eigen::MatrixXd> Mldlt(data.M);
  SMOOTH(NBT)
  {
    crba(model,data,qs[_smooth]);
    data.M.triangularView<Eigen::StrictlyLower>()
    = data.M.transpose().triangularView<Eigen::StrictlyLower>();
    timer.tic();
    Mldlt.compute(data.M);
    total += timer.toc(timer.DEFAULT_UNIT);
  }
  std::cout << "Dense Eigen Cholesky = \t" << (total/NBT)
  << " " << timer.unitName(timer.DEFAULT_UNIT) <<std::endl;
  
  timer.tic();
  SMOOTH(NBT)
  {
    cholesky::solve(model,data,rhs[_smooth]);
  }
  std::cout << "Cholesky solve vector = \t\t"; timer.toc(std::cout,NBT);
  
  timer.tic();
  SMOOTH(NBT)
  {
    cholesky::UDUtv(model,data,rhs[_smooth]);
  }
  std::cout << "UDUtv = \t\t"; timer.toc(std::cout,NBT);
  
  MatrixXd Minv(model.nv,model.nv); Minv.setZero();
  timer.tic();
  SMOOTH(NBT)
  {
    cholesky::computeMinv(model,data,Minv);
  }
  std::cout << "Minv from cholesky = \t\t"; timer.toc(std::cout,NBT);
  
  timer.tic();
  SMOOTH(NBT)
  {
    cholesky::solve(model,data,Minv.col(10));
  }
  std::cout << "Cholesky solve column = \t\t"; timer.toc(std::cout,NBT);
  
  timer.tic();
  SMOOTH(NBT)
  {
    lhs[_smooth].noalias() = Minv*rhs[_smooth];
  }
  std::cout << "Minv*v = \t\t"; timer.toc(std::cout,NBT);
  
  timer.tic();
  SMOOTH(NBT)
  {
    A.noalias() = Minv*B;
  }
  std::cout << "A = Minv*B = \t\t"; timer.toc(std::cout,NBT);
  
  data.M.triangularView<Eigen::StrictlyLower>()
  = data.M.transpose().triangularView<Eigen::StrictlyLower>();
  timer.tic();
  SMOOTH(NBT)
  {
    A.noalias() = data.M.inverse();
  }
  std::cout << "M.inverse() = \t\t"; timer.toc(std::cout,NBT);
  
  timer.tic();
  SMOOTH(NBT)
  {
    computeMinverse(model,data,qs[_smooth]);
  }
  std::cout << "M.inverse() = \t\t"; timer.toc(std::cout,NBT);
  
  return 0;
}
