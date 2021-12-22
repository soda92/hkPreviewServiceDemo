//#include <QCoreApplication>

#include <cstdlib>
#include <iostream>
#include <boost/bind/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>

#include "HCSDK.h"

using boost::asio::ip::tcp;
const int max_length = 1024;

typedef boost::shared_ptr<tcp::socket> socket_ptr;
using std::vector;
using std::string;

void session(socket_ptr sock)
{
    try
    {
        for (;;)
        {
            char data[max_length];

            boost::system::error_code error;
            size_t length = sock->read_some(boost::asio::buffer(data), error);
            if (error == boost::asio::error::eof)
                continue; // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.

            std::string str{data};
            vector<string> result;
            boost::split(result, str, boost::is_any_of(" "));
            long userID = HK::login(result[0].c_str());
            if(userID >=0 ){
                HK::preview(static_cast<long long>(std::stoi(result[1])), userID);
            }

            boost::asio::write(*sock, boost::asio::buffer(data, length));
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception in thread: " << e.what() << "\n";
    }
}

void server(boost::asio::io_context& io_context, unsigned short port)
{
    tcp::acceptor a(io_context, tcp::endpoint(tcp::v4(), port));
    for (;;)
    {
        socket_ptr sock(new tcp::socket(io_context));
        a.accept(*sock);
        boost::thread t(boost::bind(session, sock));
    }
}

int main()
{
    HK::initSDK();
    try
    {
//        if (argc != 2)
//        {
//            std::cerr << "Usage: blocking_tcp_echo_server <port>\n";
//            return 1;
//        }

        boost::asio::io_context io_context;

        using namespace std; // For atoi.
        server(io_context, atoi("1234"));
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    HK::cleanupSDK();

    return 0;
}
