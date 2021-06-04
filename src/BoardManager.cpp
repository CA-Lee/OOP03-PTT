#include "BoardManager.h"

BoardManager::BoardManager() : viewer(this)
{
    user_list.push_back(Admin("root", "root"));
    board_list.push_back(Board("hello_world"));
    board_list.push_back(Board("8========D"));
}

void BoardManager::start()
{
    viewer.start();
}

void BoardManager::login(string user_id, string passwd)
{
    bool valid = false;
    for (auto &user : user_list)
    {
        if (user.verify(user_id, passwd))
        {
            valid = true;
            current_user = &user;
        }
    }
    if (valid)
    {
        viewer.render_login_success();
        viewer.render_board_list();
    }
    else
    {
        viewer.render_login_fail();
        viewer.render_menu();
    }
}

void BoardManager::logout()
{
    current_user = nullptr;
    viewer.render_menu();
}

bool BoardManager::sign_up(string user_id, string password)
{
    for (auto &user : user_list)
    {
        if (user.id == user_id)
        {
            viewer.render_account_already_exist();
            return false;
        }
    }
    user_list.push_back(User(user_id, password, Privilege::Member));
    return true;
}

vector<Board>::iterator BoardManager::find_board(string board_id)
{
    for (int i = 0; i < board_list.size(); i++)
    {
        if (board_list[i].get_id() == board_id)
        {
            return board_list.begin() + i;
        }
    }
    return board_list.end();
}

void BoardManager::add_board(string board_id)
{
    if (current_user->get_privilege() == Privilege::Admin)
    {
        if (find_board(board_id) == board_list.end())
        {
            board_list.push_back(Board(board_id));
        }
        else
        {
            viewer.render_board_already_exist();
        }
    }
    else
    {
        viewer.render_permission_denied();
    }
}

void BoardManager::select_board(string board_id)
{
    auto board = find_board(board_id);
    if (board != board_list.end())
    {
        viewer.render_board(*board);
    }
    else
    {
        viewer.render_board_not_found();
    }
}

void BoardManager::delete_board(string board_id)
{
    if (current_user->get_privilege() == Privilege::Admin)
    {
        auto board = find_board(board_id);
        if (board != board_list.end())
        {
            board_list.erase(board);
        }
        else
        {
            viewer.render_board_not_found();
        }
    }
    else
    {
        viewer.render_permission_denied();
    }
}
