#include <fstream>
#include <sstream>
#include <string>
#include <iterator>
#include <regex>
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <iomanip>
#include <boost/regex.hpp>

using namespace std;

string escape_string_for_regex(string str) {
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

int main()
{
   
   const vector<array<string, 3>> config = {
       {"project(\"AMA2104-playground\")",
        "AMA2104-playground",
        "1"},
       {"set\\(CMAKE_CXX_STANDARD 17\\)",
        "17",
        "1"}};

   // std::stringstream ss;
   // for (size_t i = 0; i < config.size() - 1; ++i)
   // {
   //    ss << "(" << config[i].at(0) << ")"
   //       << "|";
   // }
   // ss << "(" << config[config.size() - 1].at(0) << ")";

   // string myexpression = ss.str();
   // cout << myexpression << endl;

   //std::string s = "project(\"hello world\")\ndefproject(\"hello world\") def";
   std::string s = read_file_content("CMakeLists.txt");

   for (size_t i = 0; i < config.size() - 1; i++)
   {
      auto current = config[i].at(0);
      cout << current << ":" << substr_occurrence(s, current) << endl;
   }

   // std::match_results<std::string::const_iterator> what;
   // std::regex_constants::match_flag_type flags = std::regex_constants::match_default;

   // cout << escape_string_for_regex(config.at(0).at(0)) << endl;
   // std::regex ssss("("+ escape_string_for_regex(config.at(0).at(0)) + ")");

   // std::string::const_iterator searchStart(s.cbegin());
   // smatch res;
   // while (regex_search(searchStart, s.cend(), res, ssss))
   // {
   //    std::cout << "!" << res[0] << std::endl;
   //    searchStart = res.suffix().first;
   // }

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

// // n placeholder, each must have only one occurance
// #include <iostream>
// #include <string>
// #include <map>
// #include <boost/regex.hpp>

// purpose:
// takes the contents of a file in the form of a string
// and searches for all the C++ class definitions, storing
// their locations in a map of strings/int's
// typedef std::map<std::string, int, std::less<std::string> > map_type;

// boost::regex expression(
//    "^(template[[:space:]]*<[^;:{]+>[[:space:]]*)?"
//    "(class|struct)[[:space:]]*"
//    "(\\<\\w+\\>([[:blank:]]*\\([^)]*\\))?"
//    "[[:space:]]*)*(\\<\\w*\\>)[[:space:]]*"
//    "(<[^;:{]+>[[:space:]]*)?(\\{|:[^;\\{()]*\\{)");

// void IndexClasses(map_type& m, const std::string& file)
// {
//    std::string::const_iterator start, end;
//    start = file.begin();
//    end = file.end();

//    boost::match_results<std::string::const_iterator> what;
//    boost::match_flag_type flags = boost::match_default;
//    while(regex_search(start, end, what, expression, flags))
//    {
//       what[0] contains the whole string
//       what[5] contains the class name.
//       what[6] contains the template specialisation if any.
//       add class name and position to map:
//       auto q = what[5].first;
//       auto w =  what[5].second;
//       auto e = what[6].first;
//       auto r = what[6].second;

//       m[std::string(q, w) + std::string(e, r)] = q - file.begin();
//       update search position:
//       start = what[0].second;
//       // update flags:
//       flags |= boost::match_prev_avail;
//       flags |= boost::match_not_bob;
//    }
// }

// int main() {
//    map_type a;

//    IndexClasses(a, "project(\"hello-world\"");
//    IndexClasses(a, "class a{}; class b{};");

//    std::cin.get();
//    return 0;
// }