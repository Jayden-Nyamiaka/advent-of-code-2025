#include <iostream>
#include <fstream>
#include <string>

const static size_t DEFAULT_START_POSITION = 50;

size_t count_zero_positions(std::ifstream& file, size_t start_position, bool is_step_2 = false);
size_t rotate_dial(std::string line, size_t& position, bool passes_zero = false);
void usage(char **argv);


int main(int argc, char **argv)
{
    size_t start_position = DEFAULT_START_POSITION;
    bool is_step_2 = false;

    if (argc == 1) {
        usage(argv);
        return 1;
    }

    if (argc > 2) {
        char *s = argv[2];
        for (size_t i = 0; s[i] != '\0'; ++i) {
            s[i] = std::tolower(static_cast<unsigned char>(s[i]));
        }
        if (std::string(s) == std::string("true")) {
            is_step_2 = true;
        }
    }
    std::cout << is_step_2 << std::endl;

    if (argc > 3) {
        try {
            start_position = (size_t) std::stoi(argv[2]);
        } catch (...) {
            std::cerr << "Error: Could not convert <optional_dial_start_position> argument to a valid start position.\n";
            usage(argv);
            return 1;
        }
    }
    if (start_position < 0 || start_position > 99) {
        std::cerr << "Error: dial start position must be between 0 and 99 (inclusive).\n";
        usage(argv);
        return 1;
    }

    char* filename = argv[1];
    std::cout << "Opening file " << filename << std::endl;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file.\n";
        usage(argv);
        return 1;
    }
    std::cout << "Successfully opened " << filename << std::endl;

    size_t answer = count_zero_positions(file, start_position, is_step_2);
    std::cout << "The answer is " << answer << "." << std::endl;

    file.close();
    return 0;
}

size_t count_zero_positions(std::ifstream& file, size_t start_position, bool is_step_2)
{
    size_t zero_count = 0;
    size_t pos = start_position;

    std::string line;
    while (std::getline(file, line)) {
        std::cout << "Processing " << line;

        zero_count += rotate_dial(line, pos, is_step_2);

        std::cout << ":\tPosition = " << pos << ", Count = " << zero_count << std::endl;
    }

    return zero_count;
}

/**
 * Updates position and returns the number of times the dial points to 0.
 * If passes_zero is false (step 1), it only count times when the dial lands
 *      at zero at the end of a rotation.
 * If passes_zero is true (step 2), it counts any time when the dial passed
 *      zero (during OR end of a rotation).
 */
size_t rotate_dial(std::string line, size_t& position, bool passes_zero)
{
    size_t zero_count = 0;
    char direction = line[0];
    std::string magnitude = line.substr(1);

    int val;
    try {
        val = std::stoi(magnitude);
    } catch (...) {
        std::cerr << "Error: Could not convert number to int.\n";
    }

    int psuedo_pos = position;
    if (direction == 'R') {
        psuedo_pos += val;
    } else {
        psuedo_pos -= val;
    }

    if (psuedo_pos >= 100) {
        if (passes_zero) {
            zero_count += psuedo_pos / 100;
        }
        psuedo_pos = psuedo_pos % 100;
    } else if (psuedo_pos < 0) {
        if (passes_zero) {
            zero_count += -1*(psuedo_pos / 100) + 1;
            if (position == 0) {
                zero_count--;
            }
        }
        psuedo_pos = (psuedo_pos % 100 + 100) % 100;
    } else if (psuedo_pos == 0) {
        if (passes_zero) {
            zero_count++;
        }
    }

    position = (size_t) psuedo_pos;

    if (!passes_zero && psuedo_pos == 0) {
        zero_count++;;
    }
    return zero_count;
}

void usage(char **argv) {
    std::cerr << "Usage: " << argv[0] << " <required_input_file> <optional_bool_is_step2> <optional_dial_start_position>\n";
}
