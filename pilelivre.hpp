#ifndef PILELIVRE_HPP
#define PILELIVRE_HPP
#include <string>
#include <list>
#include <iostream>
#include "graph.hpp"
#include <memory>
namespace pilelivre{


class Data {
    public :
    virtual ~Data() {}
};
class Action : public Data {
    public:
    virtual ~Action() {}
};

class Person : public Data {
public:
    Person(const std::string& name);
    void add_action(Action* action);
    std::string m_name;
    std::list<Action*> m_action;
    std::list<Person*> m_friends;
};



class MsgPublic : public Action {
public:
    MsgPublic(const std::string& msg);
    std::string m_msg;
};

class MsgPrivate : public Action {
public:
    MsgPrivate(const std::string& msg, Person* dest);
    std::string m_msg;
    Person* m_dest;
};

class CatPropaganda : public Action {
public:
    CatPropaganda(const std::string& url);
    std::string m_url;
};

class FriendAdd : public Action {
public:
    FriendAdd(Person* person);
    Person* m_person;
};

class FriendRemove : public Action {
public:
    FriendRemove(Person* person);
    Person* m_person;
};


struct PileLivre_v1 {  

    graph::Graph<std::string, Data*, bool> m_content;
    std::list<Data*> m_data;
    std::size_t m_counter_msg;
    std::size_t m_counter_cat;
    std::size_t m_counter_friend;

    PileLivre_v1();
    void add_person(const std::string& name);
    void add_msg_public(const std::string& name, const std::string& msg);
    void add_msg_private(const std::string& writer, const std::string& reader, const std::string& msg);
    void add_cat(const std::string& name, const std::string& url);
    void add_friend(const std::string& name, const std::string& friend_new);
    void rm_friend(const std::string& name, const std::string& friend_nomore);
};


struct PileLivre_v2 {
    graph::Graph<std::string, Data*, void> m_content;
    std::list<Data*> m_data;
    std::size_t m_counter_msg;
    std::size_t m_counter_cat;
    std::size_t m_counter_friend;

    PileLivre_v2();
    void add_person(const std::string& name);
    void add_msg_public(const std::string& name, const std::string& msg);
    void add_msg_private(const std::string& writer, const std::string& reader, const std::string& msg);
    void add_cat(const std::string& name, const std::string& url);
    void add_friend(const std::string& name, const std::string& friend_new);
    void rm_friend(const std::string& name, const std::string& friend_nomore);
};


}

#endif