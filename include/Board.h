#pragma once
#include <string>
#include <vector>
#include <list>
#include "Post.h"

using std::list;
using std::string;
using std::vector;

class Board
{
    list<Post> post_list;
    string id;
    int viewed;

public:
    Board();
    Board(string id);

    string get_id() const { return id; }
    const list<Post> get_post_list() const { return post_list; }
    list<Post>::iterator find_post(string post_id);
    Post *add_post(string title, string author, string content);
    void delete_post(string post_id);
    void view() { viewed++; }
    int get_view() const { return viewed; }
};
