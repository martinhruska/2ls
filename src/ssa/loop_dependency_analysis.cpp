//
// Created by Martin Hruska on 26/01/2018.
//

#include "loop_dependency_analysis.h"
#include "analysis_tools.h"
#include "ssa_pointed_objects.h"
#include "ssa_object.h"


#define IN(x,a,b) (a <= x && x<=b)

namespace
{
    void print_cycle(
      std::ostream &out,
      const std::unordered_map<size_t, std::set<irep_idt>>& cycle)
    {
      for(const std::pair<size_t, std::set<irep_idt>>& lhsp:cycle)
      {
        out << lhsp.first << " -> {";
        for(const irep_idt& ls:lhsp.second)
        {
            out << ls.c_str() << ", ";
        }
        out << "}\n";
      }
    }
}

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
      << static_cast<const loop_dependency_ait &>(ai).loops_locations.at(pair.first).first
      << "] depends on ";
    for(const size_t& x:pair.second)
    {
        out << x << ", ";
    }
    out << "\n";
  }
  out << "LHS ";
  print_cycle(out, cycle_lhs);
  out << "RHS ";
  print_cycle(out, cycle_rhs);
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
    size_t size_before=cycle_lhs[pair_lhs.first].size();
    cycle_lhs[pair_lhs.first].insert(pair_lhs.second.begin(), pair_lhs.second.end());
    changed|=(cycle_lhs[pair_lhs.first].size()-size_before);
  }
  for(const auto &pair_rhs:other.cycle_rhs)
  {
    size_t size_before=cycle_rhs[pair_rhs.first].size();
    cycle_rhs[pair_rhs.first].insert(pair_rhs.second.begin(), pair_rhs.second.end());
    changed|=(cycle_rhs[pair_rhs.first].size()-size_before);
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
        if (pair_lhs.first != pair_rhs.first &&
            std::string(item_lhs.c_str()).find("malloc") != std::string::npos)
        {
            continue;
        }
        if (pair_rhs.second.count(item_lhs) && (!dependency_graph.count(pair_rhs.first)
                                               || !dependency_graph.at(pair_rhs.first).count(pair_lhs.first)))
        {
          changed|=true;
          dependency_graph[pair_rhs.first].insert(pair_lhs.first);
        }
      }
    }
  }

  return changed;
}

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
