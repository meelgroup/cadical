#ifndef _internal_hpp_INCLUDED
#define _internal_hpp_INCLUDED

#include <cassert>
#include <climits>
#include <vector>

/*------------------------------------------------------------------------*/

using namespace std;

#include "macros.hpp"
#include "options.hpp"
#include "clause.hpp"
#include "var.hpp"
#include "watch.hpp"
#include "ema.hpp"
#include "avg.hpp"
#include "level.hpp"
#include "parse.hpp"
#include "proof.hpp"
#include "profile.hpp"
#include "timer.hpp"
#include "logging.hpp"
#include "file.hpp"
#include "message.hpp"
#include "stats.hpp"
#include "util.hpp"
#include "queue.hpp"
#include "report.hpp"
#include "format.hpp"

/*------------------------------------------------------------------------*/

namespace CaDiCaL {

class Internal {

  friend class Solver;

  friend class Parser;
  friend struct Logger;
  friend struct Message;
  friend struct Queue;
  friend class Proof;
  friend struct Stats;

  friend struct trail_smaller_than;
  friend struct trail_greater_than;
  friend struct bump_earlier;
 
  /*----------------------------------------------------------------------*/

  // The actual state of the solver is in this section.

  int max_var;                  // maximum variable index
  size_t vsize;                 // actually allocated variable data size
  Var * vtab;                   // variable table
  signed char * vals;           // current partial assignment
  signed char * phases;         // saved last assignment
  Watches * wtab;               // table of watches for all literals
  Queue queue;                  // variable move to front decision queue
  bool unsat;                   // empty clause found or learned
  int level;                    // decision level (levels.size () - 1)
  vector<Level> control;        // 'level + 1 == levels.size ()'
  vector<int> trail;            // assigned literals
  size_t propagated;            // next position on trail to propagate
  vector<int> clause;           // temporary clause in parsing & learning
  vector<Clause*> clauses;      // ordered collection of all clauses
  bool iterating;               // report learned unit (iteration)
  vector<int> seen;             // seen & bumped literals in 'analyze'
  vector<int> levels;           // decision levels of 1st UIP clause
  vector<int> minimized;        // marked removable or poison in 'minmize'
  vector<Clause*> resolved;     // large clauses in 'analyze'
  Clause * conflict;            // set in 'propagation', reset in 'analyze'
  bool clashing_unit;           // set in 'parse_dimacs'
  EMA fast_glue_avg;            // fast exponential moving average
  EMA slow_glue_avg;            // slow exponential moving average
  AVG jump_avg;                 // average back jump level
  long reduce_limit;            // conflict limit for next 'reduce'
  long restart_limit;           // conflict limit for next 'restart'
  long recently_resolved;       // to keep recently resolved clauses
  int fixed_limit;              // remember last number of units
  long reduce_inc;              // reduce interval increment
  Proof * proof;                // trace clausal proof if non zero
  Options opts;			// run-time options
  Stats stats;			// statistics
  signed char * solution;       // for debugging (like 'vals' and 'phases')
  vector<int> original;		// original CNF for debugging
  vector<Timer> timers;         // active timers for profiling functions
  Profiles profiles;            // global profiled time for functions
  Format error;                 // last (persistent) error message
  Internal * internal;          // proxy to 'this' in macros (redundant)

  /*----------------------------------------------------------------------*/
  
  void resize (int new_max_var);

  // Functions for monitoring resources.
  //
  size_t vector_bytes ();
  void inc_bytes (size_t);
  void dec_bytes (size_t);

  double seconds ();
  size_t max_bytes ();
  size_t current_bytes ();

  int active_variables () const { return max_var - stats.fixed; }

  // Regularly reports what is going on in 'report.cpp'.
  //
  void report (char type, bool verbose = false);

  // Unsigned literals (abs) with checks.
  //
  int vidx (int lit) const {
    int idx;
    assert (lit), assert (lit != INT_MIN);
    idx = abs (lit);
    assert (idx <= max_var);
    return idx;
  }

  // Get the index of variable (with checking).
  //
  int var2idx (Var * v) const {
    assert (v), assert (vtab < v), assert (v <= vtab + max_var);
    return v - vtab;
  }

  // Unsigned version with LSB denoting sign.  This is used in indexing arrays
  // by literals.  The idea is to keep the elements in such an array for both
  // the positive and negated version of a literal close together.
  //
  unsigned vlit (int lit) { return (lit < 0) + 2u * (unsigned) vidx (lit); }

  // Helper function to access variables and watches to avoid indexing bugs.
  //
  Var & var (int lit)         { return vtab [vidx (lit)]; }
  Watches & watches (int lit) { return wtab [vlit (lit)]; }

  // Watch literal 'lit' in clause with blocking literal 'blit'.
  // Inlined here, since it occurs in the tight inner loop of 'propagate'.
  //
  void watch_literal (int lit, int blit, Clause * c) {
    Watches & ws = watches (lit);
    ws.push_back (Watch (blit, c));
    LOG (c, "watch %d blit %d in", lit, blit);
  }

  // Managing clauses in 'clause.cpp'.  Without explicit 'Clause' argument
  // these functions work on the global temporary 'clause'.
  //
  void watch_clause (Clause *);
  size_t bytes_clause (int size);
  Clause * new_clause (bool red, int glue = 0);
  size_t delete_clause (Clause *);
  bool tautological_clause ();
  void add_new_original_clause ();
  Clause * new_learned_clause (int glue);

  // Forward reasoning through propagation in 'propagate.cpp'.
  //
  void assign (int lit, Clause * reason = 0);
  bool propagate ();

  // Undo and restart in 'backtrack.cpp'.
  //
  void unassign (int lit);
  void backtrack (int target_level = 0);

  // Learning from conflicts in 'analyze.cc'.
  //
  void learn_empty_clause ();
  void learn_unit_clause (int lit);
  bool minimize_literal (int lit, int depth = 0);
  void minimize_clause ();
  void bump_variable (Var * v);
  void bump_and_clear_seen_variables ();
  void bump_resolved_clauses ();
  void resolve_clause (Clause *);
  void clear_levels ();
  bool analyze_literal (int);
  void analyze ();
  void iterate ();       // for reporting learned unit clause

  // Restarting policy in 'restart.cc'.
  //
  bool restarting ();
  int reuse_trail ();
  void restart ();

  // Reducing means garbage collecting useless clauses in 'reduce.cpp'.
  //
  bool reducing ();
  void protect_reasons ();
  void unprotect_reasons ();
  int clause_contains_fixed_literal (Clause *);
  void flush_falsified_literals (Clause *);
  void mark_satisfied_clauses_as_garbage ();
  void mark_useless_redundant_clauses_as_garbage ();
  void delete_garbage_clauses ();
  void flush_watches ();
  void setup_watches ();
  void garbage_collection ();
  void reduce ();

  // Part on picking the next decision in 'decide.cpp'.
  //
  bool satisfied () const { return trail.size () == (size_t) max_var; }
  int next_decision_variable ();
  void decide ();

  // Main search functions in 'internal.cpp'.
  //
  int search ();                // CDCL loop
  void init_solving ();
  int solve ();

  // Built in profiling in 'profile.cpp'.
  //
  void start_profiling (Profile * p);
  void stop_profiling (Profile * p);
  void update_all_timers (double now);
  void print_profile (double now);

  // Checking solutions (see 'solution.cpp').
  //
  int sol (int lit);
  void check_clause ();

  void check (int (Internal::*assignment) (int));

  Internal ();
  ~Internal ();

  // Get the value of a literal: -1 = false, 0 = unassigned, 1 = true.
  //
  inline int val (int lit) {
    int idx = vidx (lit), res = vals[idx];
    if (lit < 0) res = -res;
    return res;
  }

  // As 'val' but restricted to the root-level value of a literal.
  //
  inline int fixed (int lit) {
    int idx = vidx (lit), res = vals[idx];
    if (res && vtab[idx].level) res = 0;
    if (lit < 0) res = -res;
    return res;
  }

  // Parsing functions (handed over to 'parse.cpp').
  //
  const char * parse_dimacs (FILE *);
  const char * parse_dimacs (const char *);
  const char * parse_solution (const char *);
};

};

#endif
