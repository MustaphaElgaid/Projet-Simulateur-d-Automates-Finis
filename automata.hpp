#include "graph.hpp"
#include <unordered_set>
#include <cstddef> 

namespace automata {

struct Automata {


    graph::Graph<std::size_t, void, char> g;          
    std::size_t counter_curr;                         
    std::unordered_set<std::size_t> terminal;        
    Automata(); 
    std::size_t new_node(); 
    void new_arc(std::size_t pred, std::size_t succ, char c); 
    void new_terminal(std::size_t node); 
    bool parse(const std::string& word);
    
    
};

}





 
    


