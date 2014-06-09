/*******************************************************************\

Module: Data Flow Analysis

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#ifndef DELTACHECK_SSA_FIXED_POINT_H
#define DELTACHECK_SSA_FIXED_POINT_H

#include "../ssa/local_ssa.h"

class ssa_analyzert
{
public:
  typedef strategy_solver_baset::constraintst constraintst;
  typedef strategy_solver_baset::var_listt var_listt;

  explicit ssa_analyzert(const namespacet &_ns)
    : ns(_ns)
  {
  }  

  void operator()(local_SSAt &SSA);

protected:
  const namespacet &ns;
  strategy_solver_baset::invariantt inv;
  unsigned iteration_number;
};


#endif
