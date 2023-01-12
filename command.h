#pragma once

#define MUTEX_NAME "DorregoMutex"

namespace dorrego
{
    using boost::interprocess::named_mutex;
    using boost::interprocess::open_or_create;

    template <typename BOOK = Book, typename REQUEST = Request, typename RESPONSE = Response>
    class Command
    {
    private:
        named_mutex mutex;

        BOOK &book;
        REQUEST &request;
        RESPONSE &response;

    public:
        Command(BOOK &_book, REQUEST &_request, RESPONSE &_response)
            : mutex(named_mutex(open_or_create, MUTEX_NAME)), book(_book), request(_request), response(_response)
        {
        }

        virtual ~Command() = default;

        void execute()
        {
            std::cerr << "Command not implemented" << std::endl
                      << std::flush;
        }
    };
}