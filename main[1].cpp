// Chinedu Ohiri
// Brainfuck Interpreter 
// CS355 
#include <iostream> 
#include <string> 
#include <cstring> 
#include <fstream>
#include <exception>
#include <algorithm>
using namespace std;

class BFStart {
public:
    // Constructor that creates a new state referencing the different code
    BFStart(string code);
    // Destructor that deallocates memory from the data array 
    ~BFStart(); 
    // Executes the intepreter
    void run();
private:
    // Private Methods
    void inc_dp();
    void dec_dp();
    void inc_data();
    void dec_data();
    void output() const;
    void input();
    void loop();
    void repeat();
    bool valid_instruction(char instruction) const;

    // Private Variables
    string code; // Brainfuck code 
    const char* ip; // Pointer to the current instruction in code 
    char* data; // Pointer to data array
    char* dp;  // Character pointer to modify next 

    // End of class
};

// Main Function 
int main(int argc, char* argv[]) {
    // Check if file is properly passed in 
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <filename.bf>" << endl;
        return 1;
    }

    const char* filename = argv[1];

    // Debugging
    // cout << "Input file path: " << filename << endl; 

    // Open the Brainfuck source file
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return 1;
    }

    // Read the contents of the file into a string
    string code((istreambuf_iterator<char>(file)), (istreambuf_iterator<char>()));

    // cout << code << endl; 

    // Close the file
    file.close();


    // Pass in Brainfuck code string into intepreter
    BFStart bfInterpreter(code);

    // cout << bfInterpreter.ip << endl; 
 

    // cout << code << endl; 

    // Run interpreter
    bfInterpreter.run();



    return 0;
}

// Function Constructor: BFStart(string)
// Creates a new state referencing the different code 
// Initalizes data pointer to the start of the data array
// Initalizes instruction pointer to the beginning of the string 
BFStart::BFStart(string code) : code(code), ip(code.c_str()){

    //cout << *ip << endl; 
    ip = this->code.c_str(); 
 
    // Allocate memory for the data array 
    data = new char[30000]; 
    // Initalize data array
    fill_n(data, 30000, 0);
    // Initialize data pointer 
    dp = data; 

}

// FUnction Destructor: ~BFStart()
// Deallocates data array 
BFStart::~BFStart() {

    // Deallocate data array 
    delete[] data; 
}


// Function: inc_dp
// Increments the data pointer (Next cell to right)
void BFStart::inc_dp() {

    // Make sure that data pointer is within the range of the array
    if (dp < data + 30000 - 1) {
        // Increment to the right 
        dp++;
    }
    else {
        // Throw an exception.
        throw out_of_range("Data pointer out of Bounds!");
    }
}

// Function: dec_dp 
// Decrement the data pointer (Next cell to the left)
void BFStart::dec_dp() {

    // Make sure that data pointer is within the range of the array 
    if (dp > data) {
        // Decrement to the left 
        dp--;
    }
    else {
        // Throw an exception
        throw out_of_range("Data pointer out of Bounds!");
    }
}

// Function: inc_data
// Increment (By One) the byte at the data pointer
void BFStart::inc_data() {

    // Within range of the data array 
    if (dp >= data && dp < data + 30000) {
        // Increment by one 
        (*dp)++;
    }
    else {
        // Throw an exception
        throw out_of_range("Data pointer out of Bounds!");
    }
}

// Function: dec_data
// Decrement (By One) the byte at the data pointer
void BFStart::dec_data() {

    // Within range of the data array 
    if (dp >= data && dp < data + 30000) {
        // Decrement by one 
        (*dp)--;
    }
    else {
        // Throw an exception 
        throw out_of_range("Data pointer out of Bounds!");
    }
}

// Function: output
// Output the byte at the data pointer
void BFStart::output() const {
    // Within range of data array
    if (dp >= data && dp < data + 30000) {
        // Output the byte at the data pointer as a character
        cout << static_cast<char>(*dp);
    }
    else {
        throw out_of_range("Data pointer out of Bounds!");
    }
    // cout << endl;
}

// Function: input 
// Accept one byte input, storing its value in the byte at the data pointer 
void BFStart::input() {
    // Input character. Assuring input() only reads a single character
    char inputCharacter;
    cin.get(inputCharacter); // Read a single character

    // While within range of the data array 
    if (dp >= data && dp < data + 30000) {
        // Store inputCharacter into data pointer
        *dp = inputCharacter;
    }
    else {
        // Throw an exception 
        throw out_of_range("Data pointer is out of bounds!");
    }
}

// Function: loop 
// If the byte at the dp is zero, jump until after the "]"
void BFStart::loop() {
    // Check if data pointer is within bounds
    if (dp < data || dp >= data + 30000) {
        throw out_of_range("Data pointer is out of bounds!");
    }

    // Check if current byte is 0 
    if (*dp == 0) {
        int bracketCount = 1;

        // Loop through code 
        for (const char* currentInstruction = ip + 1; *currentInstruction != '\0'; currentInstruction++) {
            if (*currentInstruction == '[') {
                bracketCount++;
                if (bracketCount == 0) {
                    // The matching bracket has been found. Set instruction pointer to next 
                    ip = currentInstruction;
                    return;
                }
            }
            else if (*currentInstruction == ']') {
                bracketCount--;

            }
        }
    }
}

// Function: repeat 
// If the byte at the data pointer is non zero, jump back until after "["
// Function: repeat 
// If the byte at the data pointer is non-zero, jump back until after "["
void BFStart::repeat() {
    // Check if data pointer is within bounds
    if (dp < data || dp >= data + 30000) {
        throw out_of_range("Data pointer is out of bounds!");
    }


    // Check if current byte is non-zero
    if (*dp != 0) {
        int bracketCount = 1;

        // Loop through code 
        for (const char* currentInstruction = ip - 1; *currentInstruction != '\0'; currentInstruction--) {
            if (*currentInstruction == '[') {
                bracketCount--;
                if (bracketCount == 0) {

                    // The matching bracket has been found. Set instruction pointer to next 
                    ip = currentInstruction;
                    return;
                }
            }
            else if (*currentInstruction == ']') {
                bracketCount++;

            }
        }
    }
}

// Function: valid_instruction
// Checks if the instruction pointer contains a valid instruction. 
bool BFStart::valid_instruction(char instruction) const {
    // Valid Instructions
    const string validInstructions = "+-<>.,[]";

    // Check if it's within valid instructions
    return validInstructions.find(instruction) != string::npos;
}

// Function: run
// Executes the intepreter
void BFStart::run() {

    try {
        // Run as long as hasn't reached the end of the program
        while (*ip != '\0') {
            // Read the current instruction
            char currentInstruction = *ip;

            //cout << *ip << endl; 
            // cout << *dp << endl; 

            // Validate the current instruction
            if (valid_instruction(currentInstruction)) {
                // Process instructions
                switch (currentInstruction) {
                    // Brainfuck code instructions
                case '+':
                    inc_data();
                    break;
                case '-':
                    dec_data();
                    break;
                case '>':
                    inc_dp();
                    break;
                case '<':
                    dec_dp();
                    break;
                case '.':
                    output();
                    break;
                case ',':
                    input();
                    break;
                case '[':
                    loop();
                    break;
                case ']':
                    repeat();
                    break;
                } 
            } 
            else {
                // Ignore 
            }

            // Move to the next instruction
            ip++;
        }
    }
    catch (const exception& e) {
        // Exceptions come from private methods
        cerr << "Exception: " << e.what() << endl;
    }
}