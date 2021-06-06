#include <fstream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/filesystem.hpp>
#include "BoardManager.h"

int main()
{
    BoardManager ptt;
    if (boost::filesystem::exists("record.s"))
    {
        std::ifstream f("record.s");
        {
            boost::archive::text_iarchive ia(f);
            ia >> ptt;
        }
    }

    ptt.start();

    std::ofstream f("record.s", std::ios_base::trunc);
    {
        boost::archive::text_oarchive oa(f);
        oa << ptt;
    }
    f.close();
}
