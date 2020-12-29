#include <iostream>  
#include <map>  
#include <string> 
 
int main() { 
  std::map<std::string, std::string> m = { 
    { "k1", "v1" },  
    { "k2", "v2" }, 
    { "k3", "v3" } 
  }; 
  for (auto& [k, v] : m) { 
    std::cout << k << '=' << v << '\n'; 
  } 
} 
