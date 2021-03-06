#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include "Viewer.h"
#include "BoardManager.h"
#include "Board.h"
#include "Post.h"
#include "User.h"
#include "Game.h"

using std::cin;
using std::cout;
using std::endl;
using std::fstream;
using std::getline;
using std::left;
using std::right;
using std::setw;
using std::string;
using std::ws;

string get_line()
{
    fflush(stdin);
    cin.clear();
    string t;
    getline(cin, t);
    cin.clear();
    return t;
}

string get_multiline()
{
    string all, line;
    cin >> ws;
    cin.clear();
    while (getline(cin, line))
    {
        all += line + "\n";
    }
    cin.clear();
    return all;
}

Viewer::Viewer(BoardManager *bm) : board_manager(*bm)
{
}

void Viewer::start()
{
    render_menu();
    render_help();
    bool cont = true;
    while (cont)
    {
        Command cmd = read_cmd();
        cont = run_cmd(cmd);
    }
}

Command Viewer::read_cmd()
{
    cout << "$ ";
    string s = get_line();

    vector<string> args;
    while (s.find(' ') != -1)
    {
        args.push_back(s.substr(0, s.find(' ')));
        s = s.substr(s.find(' ') + 1);
    }
    args.push_back(s);

    Command cmd;
    cmd.id = args[0];
    args.erase(args.begin());
    cmd.args = args;

    return cmd;
}

bool Viewer::run_cmd(Command cmd)
{
    if (cmd.id == "logout")
    {
        board_manager.logout();
    }
    else if (cmd.id == "boardlist" && cmd.args.size() == 0)
    {
        render_board_list();
    }
    else if (cmd.id == "readboard" && cmd.args.size() == 1)
    {
        board_manager.select_board(cmd.args[0]);
    }

    else if (cmd.id == "addboard" && cmd.args.size() == 1)
    {
        board_manager.add_board(cmd.args[0]);
    }
    else if (cmd.id == "delboard" && cmd.args.size() == 1)
    {
        board_manager.delete_board(cmd.args[0]);
    }
    else if (cmd.id == "readpost" && cmd.args.size() == 1)
    {
        board_manager.select_post(cmd.args[0]);
    }
    else if (cmd.id == "addpost" && cmd.args.size() == 0)
    {
        string board, title, content, temp;
        cout << "Select board: ";
        cin >> board;
        cout << "Enter the title: ";
        title = get_line();
        cout << "Enter contents, exit with EOF:" << endl;
        content = get_multiline();
        board_manager.add_post(board, title, content);
    }
    else if (cmd.id == "delpost" && cmd.args.size() == 1)
    {
        board_manager.delete_post(cmd.args[0]);
    }
    else if (cmd.id == "edpost" && cmd.args.size() == 1)
    {
        board_manager.edit_post(cmd.args[0]);
    }
    else if (cmd.id == "addcomment" && cmd.args.size() == 0)
    {
        string v;
        do
        {

            cout << "[push/pull/comment]: ";
            cin >> v;
        } while (!(v == "push" || v == "pull" || v == "comment"));
        Weight w;
        if (v == "push")
            w = Weight::push;
        else if (v == "pull")
            w = Weight::dislike;
        else if (v == "comment")
            w = Weight::comment;
        cout << "Enter comment: " << endl;
        string s = get_line();
        board_manager.add_comment(w, s);
    }
    else if (cmd.id == "exit" && cmd.args.size() == 0)
    {
        return false;
    }
    else if (cmd.id == "mailto" && cmd.args.size() == 1)
    {
        cout << "Enter the contents, exit with EOF:" << endl;
        string s = get_multiline();
        board_manager.send_mail(cmd.args[0], s);
    }
    else if (cmd.id == "mailbox" && cmd.args.size() == 0)
    {
        board_manager.check_mail();
    }
    else if (cmd.id == "game" && cmd.args.size() == 0)
    {
        game();
    }
    else
    {
        render_help();
    }
    return true;
}

void Viewer::render_help()
{
    cout << endl
         << "Command tutorial" << endl
         << "================" << endl
         << setw(20) << left << "logout"
         << "logout from this account" << endl
         << setw(20) << left << "exit"
         << "exit program" << endl
         << "-----------------" << endl
         << setw(20) << left << "boardlist"
         << "view all board" << endl
         << setw(20) << left << "readboard [board id]"
         << "enter one board" << endl
         << setw(20) << left << "addboard [board id]"
         << "add new board" << endl
         << setw(20) << left << "delboard [board id]"
         << "delete a board" << endl
         << "-----------------" << endl
         << setw(20) << left << "readpost [post id]"
         << "read one post" << endl
         << setw(20) << left << "addpost"
         << "add new post" << endl
         << setw(20) << left << "delpost [post id]"
         << "delete a post" << endl
         << setw(20) << left << "edpost [post id]"
         << "edit your post" << endl
         << setw(20) << left << "addcomment"
         << "add comment to a post" << endl
         << "-----------------" << endl
         << setw(20) << left << "mailbox"
         << "open mailbox" << endl
         << setw(20) << left << "mailto [user_id]"
         << "send mail to others" << endl
         << setw(20) << left << "game"
         << "play a game" << endl;
}

void Viewer::render_menu()
{
    string username, password;
    cout << "Menu" << endl
         << "====" << endl;
    fstream f("the_simpsons.txt");
    string buf;
    while (getline(f, buf))
        cout << buf << endl;
    f.close();
    cout << "Enter user id(`new` for register): ";
    cin >> username;
    if (username == "new")
    {
        do
        {
            cout << "Enter user id:";
            cin >> username;
            cout << "Enter password:";
            cin >> password;
        } while (!board_manager.sign_up(username, password));
        board_manager.login(username, password);
    }
    else
    {
        cout << "Enter password: ";
        cin >> password;
        //guest
        board_manager.login(username, password);
    }
}

void Viewer::render_board_list()
{
    cout << endl
         << "Board List" << endl
         << "==========" << endl;
    for (auto &board : board_manager.get_board_list())
    {
        cout << "> " << board.get_id() << endl;
    }
}

void Viewer::render_board(const Board &target_board)
{
    cout << "Board: " << target_board.get_id() << endl
         << "????????????: " << target_board.get_view() << endl
         << "|" << setw(4) << "??????"
         << "|" << setw(5) << "id"
         << "|"
         << setw(50) << left << "title"
         << "|"
         << setw(15) << right << "author"
         << "|" << endl
         << "|" << std::setfill('=') << setw(77) << "="
         << "|" << endl
         << std::setfill(' ');
    for (auto &post : target_board.get_post_list())
    {
        cout << "|" << setw(4) << (post.get_hot() > 3 ? "???" : to_string(post.get_hot()))
             << "|" << setw(5) << post.bsid << "|"
             << setw(50) << left << post.get_title() << "|"
             << setw(15) << right << post.author_id << "|" << endl;
    }
}

void Viewer::render_post(const Post &target_post)
{
    cout << "Board: " << board_manager.get_current_board().get_id() << endl
         << "Post ID: " << target_post.bsid << endl
         << "Title: " << target_post.get_title() << endl
         << "Author: " << target_post.author_id << endl
         << "---" << endl
         << endl
         << target_post.get_content() << endl
         << "---"
         << endl;
    for (auto &c : target_post.get_comments())
    {
        cout << setw(4) << left;
        if (c.weight == Weight::push)
            cout << "???";
        else if (c.weight == Weight::dislike)
            cout << "???";
        else if (c.weight == Weight::comment)
            cout << "->";
        cout << c.author << ": ";
        cout << c.content << endl;
    }
}

void Viewer::render_edit_post(const Post &origin_post)
{
    cout << "New title (leave empty to keep origin `" << origin_post.get_title() << "`): ";
    string newtitle = get_line();
    if (newtitle == "")
        newtitle = origin_post.get_title();
    cout << "New content, exit with EOF (leave empty to keep origin):" << endl;
    string new_content = get_multiline();
    if (new_content == "")
        new_content = origin_post.get_content();
    board_manager.edit_post(origin_post.bsid, newtitle, new_content);
}

void Viewer::render_mail(const vector<Mail> mail_list)
{
    cout << "Mail List" << endl
         << "=========" << endl
         << setw(15) << "Sender"
         << " | "
         << "Content" << endl;
    for (auto m : mail_list)
    {
        cout << setw(15) << m.from << " | " << m.content << endl;
    }
}

void Viewer::render_login_success()
{
    cout << "Login success!" << endl;
}

void Viewer::render_login_fail()
{
    cout << "Login failed." << endl;
}

void Viewer::render_permission_denied()
{
    cout << "Permission denied." << endl;
}

void Viewer::render_board_not_found()
{
    cout << "Board not found." << endl;
}

void Viewer::render_account_already_exist()
{
    cout << "Account already exist." << endl;
}

void Viewer::render_board_already_exist()
{
    cout << "Board already exist." << endl;
}

void Viewer::render_post_not_found()
{
    cout << "Post not found." << endl;
}

void Viewer::render_add_post_failed()
{
    cout << "Add post failed." << endl;
}

void Viewer::render_no_current_board()
{
    cout << "Select a board first." << endl;
}

void Viewer::render_no_current_post()
{
    cout << "No post is selected." << endl;
}

void Viewer::render_no_mail()
{
    cout << "Mailbox is empty." << endl;
}

void Viewer::render_user_not_found()
{
    cout << "User not found." << endl;
}
