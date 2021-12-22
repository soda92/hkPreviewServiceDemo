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
            sock->read_some(boost::asio::buffer(data), error);
            if (error == boost::asio::error::eof)
                continue; // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.

            std::string str{data};
            vector<string> result;
            boost::split(result, str, boost::is_any_of(" "));
            long result_ = 0;
            if(result[0] == "osd"){
                result_ = HK::set_osd(std::stol(result[1]),
                        result[2].c_str(), result[3].c_str(),
                        result[4].c_str(), result[5].c_str(),
                        result[6].c_str(), result[7].c_str(),
                        static_cast<bool>(std::stoi(result[8])),
                        static_cast<bool>(std::stoi(result[9])));
            }
            else if(result[0] == "sync_time"){
                result_ = HK::sync_time(std::stol(result[1]));
            }
            else if(result[0] == "time_manual"){
                result_ = HK::set_time(
                            std::stol(result[1]),
                        std::stol(result[2]),
                        std::stol(result[3]),
                        std::stol(result[4]),
                        std::stol(result[5]),
                        std::stol(result[6]),
                        std::stol(result[7]));
            }
            else{
                long userID = HK::login(result[0].c_str());
                if(userID >=0 ){
                    HK::preview(static_cast<long long>(std::stoi(result[1])), userID);
                }
                result_ = userID;
            }

            std::string str_result = std::to_string(result_);
//            boost::asio::write(*sock, boost::asio::buffer(data, length));
            boost::asio::write(*sock, boost::asio::buffer(str_result));
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
