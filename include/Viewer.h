#pragma once
#include <string>
#include <vector>

using std::string;
using std::vector;

struct Command
{
    string id;
    vector<string> args;
};

class BoardManager;
class Board;
class Post;
class Mail;

class Viewer
{
    BoardManager &board_manager;

public:
    Viewer(BoardManager *);

    void start();
    Command read_cmd();
    bool run_cmd(Command cmd);

    void render_menu();
    void render_board_list();
    void render_board(const Board &target_board);
    void render_post(const Post &target_post);
    void render_edit_post(const Post &origin_post);
    void render_mail(vector<Mail> mail_list);

    void render_login_success();
    void render_login_fail();
    void render_help();
    void render_permission_denied();
    void render_board_not_found();
    void render_account_already_exist();
    void render_board_already_exist();
    void render_post_not_found();
    void render_add_post_failed();
    void render_no_current_board();
    void render_no_current_post();
    void render_user_not_found();
    void render_no_mail();
};
