#include <string>

void jsonToFrequencies(std::string json, double *frequencies)
{
    std::stringstream ss = std::stringstream(json.substr(1, json.size() - 2));
    for (int i = 0; i < 128; ++i)
    {
        std::string substr;
        std::getline(ss, substr, ',');
        frequencies[i] = std::stod(substr);
    }
}
