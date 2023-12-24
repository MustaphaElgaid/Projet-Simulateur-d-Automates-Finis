#include "automata.hpp"
#include <iostream>

namespace automata {



Automata::Automata() : g(), counter_curr(1), terminal() {
    g.add_node(0);
}
 
std::size_t Automata::new_node() {
    std::size_t new_node_id = counter_curr++;  
    g.add_node(new_node_id);                   
    return new_node_id;                       
}


void Automata::new_arc(std::size_t pred, std::size_t succ, char c) {
    if (pred >= counter_curr || succ >= counter_curr) {
        throw std::invalid_argument("Un des nodes n'existe pas.");
    }
    auto it = g.m_edges.find(std::make_pair(pred, succ));
    if (it != g.m_edges.end() && it->second == c) {
        throw std::invalid_argument("un arc existe entre ces deux nodes.");
    }
    g.add_edge(pred, succ, c);
}

void Automata::new_terminal(std::size_t node) {
    if (node >= counter_curr) {
        throw std::invalid_argument("node n'existe pas.");
    }
    terminal.insert(node);
}

bool Automata::parse(const std::string& word) {
    std::size_t state = 0; 
    for (char c : word) {
        bool transitionFound = false;
        
        for (const auto& edge : g.m_edges) {
            if (edge.first.first == state && edge.second == c) {
                state = edge.first.second; 
                transitionFound = true;
                break;
            }
        }
        if (!transitionFound) 
            return false;   
    }
    return terminal.find(state) != terminal.end();
}

} 










