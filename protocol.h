#include <iostream>
#include <vector>
#include <math.h>
#include <map>
#include "messages.cpp"

using namespace std;

typedef char const* chars;
typedef chars (*func)(chars);

class Protocol{

    public:
        char* message;
        int size_header_type_message;
        int type_message; // 8 bits in decimal, so max of 255
        map<chars, func> type_messages_envelop; //<val_char*_of_func, func>
        map<int, func> type_messages_unwrap; // <value_int_of_func, func>

        Protocol();

        // (total_message)
        chars all_message_size(chars);

        // (type_message, data) 
        chars envelop(chars, chars);

        // (data)
        chars unwrap(chars);

        // transform first character to bits
        vector<int> transform_char_to_bits(unsigned char);

        int transform_bits_to_decimal(vector<int>);
        void print_binary(vector<int>);

};

Protocol::Protocol(){
    this->size_header_type_message = 8; // 8 bits in decimal, so max of 255
    this->type_messages_envelop["simple-message"] = simple_message_env; // key for this envelop func is "simple-message" 
    this->type_messages_unwrap[1] = simple_message_unwr; // key for this unwarp func is "1"
}

void Protocol::print_binary(vector<int> v){
    for(int i=0; i<v.size(); i++)
        cout<<v[i]<<" ";
}

// Is saving in reverse way
vector<int> Protocol::transform_char_to_bits(unsigned char type){
    vector<int> binary;
    for(int i = 0; i < this->size_header_type_message; i++)
        binary.push_back((type >> i) & 1 );
    //this->print_binary(binary);

    return binary;
}

int Protocol::transform_bits_to_decimal(vector<int> v){
    int result = 0;
    int j=0;
    for(int i=0; i<v.size(); i++)
    {
        if(v[i])
            result += pow(2, i);
    }

    return result;
}


chars Protocol::all_message_size(chars total_messa){
    /*int SIZE_ALL = strlen(total_messa) + DIGITS_TO_SIZE_IN_BYTES_ALL;*/
    int SIZE_ALL = strlen(total_messa);
    int n_digits_SIZE_ALL = number_digits(SIZE_ALL);
    chars MAIN_HEADER = fill_zeros(SIZE_ALL, DIGITS_TO_SIZE_IN_BYTES_ALL - n_digits_SIZE_ALL);

    return MAIN_HEADER;
}


chars Protocol::envelop(chars type_messa, chars data){
    map<chars, func>::iterator it;
    // searching a func in map of type of messages to envelop with func found
    it = this->type_messages_envelop.find(type_messa);

    return it->second(data);
}


chars Protocol::unwrap(chars data){
    char type_in_char = data[0];
    vector<int> type_in_vec = this->transform_char_to_bits(type_in_char);
    int type_messa = this->transform_bits_to_decimal(type_in_vec);

    map<int, func>::iterator it;
    // searching a func in map of type of messages to unwrap with func found
    it = this->type_messages_unwrap.find(type_messa);

    return it->second(data);
}

