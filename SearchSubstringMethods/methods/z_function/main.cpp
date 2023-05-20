#include <iostream>
#include <string>
#include <fstream>
#include <chrono>

int find_with_substitution(const std::string &search_str, const std::string &template_str)
{
    std::string z_str = template_str + '#' + search_str;
    int *z = new int[z_str.length()];
    int l = 0, r = 0;
    z[0] = 0;
    int result = -1;
    for (int i = 1; i < z_str.length(); i++)
    {
        if (i > r)
        {
            l = i;
            r = i;
            while (r < z_str.length() && z_str[r - l] != '#' && z_str[r] != '#' && (z_str[r - l] == z_str[r] || z_str[r - l] == '?' || z_str[r] == '?'))
            {
                ++r;
            }
            z[i] = r - l;
            --r;
        }
        else
        {
            if (z[i - l] < r - i + 1)
            {
                z[i] = z[i - l];
            }
            else
            {
                l = i;
                while (r < z_str.length() && z_str[r - l] != '#' && z_str[r] != '#' && (z_str[r - l] == z_str[r] || z_str[r - l] == '?' || z_str[r] == '?'))
                {
                    ++r;
                }
                z[i] = r - l;
                --r;
            }
        }
        if (z[i] == template_str.length())
        {
            result = i - (template_str.length() + 1);
        }
    }
    delete[] z;
    return result;
}

int find(const std::string &search_str, const std::string &template_str)
{
    std::string z_str = template_str + '#' + search_str;
    int *z = new int[z_str.length()];
    int l = 0, r = 0;
    z[0] = 0;
    int result = -1;
    for (int i = 1; i < z_str.length(); i++)
    {
        if (i > r)
        {
            l = i;
            r = i;
            while (r < z_str.length() && z_str[r - l] == z_str[r])
            {
                ++r;
            }
            z[i] = r - l;
            --r;
        }
        else
        {
            if (z[i - l] < r - i + 1)
            {
                z[i] = z[i - l];
            }
            else
            {
                l = i;
                while (r < z_str.length() && z_str[r - l] == z_str[r])
                {
                    ++r;
                }
                z[i] = r - l;
                --r;
            }
        }
        if (z[i] == template_str.length())
        {
            result = i - (template_str.length() + 1);
        }
    }
    delete[] z;
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