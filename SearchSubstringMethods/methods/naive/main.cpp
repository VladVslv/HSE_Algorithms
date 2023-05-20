#include <iostream>
#include <string>
#include <fstream>
#include <chrono>

int find_with_substitution(const std::string &search_str, const std::string &template_str)
{
    bool exist;
    int result = -1;
    for (size_t i = 0; i + template_str.size() <= search_str.size(); i++)
    {
        exist = true;
        for (size_t j = 0; j < template_str.size(); j++)
        {
            if (!(template_str[j] == '?' || template_str[j] == search_str[i + j]))
            {
                exist = false;
                break;
            }
        }
        if (exist)
        {
            result = i;
        }
    }
    return result;
}

int find(const std::string &search_str, const std::string &template_str)
{
    bool exist;
    int result = -1;
    for (size_t i = 0; i + template_str.size() <= search_str.size(); i++)
    {
        exist = true;
        for (size_t j = 0; j < template_str.size(); j++)
        {
            if (template_str[j] != search_str[i + j])
            {
                exist = false;
                break;
            }
        }
        if (exist)
        {
            result = i;
        }
    }
    return result;
}

int main(int argc, char *argv[])
{
    std::string search_str, template_str, output_file;
    int number_of_repetitions, number_of_substitutions;
    std::ifstream input_search(argv[1]), input_template(argv[2]);
    std::getline(input_search, search_str);
    input_search.close();
    std::getline(input_template, template_str);
    input_template.close();
    number_of_substitutions = std::stoi(argv[3]);
    number_of_repetitions = std::stoi(argv[5]);
    long long time = 0;
    for (int i = 0; i < number_of_repetitions; ++i)
    {
        auto start = std::chrono::high_resolution_clock::now();
        if (number_of_substitutions == 0)
        {
            find(search_str, template_str);
        }
        else
        {
            find_with_substitution(search_str, template_str);
        }
        time += std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::high_resolution_clock::now() - start)
                    .count();
    }
    time /= number_of_repetitions;
    std::ofstream output;
    output.open(argv[4], std::ios_base::app);
    output << time << " ";
    output.close();
}