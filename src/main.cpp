#include <fstream>
#include <sstream>
#include <string>
#include <iterator>
#include <regex>
#include <iostream>
#include <vector>
#include <array>
#include <map>
#include <algorithm>
#include <iomanip>
#include <boost/regex.hpp>

#include "yaml-cpp/node/node.h"
#include "yaml-cpp/yaml.h"

#include "yaml-cpp/node/node.h"
#include "yaml-cpp/emitter.h"
#include "yaml-cpp/node/convert.h"
#include "yaml-cpp/node/detail/impl.h"
#include "yaml-cpp/node/emit.h"
#include "yaml-cpp/node/impl.h"
#include "yaml-cpp/node/iterator.h"

using namespace std;

string escape_string_for_regex(string str)
{
   const regex esc("[.^$|()\\[\\]{}*+?\"\\\\]");
   const string rep("\\\\&");
   return regex_replace(str, esc, rep, regex_constants::match_default | regex_constants::format_sed);
}

int substr_occurrence(string str, string sub_str)
{
   int occurrence = 0;
   for (int i = 0; i < str.length(); i++)
   {
      if (str.substr(i, sub_str.length()) == sub_str)
      {
         occurrence++;
      }
   }

   return occurrence;
}

string read_file_content(const string &path)
{
   ifstream input_file(path);
   if (!input_file.is_open())
   {
      cerr << "Could not open the file - '"
           << path << "'" << endl;
      exit(EXIT_FAILURE);
   };
   return string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
}

class {

}
int main()
{
   YAML::Node yaml = YAML::LoadFile("config.yaml");

   for (YAML::const_iterator it = yaml.begin(); it != yaml.end(); ++it) {
      auto fileName = it->first.as<string>();
      cout << "fileName:" << fileName << endl;
      auto b = it->second.as<std::vector<std::map<string, string>>>();
      for (int i = 0; i < b.size(); i++)
      {
         
         cout << "haystack:" << b.at(i)["haystack"] <<endl;
         cout << "needle:" << b.at(i)["needle"] <<endl;
         cout << "occurrence:" << b.at(i)["occurrence"] <<endl;
      }
   }


   cout << "==================";

   const vector<array<string, 3>> config = {
       {"project(\"AMA2104-playground\")",
        "AMA2104-playground",
        "1"},
       {"set\\(CMAKE_CXX_STANDARD 17\\)",
        "17",
        "1"}};

   std::string s = read_file_content("CMakeLists.txt");

   for (size_t i = 0; i < config.size() - 1; i++)
   {
      auto current = config[i].at(0);
      cout << current << ":" << substr_occurrence(s, current) << endl;
   }

   std::cout << "++++++++++++++++++++replacing" << std::endl;

   boost::regex e2;
   e2.assign("(" + escape_string_for_regex(config.at(0).at(0)) + ")|(def)");
   //"(?1<1>$&</1>)
   auto src = escape_string_for_regex(config.at(0).at(0));

   auto new_content = src.replace(src.find(config.at(0).at(1)), config.at(0).at(1).length(), "ypypoypoy");

   string format_string = "(?1" + new_content + ")(?2<2>$&<2>)";

   auto c = boost::regex_replace(s, e2, format_string.c_str(), boost::match_default | boost::format_all);

   std::cout << c;
   std::cin.get();
   return 0;
}