#include "StringUtils.h"

std::string StringUtils::ToUpper(std::string str)
{
    //copy string
    std::string result = str;
    for (int i = 0; i < str.length(); i++)
    {
        //if character at index i is lowercase, change it to uppercase
        if (result[i] >= 97 && result[i] <= 122) result[i] = result[i] - 32;
    }

    //return the string
    return result;
}

std::string StringUtils::ToLower(std::string str)
{
    //copy string
    std::string result = str;
    for (int i = 0; i < str.length(); i++)
    {
        //if character at index i is uppercase, change it to lowercase
        if (result[i] >= 65 && result[i] <= 90) result[i] = result[i] + 32;
    }

    //return the string
    return result;
}

bool StringUtils::IsEqualIgnoreCase(std::string str1, std::string str2)
{
    //copy strings and convert to all lowercase
    std::string low1 = ToLower(str1), low2 = ToLower(str2);

    //evaluate
    return low1 == low2;
}

std::string StringUtils::CreateUnique(std::string str)
{
    //create the static count varialble
    static int count;

    //initialize it if this is the first time running (This might be unnecessary but it works :) )
    if (count == NULL) count = 0;

    //add the count to the provided string, return that, and increment the count
    return str + std::to_string(count++);

}
