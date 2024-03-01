#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>

std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

std::vector<int> compute_prefix_function(const std::vector<int>& pattern) {
    int m = pattern.size();
    std::vector<int> pi(m);
    int k = 0;
    for (int q = 1; q < m; ++q) {
        while (k > 0 && pattern[k] != pattern[q]) {
            k = pi[k - 1];
        }
        if (pattern[k] == pattern[q]) {
            ++k;
        }
        pi[q] = k;
    }
    return pi;
}
// here we are using the Knuth-Morris-Pratt algorithm to find the pattern in the data
int kmp_matcher(std::vector<uint8_t>& data, const std::string& pattern) {
    std::vector<std::string> pattern_bytes = split(pattern, ' ');
    std::vector<int> p;
    for (const auto& byte : pattern_bytes) {
        if (byte != "??") {
            p.push_back(std::stoi(byte, nullptr, 16));
        }
        else {
            p.push_back(-1);
        }
    }
    int n = data.size();
    int m = p.size();
    std::vector<int> pi = compute_prefix_function(p);
    int q = 0;
    for (int i = 0; i < n; ++i) {
        while (q > 0 && (p[q] != data[i] && p[q] != -1)) {
            q = pi[q - 1];
        }
        if (p[q] == data[i] || p[q] == -1) {
            ++q;
        }
        if (q == m) {
            return i - m + 1;
        }
    }
    return -1;
}

void patch_code(const std::string& in_file, const std::string& out_file, const std::vector<std::pair<std::string, std::string>>& byte_pairs) {
    std::ifstream input(in_file, std::ios::binary);
    std::vector<uint8_t> data((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());

    for (const auto& [orig, repl] : byte_pairs) {
        int index = kmp_matcher(data, orig);
        if (index != -1) {
            std::cout << "Found a match for " << orig << " at offset: " << std::hex << index << std::endl;
            std::vector<uint8_t> repl_bytes;
            std::istringstream hex_chars_stream(repl);
            unsigned int c;
            while (hex_chars_stream >> std::hex >> c) {
                repl_bytes.push_back(c);
            }
            if (repl_bytes.size() < split(orig, ' ').size()) {
                repl_bytes.insert(repl_bytes.end(), split(orig, ' ').size() - repl_bytes.size(), 0);
            }
            std::copy(repl_bytes.begin(), repl_bytes.end(), data.begin() + index);
        }
        else {
            std::cout << "No matches found for " << orig << std::endl;
        }
    }

    std::ofstream output(out_file, std::ios::binary);
    output.write(reinterpret_cast<const char*>(data.data()), data.size());
}

int main() {
    std::vector<std::pair<std::string, std::string>> byte_sequences = {
        {"40 53 48 83 EC ?? 8B D9 33 C9 E8 ?? ?? ?? ?? 80 3D ?? ?? ?? ?? ?? 75 ?? 8B 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? C6 05 ?? ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 45 33 C0 8B D3 48 8B 88 ?? ?? ?? ?? 48 8B 49 ?? 48 83 C4 ?? 5B E9 ?? ?? ?? ?? CC CC CC CC CC 48 83 EC ?? 33 C9 E8 ?? ?? ?? ?? 80 3D ?? ?? ?? ?? ?? 75 ?? 8B 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? C6 05 ?? ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 33 D2 48 8B 88 ?? ?? ?? ?? 48 8B 49 ?? 48 83 C4 ?? E9 ?? ?? ?? ?? CC CC CC CC CC CC CC CC CC CC CC CC CC 40 53 48 83 EC ?? 8B D9 33 C9 E8 ?? ?? ?? ?? 80 3D ?? ?? ?? ?? ?? 75 ?? 8B 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? C6 05 ?? ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 45 33 C0 8B D3 48 8B 88 ?? ?? ?? ?? 48 8B 49 ?? 48 83 C4 ?? 5B E9 ?? ?? ?? ?? CC CC CC CC CC 48 83 EC ?? 33 C9 E8 ?? ?? ?? ?? 80 3D ?? ?? ?? ?? ?? 75 ?? 8B 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? C6 05 ?? ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 33 D2 48 8B 88 ?? ?? ?? ?? 48 8B 49 ?? 48 83 C4 ?? E9 ?? ?? ?? ?? CC CC CC CC CC CC CC CC CC CC CC CC CC 48 83 EC", "48 B8 01 00 00 00 00 00 00 00 C3"}, //public static bool BIsDlcInstalled(AppId_t appID) { }
        {"40 53 48 83 EC ?? 8B D9 33 C9 E8 ?? ?? ?? ?? 80 3D ?? ?? ?? ?? ?? 75 ?? 8B 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? C6 05 ?? ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 45 33 C0 8B D3 48 8B 88 ?? ?? ?? ?? 48 8B 49 ?? 48 83 C4 ?? 5B E9 ?? ?? ?? ?? CC CC CC CC CC 40 55 53", "B8 85 47 DE 63 C3"}, //public static uint GetEarliestPurchaseUnixTime(AppId_t nAppID) { }
        {"48 83 EC ?? 80 3D ?? ?? ?? ?? ?? 75 ?? 8B 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? C6 05 ?? ?? ?? ?? ?? 48 8B 0D ?? ?? ?? ?? F6 81 ?? ?? ?? ?? ?? 74 ?? 83 B9 ?? ?? ?? ?? ?? 75 ?? E8 ?? ?? ?? ?? 33 C9 E8 ?? ?? ?? ?? 84 C0 0F 85", "48 B8 01 00 00 00 00 00 00 00 C3"} //public static bool get_isSelectedUnlockMaster() { }
    };

    std::string input_file = "GameAssembly.dll";
    std::string output_file = "GameAssembly_patched.dll";

    patch_code(input_file, output_file, byte_sequences);
    std::cin.get();
    return 0;
}
