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
   const regex esc("[.^$|:()\\[\\]{}*+?\"\\\\]");
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
   // ifstream input_file(path);
   // if (!input_file.is_open())
   // {
   //    cerr << "Could not open the file - '"
   //         << path << "'" << endl;
   //    exit(EXIT_FAILURE);
   // };
   // return string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
   std::ifstream input_file(path);
   if (!input_file.is_open())
   {
      cerr << "Could not open the file - '"
           << path << "'" << endl;
      exit(EXIT_FAILURE);
   };

   std::stringstream strStream;
   strStream << input_file.rdbuf(); // read the file
   return strStream.str();          // str holds the content of the file
}

struct placeholder
{
   string haystack;
   string needle;
   string occurrence;
};

struct entry
{
   string file_name;
   vector<placeholder> placeholder_vector;
};

class init_file_loader
{
private:
   std::vector<entry> data;

public:
   init_file_loader(const string file_name = "initfile.yaml")
   {
      YAML::Node yaml = YAML::LoadFile(file_name);

      for (YAML::const_iterator it = yaml.begin(); it != yaml.end(); ++it)
      {
         vector<placeholder> placeholder_vector;

         auto temp = it->second.as<std::vector<std::map<string, string>>>();

         for (auto it2 = temp.cbegin(); it2 != temp.cend(); ++it2)
         {
            std::map<string, string> map = *it2;
            placeholder p = {
               haystack : map["haystack"],
               needle : map["needle"],
               occurrence : map["occurrence"]
            };

            placeholder_vector.push_back(p);
         }

         entry e = {
            file_name : it->first.as<string>(),
            placeholder_vector : placeholder_vector
         };

         data.push_back(e);
      }
   }

   std::vector<entry> get_data()
   {
      return data;
   }
};

int main()
{
   init_file_loader loader;
   auto data = loader.get_data();

   string temp;

   for (int i = 0; i < data.size(); i++)
   {
      // TODO
      bool pass = false;

      auto e = data.at(i);
      std::string s = read_file_content(e.file_name);
      stringstream ss_expr;
      stringstream ss_format;
      for (size_t i = 0; i < e.placeholder_vector.size(); i++)
      {
         auto current = e.placeholder_vector.at(i);
         cout << current.haystack << ":" << substr_occurrence(s, current.haystack) << endl;
         // TODO: occurance check

         auto escaped = escape_string_for_regex(current.haystack);
         auto replacement = escaped;
         ss_expr << "(" + escaped + ")";
         auto pos = current.haystack.find(current.needle);
         cout << "needle:" << current.needle;
         if (pos == std::string::npos)
         {
            cerr << "invalid";
            return EXIT_FAILURE;
         }
         ss_format << "(?" << (i + 1) << escape_string_for_regex(current.haystack.replace(pos, current.needle.length(), "ypypoypoy")) << ")";

         if (i < e.placeholder_vector.size() - 1)
         {
            ss_expr << "|";
         }
      }

      boost::regex e2(ss_expr.str());
      
      string format = ss_format.str();
      cout << "format:" << format << endl;
      auto c = boost::regex_replace(s, e2, format.c_str(), boost::match_default | boost::format_all);
      std::cout << c;
      std::ofstream out(e.file_name);
      out << c;
      out.close();
   }
   cout << "complete." << endl;
   std::cin.get();
   return 0;
}