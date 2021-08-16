#include <openssl/md5.h>
#include <string>
#include <iomanip>
#include <ios>

int leading_zeros(string s) {
	for (size_t i = 0; i < s.size(); i++) {
		if (s[i] != '0')
			return i;
	}
	return s.size();
}

string hash_to_hex(unsigned char result[]) {
	std::ostringstream sout;
	sout << std::hex << std::setfill('0');
	for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
		sout << std::setw(2) << (long long) result[i];
	}
	return sout.str();
}

void day_4(string input) {
	unsigned char result[MD5_DIGEST_LENGTH];

	int i = 0;

	do {
		i++;
		string nonced_input = input + to_string(i);
		MD5((unsigned char*) nonced_input.c_str(), nonced_input.size(), result);
	} while (leading_zeros(hash_to_hex(result)) < 5);
	cout << "Secret Key for 5 zeros: " << i << endl;

	while (leading_zeros(hash_to_hex(result)) < 6) {
		i++;
		string nonced_input = input + to_string(i);
		MD5((unsigned char*) nonced_input.c_str(), nonced_input.size(), result);
	}
	cout << "Secret Key for 6 zeros: " << i << endl;

}
