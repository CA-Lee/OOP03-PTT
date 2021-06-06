#include "User.h"

class Member : public User
{
public:
    Member() {}
    Member(string name, string pass)
        : User(name, pass, Privilege::Member) {}
};
