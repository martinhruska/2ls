#ifndef CPROVER_DELTACHECK_SUMMARIZER_H
#define CPROVER_DELTACHECK_SUMMARIZER_H

#include "summary.h"
#include "ssa_inliner.h"
#include "../ssa/local_ssa.h"
//#include "../deltacheck/analyzer.h"

class summary_storet;

class summarizert
{
 public:
 summarizert(summary_storet &_summary_store
   /*,analyzert &_analyzer*/) : 
    summary_store(_summary_store), 
  //analyzer(_analyzer),
    inliner()
  {}

  typedef summaryt::predicatet preconditiont;
  typedef irep_idt function_namet;
  typedef local_SSAt* function_bodyt;
  typedef std::map<function_namet, preconditiont> preconditionst;
  typedef std::map<function_namet, function_bodyt> functionst;
  typedef functionst::value_type functiont;

  summaryt summarize(const functiont &function, const preconditiont &precondition); 
  summaryt summarize(const functiont &function);

  void summarize(const functionst &functions, const preconditionst &preconditions); 
  void summarize(const functionst &functions); 

 protected:
  summary_storet &summary_store;
  //  analyzert &analyzer;
  ssa_inlinert inliner;
  functionst functions;
  preconditionst preconditions;

  void run();

 private:
  typedef std::map<function_namet, bool> flag_mapt;
  flag_mapt summary_updated;

  void compute_summary_rec(function_namet function_name);
};


#endif