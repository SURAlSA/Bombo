#include "util.h"

// functions

string getCurrentTimeStamp()
{
    time_t t = time(nullptr);
    char buffer[100];
    if (strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H-%M-%S", localtime(&t))) 
    {
        return string(buffer);
    }
    return "unknown";
}


string removeFromList(const string& line, const string& item)
{
    string result = line;
    size_t pos = result.find(item);
    if (pos != string::npos)
    {
        size_t start = (pos > 0 && result[pos - 1] == ',') ? pos - 1 : pos;
        size_t end = pos + item.length();
        if (end < result.length() && result[end] == ',') ++end;
        result.erase(start, end - start);
    }
    return result;
}


int getRandomInt(int min, int max)
{
    random_device rd;
    mt19937 generator(rd());

    uniform_int_distribution<int> distribution(min, max);

    return distribution(generator);
}

