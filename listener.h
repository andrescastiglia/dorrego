#pragma once

namespace dorrego
{
    using boost::asio::awaitable;
    using boost::asio::detached;
    using boost::asio::use_awaitable;
    using boost::asio::ip::port_type;
    using boost::asio::ip::tcp;

    template <typename REQUEST = Request, typename RESPONSE = Response>
    class Listener
    {
    private:
        const port_type port;
        std::function<void(REQUEST &, RESPONSE &)> callback;

        awaitable<void> accept(tcp::socket _socket)
        {
            try
            {
                while (_socket.is_open())
                {
                    co_await this->receive(_socket);
                }
            }
            catch (std::exception &_ex)
            {
                std::cerr << _ex.what() << std::endl
                          << std::flush;
            }
        }

        awaitable<void> receive(tcp::socket &_socket)
        {
            REQUEST request;

            try
            {
                co_await boost::asio::async_read(_socket, boost::asio::buffer(&request, sizeof(request)), use_awaitable);
            }
            catch (boost::system::system_error &_ex)
            {
                if (_ex.code() != boost::asio::error::eof &&
                    _ex.code() != boost::asio::error::connection_reset &&
                    _ex.code() != boost::asio::error::connection_aborted)
                {
                    std::cerr << _ex.what() << std::endl
                              << std::flush;
                }
                _socket.close();
                co_return;
            }

            RESPONSE response;
            this->callback(request, response);

            try
            {
                co_await boost::asio::async_write(_socket, boost::asio::buffer(&response, sizeof(response)), use_awaitable);
            }
            catch (boost::system::system_error &_ex)
            {
                if (_ex.code() != boost::asio::error::eof &&
                    _ex.code() != boost::asio::error::connection_reset &&
                    _ex.code() != boost::asio::error::connection_aborted)
                {
                    std::cerr << _ex.what() << std::endl
                              << std::flush;
                }
                _socket.close();
            }
        }

    public:
        Listener(port_type _port, std::function<void(REQUEST &, RESPONSE &)> _callback) : port(_port), callback(_callback)
        {
        }

        virtual ~Listener() = default;

        awaitable<void> perform()
        {
            auto executor = co_await boost::asio::this_coro::executor;

            tcp::acceptor acceptor(executor, {tcp::v4(), this->port});
            while (acceptor.is_open())
            {
                tcp::socket socket = co_await acceptor.async_accept(use_awaitable);
                boost::asio::co_spawn(executor, this->accept(std::move(socket)), detached);
            }
        }
    };
}