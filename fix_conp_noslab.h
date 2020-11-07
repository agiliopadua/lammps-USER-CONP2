/* ----------------------------------------------------------------------
   LAMMPS - Large-scale Atomic/Molecular Massively Parallel Simulator
   http://lammps.sandia.gov, Sandia National Laboratories
   Steve Plimpton, sjplimp@sandia.gov

   Copyright (2003) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under 
   the GNU General Public License.

   See the README file in the top-level LAMMPS directory.
------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------
   Version: Nov/2020
   Shern Ren Tee (UQ AIBN), s.tee@uq.edu.au
------------------------------------------------------------------------- */

#ifdef FIX_CLASS

FixStyle(conp/noslab,FixConpNoslab)

#else

#ifndef LMP_FIX_CONP_NOSLAB_H
#define LMP_FIX_CONP_NOSLAB_H

#include "fix.h"
#include "pair.h"
#include "fix_conpv3.h"

namespace LAMMPS_NS {

class FixConpNoslab : public FixConpV3 {
 public:
  FixConpNoslab(class LAMMPS *lmp, int narg, char **arg):FixConpV3(lmp,narg,arg) {};
  ~FixConpNoslab() {}
  void a_cal();
  void b_cal();
};

}

#endif
#endif
