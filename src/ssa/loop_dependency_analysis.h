//
// Created by Martin Hruska on 26/01/2018.
//

#ifndef CPROVER_2LS_LOOP_DEPENDENCY_ANALYSIS_H
#define CPROVER_2LS_LOOP_DEPENDENCY_ANALYSIS_H

#include <analyses/ai.h>

class loop_dependency_analysis:public ai_domain_baset
{
public:

  void transform(
    locationt from,
    locationt to,
    ai_baset &ai,
    const namespacet &ns) override;

  bool merge(const loop_dependency_analysis &other, locationt from, locationt to);

  virtual void output(
    std::ostream &out,
    const ai_baset &ai,
    const namespacet &ns) const override;

private:
  std::unordered_map<size_t, std::set<size_t>> dependency_graph;
  std::unordered_map<size_t, std::set<irep_idt>> cycle_lhs;
  std::unordered_map<size_t, std::set<irep_idt>> cycle_rhs;
};


class loop_dependency_ait:public ait<loop_dependency_analysis>
{
public:
  explicit loop_dependency_ait(
    const goto_functionst &goto_functions,
    const namespacet &ns) :
          loops_locations()
  {
    operator()(goto_functions, ns);
  }

protected:
  friend class loop_dependency_analysis;

  std::vector<std::pair<int, int>> loops_locations;

  // The overload below is needed to make the entry point get a source
  // for all objects.
  virtual void initialize(const goto_functionst &goto_functions);
};


#endif //CPROVER_2LS_LOOP_DEPENDENCY_ANALYSIS_H
