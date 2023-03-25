#pragma once

#include <exception>
#include <string>

class KrakenException
{
public:
    KrakenException() = default;
    KrakenException(const KrakenException& other) = default;

private:
    size_t line;
    std::string file;
    std::wstring description;
};