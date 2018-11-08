//
// Copyright (c) 2015-2016,2018 CNRS
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

#include "pinocchio/bindings/python/algorithm/algorithms.hpp"
#include "pinocchio/algorithm/kinematics.hpp"

namespace pinocchio
{
  namespace python
  {
    
    void exposeKinematics()
    {
      using namespace Eigen;
      bp::def("updateGlobalPlacements",
              &updateGlobalPlacements<double,0,JointCollectionDefaultTpl>,
              bp::args("Model","Data"),
              "Updates the global placements of all the frames of the kinematic "
              "tree and put the results in data according to the relative placements of the joints.");
      
      bp::def("forwardKinematics",
              &forwardKinematics<double,0,JointCollectionDefaultTpl,VectorXd>,
              bp::args("Model","Data",
                       "Configuration q (size Model::nq)"),
              "Compute the global placements of all the joints of the kinematic "
              "tree and put the results in data.");
      
      bp::def("forwardKinematics",
              &forwardKinematics<double,0,JointCollectionDefaultTpl,VectorXd,VectorXd>,
              bp::args("Model","Data",
                       "Configuration q (size Model::nq)",
                       "Velocity v (size Model::nv)"),
              "Compute the global placements and local spatial velocities of all the joints of the kinematic "
              "tree and put the results in data.");
      
      bp::def("forwardKinematics",
              &forwardKinematics<double,0,JointCollectionDefaultTpl,VectorXd,VectorXd,VectorXd>,
              bp::args("Model","Data",
                       "Configuration q (size Model::nq)",
                       "Velocity v (size Model::nv)",
                       "Acceleration a (size Model::nv)"),
              "Compute the global placements, local spatial velocities and spatial accelerations of all the joints of the kinematic "
              "tree and put the results in data.");
    }
    
  } // namespace python
} // namespace pinocchio
