//
// Copyright (c) 2017-2018 CNRS
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

#ifndef __pinocchio_multibody_fwd_hpp__
#define __pinocchio_multibody_fwd_hpp__

#include "pinocchio/fwd.hpp"

# include <cstddef> // std::size_t
#include "pinocchio/multibody/joint/fwd.hpp"

namespace pinocchio
{
  typedef std::size_t Index;
  typedef Index JointIndex;
  typedef Index GeomIndex;
  typedef Index FrameIndex;
  typedef Index PairIndex;
  
  template<typename Scalar, int Options=0> struct FrameTpl;
  typedef FrameTpl<double> Frame;
  
  template<typename Scalar, int Options = 0, template<typename S, int O> class JointCollectionTpl = JointCollectionDefaultTpl>
  struct ModelTpl;
  typedef ModelTpl<double> Model;
  
  template<typename Scalar, int Options = 0, template<typename S, int O> class JointCollectionTpl = JointCollectionDefaultTpl>
  struct DataTpl;
  typedef DataTpl<double> Data;
  
  struct GeometryModel;
  struct GeometryData;
  
  enum ReferenceFrame
  {
    WORLD = 0,
    LOCAL = 1
  };

  // Forward declaration needed for Model::check
  template<class D> struct AlgorithmCheckerBase;

} // namespace pinocchio

#endif // #ifndef __pinocchio_multibody_fwd_hpp__
