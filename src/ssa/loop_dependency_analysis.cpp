//
// Created by Martin Hruska on 26/01/2018.
//

#include <iostream>
#include "loop_dependency_analysis.h"
#include "analysis_tools.h"
#include "ssa_pointed_objects.h"
#include "ssa_object.h"


#define IN(x,a,b) (a <= x && x<=b)

void loop_dependency_analysis::output(
    std::ostream &out,
    const ai_baset &ai,
    const namespacet &ns) const
  {
    ai_domain_baset::output(out, ai, ns);
    out << "Dependency graph: \n";
    for(const std::pair<size_t, std::set<size_t>> &pair:dependency_graph)
    {
      out << pair.first << " [line "
        << static_cast<const loop_dependency_ait &>(ai).loops_locations.at(0).first
        << "] depends on ";
      for(const size_t& x:pair.second)
      {
          out << x << ", ";
      }
      out << "\n";
    }
  }

void loop_dependency_analysis::transform(
        ai_domain_baset::locationt from,
        ai_domain_baset::locationt to,
        ai_baset &ai,
        const namespacet &ns)
{
    if(!from->is_assign())
    {
        return;
    }

    const code_assignt &code_assign=to_code_assign(from->code);
    const exprt lhs_deref=analysis_tools::dereference(code_assign.lhs(), ns);
    const exprt rhs_deref=analysis_tools::dereference(code_assign.rhs(), ns);

    size_t i=0;
    for(const auto& loop_interval : static_cast<const loop_dependency_ait &>(ai).loops_locations)
    {
        if IN(from->location_number, loop_interval.first, loop_interval.second)
        {
          if(lhs_deref.type().id()==ID_pointer)
          {
            cycle_lhs[i].insert(to_symbol_expr(lhs_deref).get_identifier());
          }
          // get_rhs_aliases(rhs_deref, cycle_rhs[i]);
          analysis_tools::get_rhs_values(rhs_deref, cycle_rhs[i], [](std::set<irep_idt> &, const irep_idt&){});
        }
        ++i;
    }
}


bool loop_dependency_analysis::merge(
        const loop_dependency_analysis &other,
        ai_domain_baset::locationt from,
        ai_domain_baset::locationt to)
{
   bool changed=false;
   for(const auto &pair_lhs:other.cycle_lhs)
   {
     cycle_lhs[pair_lhs.first].insert(pair_lhs.second.begin(), pair_lhs.second.end());
   }
   for(const auto &pair_rhs:other.cycle_rhs)
   {
     cycle_rhs[pair_rhs.first].insert(pair_rhs.second.begin(), pair_rhs.second.end());
   }
   for(const auto &pair_graph:other.dependency_graph)
   {
     dependency_graph[pair_graph.first].insert(pair_graph.second.begin(), pair_graph.second.end());
   }

   for(const auto &pair_lhs:other.cycle_lhs)
   {
     for(const auto &pair_rhs:other.cycle_rhs)
     {
       for(const auto &item_lhs:pair_lhs.second)
       {
         if (pair_rhs.second.count(item_lhs) && (!dependency_graph.count(pair_rhs.first)
                                                || !dependency_graph.at(pair_rhs.first).count(pair_lhs.first)))
         {
           changed=true;
           dependency_graph[pair_rhs.first].insert(pair_lhs.first);
         }
       }
     }
   }

  return changed;
}

/*
const exprt loop_dependency_analysis::dereference(
  const exprt &expr,
  const namespacet &ns)
{
  exprt deref=symbolic_dereference(expr, ns);
  members_to_symbols(deref, ns);
  return deref;
}

void loop_dependency_analysis::members_to_symbols(exprt &expr, const namespacet &ns)
{
  ssa_objectt object(expr, ns);
  if(object)
    expr=object.symbol_expr();
  Forall_operands(it, expr)members_to_symbols(*it, ns);
}

void loop_dependency_analysis::get_rhs_aliases(
  const exprt &rhs,
  std::set<const irep_idt> &rhs_ids)
{
  analysis_tools::get_rhs_values(rhs, rhs_ids, [](){});
  return;
  if(rhs.id()==ID_symbol &&
     id2string(to_symbol_expr(rhs).get_identifier()).find("__CPROVER_")==
     std::string::npos)
  {
    irep_idt identifier=to_symbol_expr(rhs).get_identifier();
    rhs_ids.insert(identifier);
  }
  else if(rhs.id()==ID_if)
  {
    get_rhs_aliases(to_if_expr(rhs).true_case(), rhs_ids);
    get_rhs_aliases(to_if_expr(rhs).false_case(), rhs_ids);
  }
  else if(rhs.id()==ID_typecast)
  {
    get_rhs_aliases(to_typecast_expr(rhs).op(), rhs_ids);
  }
}
*/

void loop_dependency_ait::initialize(const goto_functionst &goto_functions)
{
    ait<loop_dependency_analysis>::initialize(goto_functions);
    forall_goto_functions(f_it, goto_functions)
    {
        for (const auto& instr : f_it->second.body.instructions)
        {
            if (instr.is_backwards_goto())
            {
                const auto &loophead=instr.get_target();
                loops_locations.push_back(std::pair<int, int>(loophead->location_number, instr.location_number));
            }
        }
    }
}
