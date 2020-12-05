#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <iterator>
#include "half.hpp"

using half_float::half;
using namespace half_float::literal;

auto to_vector(half f)
{
	std::vector<char> data(sizeof(f));
	std::memcpy(data.data(), &f, sizeof(f));
	return data;
}

auto from_vector(const std::vector<char>& data)
{
	half f;
	std::memcpy(&f, data.data(), sizeof(f));
	return f;
}

void write_float(std::string const& name, unsigned int offset, half number) {
	std::fstream s(name);
	s.seekg(offset);
	auto new_data = to_vector(number);
	s.seekp(offset);
	s.write(&new_data[0], 2);
}

half read_float(std::string const& name, unsigned int offset) {
	std::fstream s(name);

	s.seekg(offset);
	std::vector<char> data(2);
	s.read(&data[0], 2);
	return from_vector(data);
}

void show(std::string const& name) {
	std::ifstream in(name);

	std::copy(std::istreambuf_iterator<char>(in),
		std::istreambuf_iterator<char>(),
		std::ostream_iterator<char>(std::cout, ""));
}

std::vector<half> create_vec2(half x, half y) {
	std::vector<half> output(2);
	output[0] = x;
	output[1] = y;
	return output;
}

void write_data(std::string const& name) {
	unsigned int buffer_offset = 0xB92C;
	unsigned int object_offset = 2820116;
	unsigned int offset = buffer_offset + object_offset;
	unsigned int stride = 36;
	unsigned int data_size = 12;

	std::vector<std::vector<half>> data(data_size);

	data[0] = create_vec2(0.0304565_h, 0.410156_h);
	data[1] = create_vec2(0.0_h, 0.433838_h);
	data[2] = create_vec2(0.0_h, 0.410156_h);
	data[3] = create_vec2(0.261475_h, 0.020416_h); //LR
	data[4] = create_vec2(0.249146_h, 0.0_h); //UL
	data[5] = create_vec2(0.261475_h, 0.0_h); //UR
	data[6] = create_vec2(0.249146_h, 0.020416_h); //LL
	data[7] = create_vec2(0.728516_h, 0.0_h);
	data[8] = create_vec2(0.722168_h, 0.0204163_h);
	data[9] = create_vec2(0.722168_h, 0.0_h);
	data[10] = create_vec2(0.728516_h, 0.0204163_h);
	data[11] = create_vec2(0.0304565_h, 0.433838_h);

	for (unsigned int i = 0; i < data_size; i++) {
		unsigned int o = offset + (i * stride);
		write_float(name, o, data[i][0]);
		write_float(name, o + 2, data[i][1]);

		write_float(name, o + 4, data[i][0]);
		write_float(name, o + 6, data[i][1]);
	}
}

void read_data(std::string const& name) {
	unsigned int buffer_offset = 0xB92C;
	unsigned int object_offset = 2825876;
	unsigned int offset = buffer_offset + object_offset;
	unsigned int stride = 36;
	unsigned int  data_size = 12;

	for (unsigned int i = 0; i < data_size; i++) {
		unsigned int o = offset + (i * stride);
		std::cout << i << ": ";
		std::cout << read_float(name, o);
		std::cout << ", ";
		std::cout << read_float(name, o + 2);
		if (i < (data_size-1)) std::cout << "\n";
	}

	
}

int main() {
	std::string name("C:/Users/Tyler Knox/Documents/GitHub/A32NX/A32NX/SimObjects/AirPlanes/Asobo_A320_NEO/model/A320_NEO_INTERIOR_LOD00.bin");

	write_data(name);
	//read_data(name);
}