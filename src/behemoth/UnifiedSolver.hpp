// TODO: this file should be generated automatically
#ifndef BEHEMOTH_UNIFIED_SOLVER_HPP
#define BEHEMOTH_UNIFIED_SOLVER_HPP

#include <algorithm>
#include <functional>
#include <iostream>

// TODO: this file should be generated automatically
#include <solver_kp01_generic/solver_kp01_generic.h>
// this .cpp should not be necessary!! TODO: fix (on Bazel?)
#include <solver_kp01_generic/solver_kp01_generic.cpp>

using namespace std; // TODO: remove

// namespace para o problema da mochila
namespace behemoth
{

   class UnifiedSolver
   {
   public:
      typedef int (*SolveFunc)(int, const char *, char *);
      typedef int (*InfoFunc)(int, const char *, char *);

      std::map<string, SolveFunc> solvers;
      std::map<string, InfoFunc> infos;

      // load solvers according to prefix
      void load(string prefix = "")
      {
         // populate solver according to prefix (TODO: check prefix)
         SolveFunc solver = ::solver_kp01_generic;
         solvers["kp01_generic"] = solver;
         infos["kp01_generic"] = ::info_kp01_generic;
      }

      // format = 1 (json), = 0 (default)
      string solve(int format, string config, string problem, string solver_name = "")
      {
         // match problem and solver name
         std::stringstream ss;
         if (solver_name == "")
            ss << problem;
         else
            ss << problem << "_" << solver_name;
         string solver_project = ss.str();

         string output(1000, '\0');
         if (solvers.find(solver_project) == solvers.end())
         {
            std::cout << "could not find solver '" << solver_project << "'" << std::endl;
            std::cout << "trying complete check!" << std::endl;
            for (auto &it : solvers)
            {
               if (it.first.find(solver_project, 0) != string::npos)
               {
                  std::cout << "found solver at '" << it.first << "'" << std::endl;

                  // find solver options (TODO: just get one!)
                  int (*solver)(int, const char *, char *) = it.second;

                  // invoke solver
                  std::cout << "invoking solver '" << ss.str() << "'" << std::endl;
                  solver(format, config.c_str(), (char *)output.c_str());
               }
            }
         }
         else
         {

            // find solver options (TODO: just get one!)
            int (*solver)(int, const char *, char *) = solvers[ss.str()];

            
            // invoke solver
            std::cout << "invoking solver '" << ss.str() << "'" << std::endl;
            solver(format, config.c_str(), (char *)output.c_str());
         }

         return output;
      }

      string info(int format, string key, string problem, string solver_name = "")
      {
         // match problem and solver name
         std::stringstream ss;
         ss << problem << "_" << solver_name;

         string output(1000, '\0');

         // find info options (TODO: just get one!)
         int (*info)(int, const char *, char *) = infos[ss.str()];

         info(format, key.c_str(), (char *)output.c_str());
         return output;
      }
   };

   //
} // namespace behemoth

#endif // BEHEMOTH_UNIFIED_SOLVER_HPP