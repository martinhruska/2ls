//
//  analysis_tools.cpp
//  2ls
//
//  Created by Martin Hruska on 19/02/2018.
//

#include "analysis_tools.h"
                                        
/*******************************************************************\
 
 Function: may_alias_domaint::dereference
 
 Inputs:
 
 Outputs:
 
 Purpose:
 
 \*******************************************************************/
const exprt analysis_tools::dereference(
  const exprt &expr,
  const namespacet &ns)
{
    exprt deref=symbolic_dereference(expr, ns);
    members_to_symbols(deref, ns);
    return deref;
}
                                        
/*******************************************************************\
 
 Function: may_alias_domaint::members_to_symbols
 
 Inputs:
 
 Outputs:
 
 Purpose:
 
 \*******************************************************************/
void analysis_tools::members_to_symbols(exprt &expr, const namespacet &ns)
{
    ssa_objectt object(expr, ns);
    if(object)
        expr=object.symbol_expr();
    Forall_operands(it, expr)members_to_symbols(*it, ns);
}
