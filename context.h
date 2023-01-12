#pragma once

namespace dorrego
{
    using boost::asio::detached;
    using boost::asio::ip::port_type;

    template <typename BOOK = Book, typename REQUEST = Request, typename RESPONSE = Response>
    class Context
    {
    private:
        BOOK book;
        Listener<REQUEST, RESPONSE> listener;

    public:
        Context(port_type _port) : listener(Listener<REQUEST, RESPONSE>(_port, [&](REQUEST &_request, RESPONSE &_response)
                                                                        { return this->callback(_request, _response); }))
        {
        }

        virtual ~Context() = default;

        void run()
        {
            try
            {
                boost::asio::io_context io_context(10);
                boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);

                signals.async_wait([&](auto, auto)
                                   { io_context.stop(); });

                boost::asio::co_spawn(io_context, this->listener.perform(), detached);

                io_context.run();
            }
            catch (std::exception &_ex)
            {
                std::cerr << _ex.what() << std::endl
                          << std::flush;
            }
        }

        void callback(REQUEST &_request, RESPONSE &_response)
        {
            Command command(this->book, _request, _response);

            command.execute();
        }
    };
}