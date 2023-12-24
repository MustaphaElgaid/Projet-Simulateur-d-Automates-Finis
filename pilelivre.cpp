#include "pilelivre.hpp"
#include <iostream>
#include <string>
#include <algorithm>
namespace pilelivre{


Person::Person(const std::string& name) : m_name(name) {}



void Person::add_action(Action* action) {
    m_action.push_back(action);
// on peut faire la meme chose avec enum class et faire un switch pour determiner quelle type d'action
    if (dynamic_cast<FriendAdd*>(action) != nullptr) {
        m_friends.push_back(static_cast<FriendAdd*>(action)->m_person);
    } else if (dynamic_cast<FriendRemove*>(action) != nullptr) {
        m_friends.remove(static_cast<FriendRemove*>(action)->m_person);
    }
}



MsgPublic::MsgPublic(const std::string& msg) : m_msg(msg) {}

MsgPrivate::MsgPrivate(const std::string& msg, Person* dest) : m_msg(msg), m_dest(dest) {}

CatPropaganda::CatPropaganda(const std::string& url) : m_url(url) {}

FriendAdd::FriendAdd(Person* person) : m_person(person) {}

FriendRemove::FriendRemove(Person* person) : m_person(person) {}

//------------------------------------------------

PileLivre_v1::PileLivre_v1() : m_data(), m_content(), m_counter_msg(0), m_counter_cat(0), m_counter_friend(0) {}



void PileLivre_v1::add_person(const std::string& name) {
    Person* p = new Person(name);
    m_data.push_back(p);
    m_content.add_node(name,p);
}


void PileLivre_v1::add_msg_public(const std::string& name, const std::string& msg) {
    std::string id_act = "action/msg/" + std::to_string(m_counter_msg++);
    MsgPublic* messg = new MsgPublic(msg); 

    m_content.add_node(id_act, messg);
    m_content.add_edge(name, id_act, true);
    
    
    // ajouter le message a la liste d'actions de la personne
    auto it = std::find_if(m_data.begin(), m_data.end(), [&name](Data* data) {
        Person* person = dynamic_cast<Person*>(data);
        return person && person->m_name == name;
    });

    if (it != m_data.end()) {
        Person* person = dynamic_cast<Person*>(*it);
        if (person) {
            person->add_action(messg);
        }
    }

    m_data.push_back(messg);
}





void PileLivre_v1::add_msg_private(const std::string& writer, const std::string& reader, const std::string& msg) {
    
    // Chercher le destinataire 
    auto it = std::find_if(m_data.begin(), m_data.end(), [&reader](Data* data) {
        Person* person = dynamic_cast<Person*>(data);
        return person && person->m_name == reader;
    });

    std::string id_act = "action/msg/" + std::to_string(m_counter_msg++);
    MsgPrivate* messg = new MsgPrivate(msg, nullptr); // au debut * dest = nullptr , apres la recuperation on va definir m_dest

    if (it != m_data.end()) {
        messg->m_dest = dynamic_cast<Person*>(*it); 
    }

    // Chercher Writer et Ajouter le message à sa liste d'actions
    auto writer_it = std::find_if(m_data.begin(), m_data.end(), [&writer](Data* data) {
        Person* person = dynamic_cast<Person*>(data);
        return person && person->m_name == writer;
    });

    if (writer_it != m_data.end()) {
        Person* person = dynamic_cast<Person*>(*writer_it);
        if (person) {
            person->add_action(messg);
        }
    }
    
    m_content.add_node(id_act, messg);
    m_content.add_edge(writer, id_act, true);

    m_data.push_back(messg);
}




void PileLivre_v1::add_cat(const std::string& name, const std::string& url) {
    CatPropaganda* cat = new CatPropaganda(url);
    
    std::string id_act = "action/cat/" + std::to_string(m_counter_cat++);
    // ajouter les donnees au graphe et aussi a la liste 
    m_content.add_node(id_act, cat);
    m_content.add_edge(name, id_act, true);
    m_data.push_back(cat);

    auto user_it = std::find_if(m_data.begin(), m_data.end(), [&name](Data* data) {
        Person* person = dynamic_cast<Person*>(data);
        return person && person->m_name == name;
    });

    if (user_it != m_data.end()) {
        Person* person = dynamic_cast<Person*>(*user_it);
        if (person) {
            person->add_action(cat);
        }
    }
}


//---------------------------------------------------------


void PileLivre_v1::add_friend(const std::string& name, const std::string& friend_new) {


        Person* new_frd = nullptr;  // initialise new_friend par nullptr, pour chercher friend_new person
        for (Data* data : m_data) { // for each pour parcourir Data
            Person* person = dynamic_cast<Person*>(data);
                if (person && person->m_name == friend_new) {
                new_frd = person;
                break;
            }
        }
        
        std::string id_act = "action/friend/" + std::to_string(m_counter_friend++);

        if (new_frd) {
            FriendAdd* f = new FriendAdd(new_frd);
            m_data.push_back(f);

            m_content.add_node(id_act,f);

            // Ajouter les donnees
            if (m_content.has_node(name) && m_content.has_node(id_act)) {
                m_content.add_edge(name,id_act, true);
            }

            // Ajouter l'action à la personne spécifiée
            auto user_it = std::find_if(m_data.begin(), m_data.end(), [&name](Data* data) {
                Person* person = dynamic_cast<Person*>(data);
                return person && person->m_name == name;
            });

            if (user_it != m_data.end()) {
                Person* person = dynamic_cast<Person*>(*user_it);
                if (person) {
                    person->add_action(f);
                }
            }
        }

    
}


//---------------------------------------------------------

void PileLivre_v1::rm_friend(const std::string& name, const std::string& friend_nomore) {

        Person* friend_nmr = nullptr;  // initialise friend_nomore par nullptr, pour chercher friend_nomore person
        for (Data* data : m_data) {       // for each pour parcourir Data
            Person* person = dynamic_cast<Person*>(data);
            if (person && person->m_name == friend_nomore) {
                friend_nmr = person;
                break;
            }
        }

        if (friend_nmr) {

                FriendRemove* frm = new FriendRemove(friend_nmr);
                m_counter_friend++;
                m_data.push_back(frm);

                std::string id_act = "action/friend/" + std::to_string(m_counter_friend - 1);
                
                m_content.add_node(id_act, frm);
                m_content.add_edge(name, id_act, true);

                auto user_it = std::find_if(m_data.begin(), m_data.end(), [&name](Data* data) {
                    Person* person = dynamic_cast<Person*>(data);
                    return person && person->m_name == name;
                });

                if (user_it != m_data.end()) {
                    Person* person = dynamic_cast<Person*>(*user_it);
                    if (person) {
                        person->add_action(frm);
                    }
                }
            }
        }

//----------------------------------------------------------




PileLivre_v2::PileLivre_v2() : m_content(), m_data(), m_counter_msg(0), m_counter_cat(0), m_counter_friend(0) {}

void PileLivre_v2::add_person(const std::string& name) {
    auto* person = new Person(name);
    m_data.push_back(person);
    m_content.add_node(name, person);
}

void PileLivre_v2::add_msg_public(const std::string& name, const std::string& msg) {
    std::string id_act = "action/msg/" + std::to_string(m_counter_msg++);
    MsgPublic* messg = new MsgPublic(msg); 

    m_content.add_node(id_act, messg);
    m_content.add_edge(name, id_act);
    
    
    // ajouter le message a la liste d'actions de la personne
    auto it = std::find_if(m_data.begin(), m_data.end(), [&name](Data* data) {
        Person* person = dynamic_cast<Person*>(data);
        return person && person->m_name == name;
    });

    if (it != m_data.end()) {
        Person* person = dynamic_cast<Person*>(*it);
        if (person) {
            person->add_action(messg);
        }
    }

    m_data.push_back(messg);
}





void PileLivre_v2::add_msg_private(const std::string& writer, const std::string& reader, const std::string& msg) {
    
    // Chercher le destinataire 
    auto it = std::find_if(m_data.begin(), m_data.end(), [&reader](Data* data) {
        Person* person = dynamic_cast<Person*>(data);
        return person && person->m_name == reader;
    });

    std::string id_act = "action/msg/" + std::to_string(m_counter_msg++);
    MsgPrivate* messg = new MsgPrivate(msg, nullptr); // au debut * dest = nullptr , apres la recuperation on va definir m_dest

    if (it != m_data.end()) {
        messg->m_dest = dynamic_cast<Person*>(*it); 
    }

    // Chercher Writer et Ajouter le message à sa liste d'actions
    auto writer_it = std::find_if(m_data.begin(), m_data.end(), [&writer](Data* data) {
        Person* person = dynamic_cast<Person*>(data);
        return person && person->m_name == writer;
    });

    if (writer_it != m_data.end()) {
        Person* person = dynamic_cast<Person*>(*writer_it);
        if (person) {
            person->add_action(messg);
        }
    }
    
    m_content.add_node(id_act, messg);
    m_content.add_edge(writer, id_act);

    m_data.push_back(messg);
}




void PileLivre_v2::add_cat(const std::string& name, const std::string& url) {
    CatPropaganda* cat = new CatPropaganda(url);
    
    std::string id_act = "action/cat/" + std::to_string(m_counter_cat++);
    // ajouter les donnees au graphe et aussi a la liste 
    m_content.add_node(id_act, cat);
    m_content.add_edge(name, id_act);
    m_data.push_back(cat);

    auto user_it = std::find_if(m_data.begin(), m_data.end(), [&name](Data* data) {
        Person* person = dynamic_cast<Person*>(data);
        return person && person->m_name == name;
    });

    if (user_it != m_data.end()) {
        Person* person = dynamic_cast<Person*>(*user_it);
        if (person) {
            person->add_action(cat);
        }
    }
}


//---------------------------------------------------------


void PileLivre_v2::add_friend(const std::string& name, const std::string& friend_new) {


        Person* new_frd = nullptr;  // initialise new_friend par nullptr, pour chercher friend_new person
        for (Data* data : m_data) { // for each pour parcourir Data
            Person* person = dynamic_cast<Person*>(data);
                if (person && person->m_name == friend_new) {
                new_frd = person;
                break;
            }
        }
        
        std::string id_act = "action/friend/" + std::to_string(m_counter_friend++);

        if (new_frd) {
            FriendAdd* f = new FriendAdd(new_frd);
            m_data.push_back(f);

            m_content.add_node(id_act,f);

            // Ajouter les donnees
            if (m_content.has_node(name) && m_content.has_node(id_act)) {
                m_content.add_edge(name,id_act);
            }

            // Ajouter l'action à la personne spécifiée
            auto user_it = std::find_if(m_data.begin(), m_data.end(), [&name](Data* data) {
                Person* person = dynamic_cast<Person*>(data);
                return person && person->m_name == name;
            });

            if (user_it != m_data.end()) {
                Person* person = dynamic_cast<Person*>(*user_it);
                if (person) {
                    person->add_action(f);
                }
            }
        }

    
}


//---------------------------------------------------------

void PileLivre_v2::rm_friend(const std::string& name, const std::string& friend_nomore) {

        Person* friend_nmr = nullptr;  // initialise friend_nomore par nullptr, pour chercher friend_nomore person
        for (Data* data : m_data) {       // for each pour parcourir Data
            Person* person = dynamic_cast<Person*>(data);
            if (person && person->m_name == friend_nomore) {
                friend_nmr = person;
                break;
            }
        }

        if (friend_nmr) {

                FriendRemove* frm = new FriendRemove(friend_nmr);
                m_counter_friend++;
                m_data.push_back(frm);

                std::string id_act = "action/friend/" + std::to_string(m_counter_friend - 1);
                
                m_content.add_node(id_act, frm);
                m_content.add_edge(name, id_act);

                auto user_it = std::find_if(m_data.begin(), m_data.end(), [&name](Data* data) {
                    Person* person = dynamic_cast<Person*>(data);
                    return person && person->m_name == name;
                });

                if (user_it != m_data.end()) {
                    Person* person = dynamic_cast<Person*>(*user_it);
                    if (person) {
                        person->add_action(frm);
                    }
                }
            }
        }

} 
