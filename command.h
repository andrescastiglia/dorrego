#pragma once

namespace dorrego
{
    using boost::interprocess::named_mutex;
    using boost::interprocess::open_or_create;

    template <typename BOOK = Book, typename REQUEST = Request, typename RESPONSE = Response>
    class Command
    {
    private:
        char *mutex_name;
        bool succeeded;
        short phase;

        BOOK &book;
        REQUEST &request;
        RESPONSE &response;

    public:
        Command(BOOK &_book, REQUEST &_request, RESPONSE &_response);
        virtual ~Command();

        void execute()
        {
            std::cerr << "Command not implemented" << std::endl
                      << std::flush;
        }

        void rollback()
        {
            std::cerr << "Rollback not implemented" << std::endl
                      << std::flush;
        }
    };
}