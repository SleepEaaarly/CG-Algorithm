#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <cassert>
#include <sstream>

class SHSampler {
private:
    size_t degree;
    std::vector<float> sh_coeffs;

    std::string path;
    bool has_data;

    bool read_n_floats() {
        std::ifstream fin(path);
        if (!fin) {
            return false;
        }
        auto n = degree * degree;
        std::string line;
        if (!std::getline(fin, line)) {
            return false;
        }
        std::istringstream iss(line);
        sh_coeffs.reserve(n);
        float value;    size_t cnt = 0;
        while (iss >> value) {
            cnt++;
            if (cnt > n)    return false;
            sh_coeffs.push_back(value);
        }

        return sh_coeffs.size() == n;
    }

public:
    SHSampler(size_t deg, const char* path) : degree(deg), path(path) {
        has_data = read_n_floats();
    }

    bool hasData() {
        return has_data;
    }

    void calcSHCoeffs(std::vector<float>& image_data) {
        
    }
};