#include <cstring>
#include "saver.h"

void Saver::perform(char *in_buffer, std::size_t in_len, char *out_buffer, std::size_t& out_len)
{
    //echo
    std::memcpy( out_buffer, in_buffer, in_len);
    out_len = in_len;
}