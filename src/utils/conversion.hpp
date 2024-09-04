#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <cstdint>
#include <cstring>


// Converts a float to a vector of uint8_t (byte array)
const std::vector<uint8_t> float2vec(float value) {
    std::vector<uint8_t> vec(sizeof(float));
    std::memcpy(vec.data(), &value, sizeof(float));
    return vec;
}

// Converts a portion of a vector of uint8_t (byte array) to a float
// If the vector size from the given offset is not sufficient to convert to float, returns NaN
const float vec2float(const std::vector<uint8_t>& vec, std::size_t offset = 0) {
    float value;
    if (offset + sizeof(float) > vec.size()) {
        value = std::numeric_limits<float>::quiet_NaN();  // NaN
    } else {
        std::memcpy(&value, vec.data() + offset, sizeof(float));
    }
    return value;
}

#endif // UTILS_HPP
