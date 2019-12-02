/*
INSTR Hex Instruction Set
Andrew Player, Robert Lawton, Gannon Higgins
October 18, 2019
DOESN'T WORK WITH 32-BIT MINGW/32-BIT GCC
*/

#include "assembler.hpp"

/* This is an assembler that takes instr assembly and */
/* turns it into valid instr machine code.            */

/* This holds all current labels and the integers   */
/* that they get converted to in the machine code   */
/* this is global for use between functions without */
/* needed to pass it around                         */
std::map<std::string, int> LABEL_MAP;
int CURRENT_LABEL_VALUE = 0x0001;

/* Remove ws from beginning and end of line*/
std::string trim(const std::string& s) {
    std::size_t s_begin = s.find_first_not_of(' ');
    if(s_begin < s.size()) {
        std::size_t s_end   = s.find_last_not_of(' ');
        std::string s_trimmed = s.substr(s_begin, s_end - s_begin + 1);
        if(s_trimmed == "") {
            std::cout << "trimmed " << s << " to nothing" << std::endl;
        }
        return s_trimmed;
    }
    return "";
}

bool is_label(const std::string& line) {
    int pos = 0;
    std::string line_trimmed = trim(line);
    while(++pos != line_trimmed.size() && line_trimmed[pos] != ':');
    if(line_trimmed[pos] != ':') {
        return false;
    }
    return true;
}

/* gets just the label name before the ':' */
std::string trim_label(const std::string& label) {
    int pos = 0;
    std::string label_no_ws = trim(label);
    std::string label_trimmed = "";
    while(pos != label_no_ws.size() && label_no_ws[pos] != ':') {
        label_trimmed += label_no_ws[pos];
        pos++;
    }
    return label_trimmed;
}

/* checks if str is a valid opcode mnemonic */
bool is_opcode(const std::string& str) {
    for(std::string s : opcode_vector) {
        if(str == s) return true;
    }
    return false;
}

bool is_comment(const std::string& line) {
    if(trim(line)[0] != '#') {
        return false;
    } 
    return true;
} 

bool is_register(const std::string& str) {
    if(str_to_reg.count(str) == 0) {
        return false;
    }
    return true;
}

bool is_instruction(const std::string& line) {
    bool prop = is_opcode(trim(line.substr(0, 3)));
    if(prop) {
        return true;
    }
    return false;
}

bool is_int(const std::string& str) {
    int num;
    std::istringstream is(str);
    if(is >> num) {
        return true;
    }
    return false;
}

/* convert str to an integer if possible */
/* exits with code 1 if not              */
int to_int(const std::string& str) {
    if(is_int(str)) {
        int number;
        std::istringstream is(str);
        is >> number;
        return number;
    }
    std::cout << "ERROR: Tried to convert " << str << "to an int." << std::endl;
    exit(1);
}

/* makes a vector with the opcode and its operands from a givin line */
vector<std::string> parse_instruction(const std::string& line) {
    int pos = 0;
    std::string line_trimmed = trim(line);
    std::string op = "";
    while(pos != line_trimmed.size() && !iswspace(line_trimmed[pos])) {
        op += line_trimmed[pos++];
    }
    if(!is_opcode(op)) {
        std::cout << "Invalid Instruction: " << op << std::endl;
        exit(1);
    }
    while(pos != line_trimmed.size() && iswspace(line_trimmed[pos])) {
        pos++;
    }
    std::string arg0 = "";
    std::string arg1 = "";
    while(pos != line_trimmed.size() 
    && (!iswspace(line_trimmed[pos])) && line_trimmed[pos] != ',') {
        arg0 += line_trimmed[pos++];
    }
    while(pos != line_trimmed.size() 
    && (iswspace(line_trimmed[pos]) || line_trimmed[pos] == ',')) {
        pos++;
    }
    while(pos != line_trimmed.size() && !iswspace(line_trimmed[pos])) {
        if(line_trimmed[pos] == '\\' && line_trimmed[pos+1] == '\'') {
            arg1 += line_trimmed[++pos];
            pos++;
        }
        if(line_trimmed[pos] != '\'') {
            arg1 += line_trimmed[pos++];
        } else {
            pos++;
        }
    }
    if(!is_int(arg1) && !is_register(arg1) && pos != line_trimmed.size()) {
        while(pos != line_trimmed.size() 
        && (line_trimmed[pos-1] != '0' && line_trimmed[pos-2] != '\\')) {
            if(line_trimmed[pos] == '\\' && line_trimmed[pos+1] == '\'') {
                arg1 += line_trimmed[++pos];
                pos++;
            } 
            if(line_trimmed[pos] != '\'') {    
                arg1 += line_trimmed[pos++];
            } else {
                pos++;
            }
        }
    }
    vector<std::string> instruction_plus_args = {op, arg0, arg1};
    return instruction_plus_args;
}

/* makes a vector with the opcode, and operands, paired with their types */
/* NOTE: This has no error handling, only pass it validated instructions */
vector<pair<std::string, ARG_TYPE>> parse_arg_types(const vector<std::string>& arg_vector) {
    vector<pair<std::string, ARG_TYPE>> arg_type_vec;
    for(std::string arg : arg_vector) {
        if(is_opcode(arg)) {
            arg_type_vec.push_back(pair<std::string, ARG_TYPE>(arg, OPCODE));
        } else if(is_int(arg)) {
            arg_type_vec.push_back(pair<std::string, ARG_TYPE>(arg, INTEGER));
        } else if(is_register(arg)) {
            arg_type_vec.push_back(pair<std::string, ARG_TYPE>(arg, REGISTER));
        } else {
            arg_type_vec.push_back(pair<std::string, ARG_TYPE>(arg, STRING));
        }
    }
    return arg_type_vec;
}

/* takes in an instruction (arg_type) vector along with its corresponding    */
/* opcode's information then generates the machine code for that instruction */
std::string builder(const vector<pair<std::string, ARG_TYPE>>& instr) {
    std::string op = instr[0].first;
    int op_bits  = op_to_int[op][0];
    int reg_bits = op_to_int[op][1];
    int int_bits = op_to_int[op][2];
    int str_bits = op_to_int[op][3];
    int B, C, D = 0x0;
    bool is_int_arg = false;
    int int_arg = 0x0;
    bool is_str_arg = false;
    std::string str_arg = "";
    if(instr.size() == 3) {
        switch(instr[2].second) {
            case INTEGER:
                B = int_bits;
                C = 0x0;
                D = str_to_reg[instr[1].first];
                int_arg = to_int(instr[2].first);
                is_int_arg = true;
                break;
            case REGISTER:
                B = reg_bits;
                C = str_to_reg[instr[1].first];
                D = str_to_reg[instr[2].first];
                break;
            case STRING:
                B = str_bits;
                C = 0x0;
                if(op_bits != 0xD) {
                    D = str_to_reg[instr[1].first];
                    str_arg = instr[2].first;
                    is_str_arg = true;
                } else {
                    D = 0x0;
                    str_arg = instr[1].first;
                    if(LABEL_MAP.find(str_arg)->second == 0) {
                        LABEL_MAP.insert({str_arg, CURRENT_LABEL_VALUE});
                        int_arg = CURRENT_LABEL_VALUE++;
                    } else {
                        int_arg = LABEL_MAP.find(str_arg)->second;
                    }
                    is_int_arg = true;
                }
                break;
            default:
                std::cout << "Something wrong in general" << std::endl;
                exit(1);
        }
    }
    int inst = (op_bits << 12) | (B << 8) | (C << 4) | (D << 0);
    std::ostringstream os;
    os << "0x" << std::hex << inst << '\n';
    if(is_int_arg) {
        os << "0x" << int_arg << '\n';
    }
    if(is_str_arg && str_arg != "") {
        os << str_arg << '\n';
    }
    return os.str();
}

/* Generates machine code from the assembly file: filename  */
/* returns an ostringstream with the generated machine code */
std::ostringstream gen_machine_code(const std::string& filename) {
    std::ostringstream os;
    std::ifstream file(filename); 
    std::string line = "";
    int line_number = 1;
    while(std::getline(file, line)) {        
        if(line != "") {
            std::string line_trimmed = trim(line);
            if(is_instruction(line_trimmed)) {
                vector<std::string> instruction_vector = parse_instruction(line);
                auto arg_type_vector = parse_arg_types(instruction_vector);
                os << builder(arg_type_vector);
            } else if(is_comment(line)) {
                os << line << '\n';
            } else if(is_label(line)) {
                std::string label = trim_label(line);
                os << "#" << label << '\n';
                os << "0x" << std::hex << 0xDF00 << '\n';
                if(LABEL_MAP.find(label)->second == 0) {
                    LABEL_MAP.insert({trim_label(line), CURRENT_LABEL_VALUE});
                    os << "0x" << std::hex << CURRENT_LABEL_VALUE++ << '\n';
                } else {
                    os << "0x" << std::hex << LABEL_MAP.find(label)->second << '\n';
                }
            } else {
                std::cout << "Invalid Line: ";
                std::cout << line;
                std::cout << " @" << line_number << std::endl;
                exit(1); 
            }
            ++line_number;
        }
    }

    return os;
}

int main() {
    std::cout << "Filename: ";
    std::string filename = "";
    std::cin >> filename;
    std::ostringstream machine_code = gen_machine_code(filename);
    std::string ofilename = filename + ".inst";
    std::ofstream out_file(ofilename);
    out_file << machine_code.str();
    std::cout << "Program Compiled Successfully :)\n" << std::endl;
    return 0;
}