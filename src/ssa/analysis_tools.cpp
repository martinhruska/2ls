//
//  analysis_tools.cpp
//  2ls
//
//  Created by Martin Hruska on 19/02/2018.
//

#include "analysis_tools.h"

/*
template <class T>
void analysis_tools::get_rhs_values(
  const exprt &rhs,
  std::set<irep_idt> &value_set,
  T process_symbol)
  // void (*process_symbol)(std::set<irep_idt> &value_set, const irep_idt& identifier))
{
    if(rhs.id()==ID_symbol &&
       id2string(to_symbol_expr(rhs).get_identifier()).find("__CPROVER_")==
       std::string::npos)
    {
        irep_idt identifier=to_symbol_expr(rhs).get_identifier();
        value_set.insert(identifier);
        process_symbol(value_set, identifier);
    }
    else if(rhs.id()==ID_if)
    {
        get_rhs_values(to_if_expr(rhs).true_case(), value_set, process_symbol);
        get_rhs_values(to_if_expr(rhs).false_case(), value_set, process_symbol);
    }
    else if(rhs.id()==ID_typecast)
    {
        get_rhs_values(to_typecast_expr(rhs).op(), value_set, process_symbol);
    }
}
 */
                                        
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
