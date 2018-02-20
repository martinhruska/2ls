//
//  analysis_tools.hpp
//  2ls
//
//  Created by Martin Hruska on 19/02/2018.
//

#ifndef analysis_tools_h
#define analysis_tools_h

#include <stdio.h>
#include <set>

#include "ssa_value_set.h"

class analysis_tools
{
public:
    template<class T>
    static void get_rhs_values(
      const exprt &rhs,
      std::set<irep_idt> &values_set,
      T process_symbol)
    {
        if(rhs.id()==ID_symbol &&
           id2string(to_symbol_expr(rhs).get_identifier()).find("__CPROVER_")==
           std::string::npos)
        {
            irep_idt identifier=to_symbol_expr(rhs).get_identifier();
            values_set.insert(identifier);
            process_symbol(values_set, identifier);
        }
        else if(rhs.id()==ID_if)
        {
            get_rhs_values(to_if_expr(rhs).true_case(), values_set, process_symbol);
            get_rhs_values(to_if_expr(rhs).false_case(), values_set, process_symbol);
        }
        else if(rhs.id()==ID_typecast)
        {
            get_rhs_values(to_typecast_expr(rhs).op(), values_set, process_symbol);
        }
    }

    static const exprt dereference(const exprt &expr, const namespacet &ns);
    static void members_to_symbols(exprt &expr, const namespacet &ns);
};

#endif /* analysis_tools_hpp */
