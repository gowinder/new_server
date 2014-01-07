#include <cstdlib>
#include <locale>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/atomic.hpp>
#include <boost/thread.hpp>

using namespace boost::asio::ip;
using namespace std;
using namespace boost::posix_time;
using namespace boost;
static int id = 1;
static const int msg_size = 1024 * 1;
static const char message[msg_size] =
{ 0 };
static const int BUFFER_SIZE = 1024 * 512;

static boost::detail::atomic_count s_echo_count(0);

template<typename T>
class basic_atom: noncopyable
{
private:
	T n;
	mutex mu;
public:
	basic_atom(T x = T()) :
			n(x)
	{
	}
	T operator++()
	{
		mutex::scoped_lock lock(mu);
		return ++n;
	}
	operator T()
	{
		return n;
	}

	T operator +=(T x)
	{
		mutex::scoped_lock lock(mu);
		return n = n + x;
	}
	T get_value()
	{
		return n;
	}
};

typedef unsigned long long UINT64;
typedef basic_atom<int> atom_int;
typedef basic_atom<UINT64> atom_uint64;
static atom_int s_echo_use_time(0);
static atom_uint64 s_all_byte(0);

class echo_session
{
public:
	echo_session(boost::asio::io_service& io_service) :
			socket_(io_service)
	{
		id_ = id;
		++id;
	}
	void start(const std::string& ip, const std::string& port)
	{
		try
		{
			//解析主机地址
			tcp::resolver resolver(socket_.get_io_service());
			tcp::resolver::query query(tcp::v4(), ip, port);
			tcp::resolver::iterator iterator = resolver.resolve(query);
			//异步连接
			socket_.async_connect(*iterator,
					boost::bind(&echo_session::handle_connect, this,
							boost::asio::placeholders::error));
		} catch (boost::system::error_code& ec)
		{
			cout << ec.message() << endl;
		}
	}
private:
	void handle_connect(const boost::system::error_code& error)
	{
		if (!error)
		{
			try
			{

				boost::asio::socket_base::send_buffer_size send_buffer_size(
						BUFFER_SIZE);
				boost::asio::socket_base::receive_buffer_size receive_buffer_size(
						BUFFER_SIZE);
				socket_.set_option(send_buffer_size);
				socket_.set_option(receive_buffer_size);

				//连接成功，发送message中的数据
				boost::asio::async_write(socket_,
						boost::asio::buffer(message, msg_size),
						boost::bind(&echo_session::handle_write, this,
								boost::asio::placeholders::error));
			}
			catch (boost::system::error_code& ec)
			{
				cout << ec.message() << endl;
			}

			_last_write_time = microsec_clock::universal_time();
		}
		else
			cout << "id:" << id << " " << error << endl;
	}
	void handle_write(const boost::system::error_code& error)
	{
		if (!error)
		{
			//写入完毕，接收服务器回射的消息
			boost::asio::async_read(socket_,
					boost::asio::buffer(buf_, sizeof(buf_)),
					boost::bind(&echo_session::handle_read, this,
							boost::asio::placeholders::error,
							boost::asio::placeholders::bytes_transferred));
		}
		else
			cout << "write id:" << id << " " << error << endl;
	}
	void handle_read(const boost::system::error_code& error,
			size_t bytes_transferred)
	{
		if (!error)
		{
			//读取完毕，在终端显示
			//	cout << id_ << ":receive:" << bytes_transferred << "," << buf_ << endl;
			//周而复始...
			time_duration td = microsec_clock::universal_time()
					- _last_write_time;
			++s_echo_count;
//				cout << " use " << td.total_microseconds() / 1000 << endl;
			s_echo_use_time += (int) td.total_microseconds() / 1000;
			s_all_byte += bytes_transferred;
//				boost::detail::InterlockedExchangeAdd(&s_echo_total_time, td);
//				boost::detail::atomic_count::atomic_exchange_and_add((long)s_echo_use_time, td);
			if (s_echo_count % 10000 == 0)
			{
//					locale chs("chs");
//					cout.imbue(chs);
				cout << "echo count " << s_echo_count << " use time "
						<< s_echo_use_time << " avg: "
						<< s_echo_use_time / s_echo_count << " byte: "
						<< s_all_byte.get_value() << endl;
			}
			handle_connect(error);
		}
		else
			cout << "receive id:" << id << " " << error << endl;
	}
	int id_;
	tcp::socket socket_;
	char buf_[sizeof(message)];
	ptime _last_write_time;
};
int main(int argc, char* argv[])
{
	const int session_num = 1000;			//连接的数量
	echo_session* sessions[session_num];
	memset(sessions, 0, sizeof(sessions));
	try
	{
//		if (argc != 3)
//		{
//			std::cerr << "Usage: blocking_tcp_echo_client <host> <port>/n";
//			return 1;
//		}
		boost::asio::io_service io_service;
		//创建session_num个连接
		for (int i = 0; i < session_num; i++)
		{
			sessions[i] = new echo_session(io_service);
			//	sessions[i]->start(argv[1], argv[2]);
			string ip = "192.168.96.150";
			string port = "2234";
			sessions[i]->start(ip, port);
			usleep(30 * 1000);
		}
		//io_service主循环
		io_service.run();
		for (int i = 0; i < session_num; ++i)
			if (sessions[i] != NULL)
				delete sessions[i];
	} catch (std::exception& e)
	{
		for (int i = 0; i < session_num; ++i)
			if (sessions[i] != NULL)
				delete sessions[i];
		std::cerr << "Exception: " << e.what() << "/n";
	}
	return 0;
}
