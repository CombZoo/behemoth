// mainKP-fcore-ex.cpp
#include <fstream>
#include <iomanip> // setw
#include <iostream>
#include <nlohmann/json.hpp>

//#include <solver_kp01_generic/solver_kp01_generic.h> // general solver
#include <behemoth/UnifiedSolver.hpp>
#include <solver_kp01_generic/solver_kp01_generic.h>

// import everything on main()
using namespace std;

class Behemoth
{
public:
   behemoth::UnifiedSolver unisolver;

   int runArgs(int argc, char **argv)
   {
      std::cout << "loading parameters" << std::endl;
      string local = argv[0];
      vector<string> params(argc);
      bool info = false;
      for (int i = 1; i < argc; i++)
      {
         params[i] = argv[i];
         if (params[i] == "--help")
         {
            std::cout << "help: behemoth-solver --problem kp01 --config default_kp01.json" << std::endl;
            return 0;
         }
         if (params[i] == "--info")
         {
            info = true;
         }
      }

      string problem;
      for (unsigned i = 0; i < params.size(); i++)
      {
         if (params[i] == "--problem")
            problem = params[i + 1];
      }

      if (problem == "")
      {
         std::cout << "missing --problem... aborting!" << std::endl;
         return 1;
      }

      std::cout << "loading solvers for problem '" << problem << "'" << std::endl;
      unisolver.load(problem);

      // must display --info
      if (info)
      {
         string sout = unisolver.info(1, "", problem);
         return 0;
      }

      string config_file;
      for (unsigned i = 0; i < params.size(); i++)
      {
         if (params[i] == "--config")
            config_file = params[i + 1];
      }

      if (config_file == "")
      {
         std::cout << "No run configuration. Try '--config', '--info' or '--help'. Aborting!" << std::endl;
         return 1;
      }
      else
      {
         std::cout << "loading file '" << config_file << "'" << std::endl;
         std::ifstream ijson(config_file);
         if (!ijson.is_open())
         {
            std::cout << "file not found!" << std::endl;
            return 1;
         }
         nlohmann::json j;
         ijson >> j;

         std::stringstream ss;
         ss << std::setw(4) << j << std::endl;
         string config = ss.str();

         // TODO: consider solver_name
         // invoking unified solver!
         string sout = unisolver.solve(1, config, problem);
         //

         // TODO: parse solve output
         std::cout << "output = '" << sout << "'" << std::endl;
         return 0;
      }

      return 0;
   }
};

int main(int argc, char **argv)
{
   Behemoth solver;
   return solver.runArgs(argc, argv);
}