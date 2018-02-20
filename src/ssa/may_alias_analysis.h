/*******************************************************************\

Module: May-alias analysis for a single function

Author: Viktor Malik, imalik@fit.vutbr.cz

\*******************************************************************/

#ifndef CPROVER_2LS_SSA_MAY_ALIAS_ANALYSIS_H
#define CPROVER_2LS_SSA_MAY_ALIAS_ANALYSIS_H

#include <analyses/ai.h>
#include <util/union_find.h>
#include "ssa_value_set.h"

class may_alias_domaint:public ai_domain_baset
{
public:
  void transform(
    locationt from,
    locationt to,
    ai_baset &ai,
    const namespacet &ns) override;

  bool merge(const may_alias_domaint &other, locationt from, locationt to);

  typedef union_find<irep_idt> aliasest;
  aliasest aliases;

protected:
  void assign_lhs_aliases(
    const exprt &lhs,
    const std::set<irep_idt> &rhs_alias_set);
};

class may_alias_analysist:public ait<may_alias_domaint>
{
public:
  may_alias_analysist(
    const goto_functionst::goto_functiont &goto_function,
    const namespacet &ns)
  {
    operator()(goto_function, ns);
  }
};


#endif // CPROVER_2LS_SSA_MAY_ALIAS_ANALYSIS_H
