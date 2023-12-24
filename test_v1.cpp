#include "pilelivre.hpp"

pilelivre::PileLivre_v1 first_day() {
    pilelivre::PileLivre_v1 res;

    res.add_person("James");
    res.add_msg_public("James", "Yoh, Salut PileLivre !!!");
    res.add_person("Jack");
    res.add_friend("James", "Jack");
    res.add_msg_private("Jack", "James", "t’es qui???");
    res.rm_friend("Jack", "James");
    return res;
}


