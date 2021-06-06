#pragma once
#include <vector>
#include <string>
#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"
#include "boost/serialization/vector.hpp"
#include "boost/serialization/string.hpp"
#include "User.h"
#include "Admin.h"
#include "Member.h"
#include "Board.h"
#include "Viewer.h"
#include "Post.h"

using std::string;
using std::vector;

class BoardManager
{
    vector<User> user_list;
    vector<Board> board_list;
    Viewer viewer;

    //user_state
    User *current_user;
    Board *current_board;
    Post *current_post;

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar &board_list;
        ar &user_list;
        ar &current_user;
        ar &current_board;
        ar &current_post;
    }

public:
    BoardManager();

    const vector<Board> &get_board_list() { return board_list; }
    const Board &get_current_board() { return *current_board; }
    // const User &get_current_user() { return *current_user; }
    // const Post &get_current_post() { return *current_post; }

    void start();

    void login(string user_id, string password);
    void logout();
    bool sign_up(string user_id, string password);
    vector<User>::iterator find_user(string user_id);

    vector<Board>::iterator find_board(string board_id);
    void add_board(string board_id);
    void select_board(string board_id);
    void delete_board(string board_id);

    void add_post(string board_id, string title, string content);
    void select_post(string post_id);
    void delete_post(string post_id);

    void add_comment(Weight w, string s);

    void send_mail(string to, string content);
    void check_mail();
};

BOOST_CLASS_TRACKING(BoardManager, boost::serialization::track_always)
