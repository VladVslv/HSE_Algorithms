#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <vector>

int find(const std::string &search_str, const std::string &template_str)
{
    int n = static_cast<int>(search_str.length()), m = static_cast<int>(template_str.length());
    int *br = new int[m + 1];
    int k = 0;
    br[1] = 0;
    br[0] = 0;
    int result = -1;
    for (int i = 2; i <= m; ++i)
    {
        while (k > 0 && template_str[k] != template_str[i - 1])
        {
            k = br[k];
        }
        if (template_str[k] == template_str[i - 1])
        {
            ++k;
        }
        br[i] = k;
    }
    k = 0;
    for (int i = 0; i < n; ++i)
    {
        while (k > 0 && template_str[k] != search_str[i])
        {
            k = br[k];
        }
        if (template_str[k] == search_str[i])
        {
            ++k;
        }
        if (k == m)
        {
            result = i - m + 1;
        }
    }
    delete[] br;
    return result;
}

int find_with_substitution(const std::string &search_str, const std::string &template_str)
{
    int res = -1;
    int max_ch = 1;
    for (auto ch : search_str)
    {
        if (ch != '?' && ch - '0' + 1 > max_ch)
        {
            max_ch = ch - '0' + 1;
        }
    }
    std::vector<int> subtitutions = {};
    for (size_t i = 0; i < template_str.length(); i++)
    {
        if (template_str[i] == '?')
        {
            subtitutions.push_back(i);
        }
    }
    std::string current = template_str;
    int pos = -1;
    int number_of_variants = 1;
    for (size_t i = 0; i < subtitutions.size(); i++)
    {
        number_of_variants *= max_ch;
    }
    for (size_t i = 0; i < number_of_variants; i++)
    {
        int num = i;
        for (size_t j = 0; j < subtitutions.size(); j++)
        {
            current[subtitutions[j]] = num % max_ch + '0';
            num /= max_ch;
        }
        pos = find(search_str, current);
        res = pos > res ? pos : res;
    }
    return res;
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