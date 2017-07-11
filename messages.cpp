/*
    PLEASE READ THE FILE "protocol.txt" WHERE IS DEFINED THE STRUCTURE OF EACH TYPE OF MESSAGE OF THIS FILE !!!!!!!

    (S) --> begin section of comment
    (E) --> end section of comment
*/

#include <sstream>
#include <string.h>
#include <iostream>

using namespace std;

typedef char const* chars;
const int DIGITS_TO_SIZE_IN_BYTES_ALL = 6; // this variable is for all type of message, because is the number of digits where the size of ALL message is 

chars concat_chars(chars a, chars b){
    char * result = (char*)malloc(strlen(a) + strlen(b) + 1);
    strcpy(result, a); // copy string a into the result.
    strcat(result, b);

    return result;
}

chars fill_zeros(int number, int how_many){
    stringstream strs;
    for (int i = 0; i < how_many; ++i)
        strs<<"0";
    strs<<number;
    string temp_str = strs.str();
    chars resp = temp_str.c_str();

    return resp;
}

int number_digits(int number){
    int digits = 0;
    while (number != 0) { number /= 10; digits++; }

    return digits;
}

chars char_to_chars(char a){
    stringstream strs;
    strs<<a;
    string temp_str = strs.str();
    chars chars_a = temp_str.c_str();

    return chars_a;
}

int chars_to_int(chars a){
    stringstream ss(a);
    long num;
    ss>>num;
    return num;
}

chars sub_chars(chars c, int pos, int range){

    char* substr = new char[range + 1];
    for(int i=pos; i<pos+range; i++)
        substr[i-pos] = c[i];

    return substr;
}


// THIS BOTTOM SECTION ARE ALL TYPE OF MESSAGES FOR COMUNICATE WITH SERVER.
// uSUALLY IS THE NAME OF FUNCTION TYPE MESSAGE WITH THEIR FUNCTIONALLITY(ENVELOP OR UNWRAPPING). Ex name_of_message_env() / name_of_message_unwr()

chars simple_message_env(chars m){
    int type_message = 1; // this is the type of this message func
    int size_header_2 = 6; // characters

    //(S) First part composed by header-2 and text-data
    int size_m = strlen(m);    
    int n_digits = number_digits(size_m);
    chars header2 = fill_zeros(size_m, size_header_2 - n_digits);
    chars first_part = concat_chars(header2, m);
    //(E) First part composed by header-2 and text-data

    //(S) Second part composed by header-1 and first part(header-2 | text-data)
    char char_size_header_2 = (char)size_header_2;
    chars chars_size_header_2 = char_to_chars(char_size_header_2);
    chars second_part = concat_chars(chars_size_header_2, first_part);
    //(E) Second part composed by header-1 and first part(header-2 | text-data)

    //(S) Total message composed by type and second_part(header-1 | header-2 | text-data)
    char char_type_message = (char)type_message;
    chars chars_type_message = char_to_chars(char_type_message);
    chars enveloped_messa = concat_chars(chars_type_message, second_part);
    //(E) Total message composed by type and second_part(header-1 | header-2 | text-data)

   
    /*int SIZE_ALL = strlen(enveloped_messa);
    int n_digits_SIZE_ALL = number_digits(SIZE_ALL);
    chars MAIN_HEADER = fill_zeros(SIZE_ALL, DIGITS_TO_SIZE_IN_BYTES_ALL - n_digits_SIZE_ALL);
    chars TOTAL_enveloped_messa = concat_chars(MAIN_HEADER, enveloped_messa);
    printf("SIZE_ALL: %d\n", SIZE_ALL);
    printf("TOTAL_enveloped_messa: %s\n", TOTAL_enveloped_messa);*/
    
    return enveloped_messa;
}

chars simple_message_unwr(chars m){
    int type_message = 1; // this is the type of this message func

    // (S) Getting data of header-1 that is size of header-2, and after getting the data of header-2
    int size_header_2 = (int)m[1]; // getting the data of header-1 that is size of header_2
    chars header2 = sub_chars(m, 2, size_header_2);
    // (E) Getting data of header-1 that is size of header-2, and after getting the data of header-2

    // (S) Getting the size of text-data, and after getting the substr of message
    int value_header2 = chars_to_int(header2);;
    chars unwrapped_messa = sub_chars(m, 2 + size_header_2 , value_header2);
    // (E) Getting the size of text-data, and after getting the substr of message

    return unwrapped_messa;
}