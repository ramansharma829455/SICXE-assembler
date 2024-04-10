#include <iostream>
#include <sstream>
#include <iomanip>

std::string decimalToTwosComplement(int decimalNumber, int hexStringLength) {
    // Check if the number fits in the specified length
    if (decimalNumber >= -(1 << (4 * hexStringLength - 1)) && decimalNumber < (1 << (4 * hexStringLength - 1))) {
        // Convert negative numbers to their equivalent positive values in twos complement
        if (decimalNumber < 0) {
            decimalNumber += (1 << (4 * hexStringLength));
        }

        // Convert the positive decimal number to hexadecimal string
        std::stringstream ss;
        ss << std::uppercase << std::hex << std::setw(hexStringLength) << std::setfill('0') << decimalNumber;
        return ss.str();
    } else {
        // If the number doesn't fit in the specified length, return an empty string
        return "";
    }
}

int main() {
    int decimalNumber = 255; // Example decimal number
    int hexStringLength = 6;  // Example length of the hexadecimal string
    std::string twosComplementHex = decimalToTwosComplement(decimalNumber, hexStringLength);
    if (!twosComplementHex.empty()) {
        std::cout << "Twos complement of " << decimalNumber << " in hexadecimal base with length " << hexStringLength << " is: " << twosComplementHex << std::endl;
    } else {
        std::cout << "Decimal number doesn't fit in the specified length." << std::endl;
    }
    return 0;
}
