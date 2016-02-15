// polar_test_2.cpp : Defines the entry point for the console application.
// DEVELOPED BY Polyakov Yaroslav AS TEST
// Program implements nearest palindrome search for integer value


#include "stdafx.h"
#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include <conio.h>

enum NUMBER_TYPE { EVEN_SIZED_NUMBER, UNEVEN_SIZED_NUMBER };

//chars in number count type(EVEN/UNEVEN)
//how will go pairing process depends on this property 
NUMBER_TYPE getNumberType(int number) {
	if (number % 2 == 0)
		return EVEN_SIZED_NUMBER;
	return UNEVEN_SIZED_NUMBER;
}

//pair of two integeres staying on the same distance from the center in decomposed number representation
struct pair {
	pair() :left(0), right(0) {}
	pair(int _left, int _right) :left(_left), right(_right) {}

	int left;
	int right;
};

//decompose number into chars
void decompose_number(int src_number, std::vector<int>& dst_decomposed_number) {
	std::string number_str = std::to_string(src_number);
	const int size = number_str.size();
	dst_decomposed_number.resize(size);
	for (int i = 0; i < size; i++) {
		dst_decomposed_number[i] = std::stoi(std::string(1, number_str[i]));
	}
}

//left and right direction start indexes and pairs array size initialization
void precalculate_pairs_iterator_info(int& left_start, int& right_start, int& pairs_size, int decomposed_number_size, NUMBER_TYPE type) {
	if (type == EVEN_SIZED_NUMBER) {
		pairs_size = decomposed_number_size / 2;
		left_start = decomposed_number_size / 2 - 1;
		right_start = decomposed_number_size / 2;
	}
	else {
		pairs_size = round(static_cast<float>(decomposed_number_size) / 2.0f);
		left_start = pairs_size - 1;
		right_start = pairs_size - 1;
	}
}


//pairs array fill/initialization
void pairs_initialize(const std::vector<int>& src_decomposed_number, std::vector<pair>& dst_pairs, NUMBER_TYPE type) {
	const int size = src_decomposed_number.size();
	int pairs_size = 0;

	int p_left = 0;
	int p_right = 0;

	precalculate_pairs_iterator_info(p_left, p_right, pairs_size, size, type);

	dst_pairs.resize(pairs_size);
	for (int i = 0; i < pairs_size; i++) {
		dst_pairs[i] = pair(src_decomposed_number[p_left], src_decomposed_number[p_right]);
		p_left--;
		p_right++;
	}
}

//base process in calculating palindrome
//algorithm is based on pair's properties
void process_pairs(std::vector<pair>& dst_pairs, int decompose_elementls_size, NUMBER_TYPE type) {
	const int pairs_size = dst_pairs.size();
	int all_pairs_left_values_unlocked_since_index = pairs_size - 1;
	for (int i = 0; i < pairs_size; i++) {
		if (dst_pairs[i].left > dst_pairs[i].right && i == 0) {
			dst_pairs[i].right = dst_pairs[i].left;
			all_pairs_left_values_unlocked_since_index = i;
			break;
		}

		if (dst_pairs[i].left < dst_pairs[i].right && i == 0) {
			dst_pairs[i].left++;
			dst_pairs[i].right = dst_pairs[i].left;
			all_pairs_left_values_unlocked_since_index = i;
			break;
		}

		if (dst_pairs[i].left == dst_pairs[i].right && i == 0)
			continue;

		if (dst_pairs[i].left > dst_pairs[i].right) {
			dst_pairs[i].right = dst_pairs[i].left;
			all_pairs_left_values_unlocked_since_index = i;
			break;
		}
		if (dst_pairs[i].left < dst_pairs[i].right) {
			dst_pairs[i - 1].left++;
			dst_pairs[i - 1].right = dst_pairs[i - 1].left;
			dst_pairs[i].right = dst_pairs[i].left;
		}
	}
	for (int i = all_pairs_left_values_unlocked_since_index + 1; i < pairs_size; i++) {
		dst_pairs[i].right = dst_pairs[i].left;
	}
}

//composing pairs into decomposed number
void compose_pairs(const std::vector<pair>& src_pairs, std::vector<int>& dst_decomposed_number, NUMBER_TYPE type) {
	const int size = dst_decomposed_number.size();
	int pairs_size;

	int p_left = 0;
	int p_right = 0;

	precalculate_pairs_iterator_info(p_left, p_right, pairs_size, size, type);

	for (int i = 0; i < pairs_size; i++) {
		dst_decomposed_number[p_left] = src_pairs[i].left;
		dst_decomposed_number[p_right] = src_pairs[i].right;
		p_left--;
		p_right++;
	}
}

//compose decomposed number
int compose_number(const std::vector<int>& decomposed_number) {
	const int size = decomposed_number.size();

	std::string buffer;
	for (int i = 0; i < size; i++) {
		buffer += std::to_string(decomposed_number[i]);
	}
	return std::stoi(buffer);
}

int generate_palindrome(int number) {
	std::vector<int> decomposed_number;
	std::vector<pair> pairs;

	decompose_number(number, decomposed_number);

	const int size = decomposed_number.size();
	NUMBER_TYPE type = getNumberType(size);

	pairs_initialize(decomposed_number, pairs, type);
	process_pairs(pairs, size, type);
	compose_pairs(pairs, decomposed_number, type);

	return compose_number(decomposed_number);
}

int main()
{
	int input_value = 0;
	std::cout << "Enter value: " << std::endl;
	std::cin >> input_value;
	std::cout << generate_palindrome(input_value);
	_getche();
	return 0;
}