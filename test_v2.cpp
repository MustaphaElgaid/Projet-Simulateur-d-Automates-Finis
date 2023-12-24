#include "pilelivre.hpp"



namespace pilelivre {


PileLivre_v2 first_day() {
    PileLivre_v2 pl;

    pl.add_person("Mustapha");
    pl.add_person("Mohammed");
    pl.add_person("Youssef");

    pl.add_msg_public("Mustapha", "Bonjour");
    pl.add_msg_private("Mohammed", "youssef", "Bonjour Youssef ");
    pl.add_cat("Mustapha", "image");
    pl.add_friend("Mustapha", "Mohammed");
    pl.add_friend("Mustapha", "Youssef");
    
    return pl;
}
} 
