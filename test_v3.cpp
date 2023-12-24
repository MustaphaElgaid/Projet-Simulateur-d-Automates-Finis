
#include "automata.hpp"
#include <cassert>

automata::Automata start_ab() {
    automata::Automata test;

    std::size_t etat1 = test.new_node(); 
    std::size_t etat2 = test.new_node(); 
    std::size_t etat3 = test.new_node();

    test.new_arc(0, etat1, 'a'); 
    test.new_arc(etat1, etat2, 'b'); 
    test.new_arc(etat2, etat2, 'a'); 
    test.new_arc(etat2, etat3, 'b'); 
    test.new_arc(etat3, etat2, 'a'); 
    test.new_arc(etat3, etat3, 'b'); 
    test.new_terminal(etat2);
    test.new_terminal(etat3);

    return test;
}


void start_ab_check() {
    automata::Automata automata = start_ab();

    assert(automata.parse("abba") == true);
    assert(automata.parse("baba") == false);
    assert(automata.parse("abcba") == false);
}
