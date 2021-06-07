#include <algorithm>
#include "BoardManager.h"

using std::find_if;

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
    current_board = nullptr;
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
    user_list.push_back(Member(user_id, password));
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
        current_board = &(*board);
        current_board->view();
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

void BoardManager::select_post(string post_id)
{
    if (current_board != nullptr)
    {
        auto post = current_board->find_post(post_id);
        if (post != current_board->get_post_list().end())
        {
            current_post = &(*post);
            viewer.render_post(*post);
        }
        else
        {
            viewer.render_post_not_found();
        }
    }
    else
    {
        viewer.render_no_current_board();
    }
}

void BoardManager::add_post(string board_id, string title, string content)
{
    auto board = find_board(board_id);
    if (board != board_list.end())
    {
        Post *p = (*board).add_post(title, current_user->id, content);
        current_user->add_post(p);
    }
    else
    {
        viewer.render_board_not_found();
        viewer.render_add_post_failed();
    }
}

void BoardManager::delete_post(string post_id)
{
    if (current_board != nullptr)
    {
        auto post = current_board->find_post(post_id);
        if (post != current_board->get_post_list().end())
        {
            if (current_user->id == (*post).author_id)
            {
                //delete pointer
                current_user->delete_post(&(*post));
                //delete post
                current_board->delete_post(post_id);
            }
            else
            {
                viewer.render_permission_denied();
            }
        }
        else
        {
            viewer.render_post_not_found();
        }
    }
    else
    {
        viewer.render_no_current_board();
    }
}

void BoardManager::edit_post(string post_id)
{
    if (current_board != nullptr)
    {
        auto post = current_board->find_post(post_id);
        if (post != current_board->get_post_list().end())
        {
            if (current_user->id == (*post).author_id)
            {
                viewer.render_edit_post(*post);
            }
            else
            {
                viewer.render_permission_denied();
            }
        }
        else
        {
            viewer.render_post_not_found();
        }
    }
    else
    {
        viewer.render_no_current_board();
    }
}

void BoardManager::edit_post(string post_id, string title, string content)
{
    if (current_board != nullptr)
    {
        auto post = current_board->find_post(post_id);
        if (post != current_board->get_post_list().end())
        {
            if (current_user->id == (*post).author_id)
            {
                (*post).edit(title, content);
            }
            else
            {
                viewer.render_permission_denied();
            }
        }
        else
        {
            viewer.render_post_not_found();
        }
    }
    else
    {
        viewer.render_no_current_board();
    }
}

void BoardManager::add_comment(Weight w, string s)
{
    if (current_post != nullptr)
    {
        current_post->add_comment(Comment(w, s, current_user->id));
        viewer.render_post(*current_post);
    }
    else
    {
        viewer.render_no_current_post();
    }
}

void BoardManager::send_mail(string to, string content)
{
    auto user = find_user(to);
    if (user != user_list.end())
    {
        (*user).add_mail(current_user->id, content);
    }
    else
    {
        viewer.render_user_not_found();
    }
}

vector<User>::iterator BoardManager::find_user(string user_id)
{
    return find_if(
        user_list.begin(),
        user_list.end(),
        [&user_id](User u)
        { return u.id == user_id; });
}

void BoardManager::check_mail()
{
    if (current_user->get_mail().size() == 0)
    {
        viewer.render_no_mail();
    }
    else
    {
        viewer.render_mail(current_user->get_mail());
    }
}
