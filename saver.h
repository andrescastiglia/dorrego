#pragma once

class Saver
{
public:
    void perform(char *in_buffer, std::size_t in_len, char *out_buffer, std::size_t& out_len);
};
