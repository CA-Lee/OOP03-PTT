#pragma once
#include <string>
#include <vector>
#include "boost/serialization/access.hpp"
#include "boost/serialization/string.hpp"

using std::string;
using std::vector;

class Post;

enum class Privilege
{
    Guest,
    Member,
    Admin
};

struct Mail
{
    string from;
    string content;
    Mail() {}
    Mail(string a, string b)
        : from(a), content(b) {}

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar &from &content;
    }
};

class User
{
    string passwd;
    Privilege privilege;
    vector<Post *> my_post_list;
    vector<Mail> mail_box;

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar &const_cast<string &>(id) & passwd &privilege &my_post_list &mail_box;
    }

public:
    const string id;

    User() {}
    User(string _username, string _password, Privilege _privilege);

    bool verify(string user_id, string _passwd)
    {
        return (user_id == id && _passwd == passwd);
    }
    Privilege get_privilege() { return privilege; }
    void add_post(Post *p);
    void delete_post(Post *p);
    void add_mail(string from, string content);
    const vector<Mail> get_mail() const { return mail_box; }
};
