#include "AES2.h"
#include <iostream>
#include<fstream>

using namespace std;

//constructor
AES2::AES2(string s) {
	fileName = s;
}

//displays the vector in hex array
void AES2::DisplayVector(vector<uint8_t> v) {
	//check if vector size is a multiple of 16
	if (v.size() != 16) {
		cout << "The vector is not of size 16." << endl;
		cout << "It is of size " << v.size() << "." << endl;
	}

	for (int i = 0; i < 4; i++) {
		cout << hex << (int)v[i] << " ";
		cout << hex << (int)v[i+4] << " ";
		cout << hex << (int)v[i+8] << " ";
		cout << hex << (int)v[i + 12] << endl;
	}
	cout << endl;
}

//displays the input vector
void AES2::DisplayInput() {
	DisplayVector(input);
}

//displays the state vector
void AES2::DisplayState() {
	DisplayVector(state);
}

 //add element to input vector
 void AES2::InputInsert(uint8_t u) {
	 input.push_back(u);
 }

 //add element to key vector
 void AES2::InputKey(uint8_t u) {
	 key_input.push_back(u);
 }
 
 //generate the Keys form key_input vector
 void AES2::generateKeys() {
	 //local variables
	 vector<uint8_t> cv;

	 //add initial 4 words from key
	 for (int i = 0; i < key_input.size(); i++) {
		 //add char to vector if not full
		 if (cv.size() < 4) {
			 cv.push_back(key_input[i]);
		 }
		 //when full,add to Words vector,clear then add to start of new vector
		 else {
			 addKey(cv);
			 cv.clear();
			 cv.push_back(key_input[i]);
		 }
	 }
	 addKey(cv);


	 //generate the rest of words
	 int i = 4;
	 while (i < 44) {
		 //get previous word
		 vector<uint8_t> temp = Keys[i - 1];
		 if (i % 4 == 0) {
			 //perform roate then substitution after each round(4 words/keys)
			Keys.push_back(performXOR(performXOR(Rcon[i / 4 - 1], SubKey(RotateKey(temp))), Keys[i - 4]));
		 }
		 else {
			 Keys.push_back(performXOR(Keys[i - 4], temp));
		 }
		 ++i;
	 }

 }

 //adds a word to words vector
 void AES2::addKey(vector<uint8_t> word) {
	 Keys.push_back(word);
 }

 //returns a vector with elements once left rotated 
 vector<uint8_t> AES2::RotateKey(vector<uint8_t> cv) {
	 //local variable
	 vector<uint8_t> temp;

	 //start from index 1 of inputted vector
	 for (int i = 1; i < cv.size(); i++) {
		 temp.push_back(cv[i]);
	 }
	 //add first inputted vector element as last in shifted vector
	 temp.push_back(cv[0]);

	 return temp;
 }

 //returns a substitution vector of the one inputted
 vector<uint8_t> AES2::SubKey(vector<uint8_t> cv) {
	 //local variable
	 vector<uint8_t> temp;
	 int x = 0;
	 int y = 0;

	 for (int i = 0; i < cv.size(); i++) {
		 //calculate grid coordinate for subsituition look up
		 x = cv[i] / 16;
		 y = cv[i] % 16;

		 //look up and add to vector
		 temp.push_back(S_Box[x][y]);
	 }

	 return temp;
 }

 //performs XOR operation on two vector<char>
 vector<uint8_t> AES2::performXOR(vector<uint8_t> va, vector<uint8_t> vb) {
	 //local variable
	 vector<uint8_t> temp;
	 uint8_t j = 0;

	 //compare each elements in same index and XOR
	 for (int i = 0; i < 4; i++) {
		 j = va[i] ^ vb[i];
		 temp.push_back(j);
	 }

	 return temp;
 }

 //displays all of the words in words vector
 void AES2::displayKeys() {

	 int i = 0;
	 int j = 0;

	 for (; i < Keys.size(); i++) {
		 cout << "w[ " << dec << i << " ] = ";
		 for (; j < 4; j++) {
			 cout << hex << (int)Keys[i][j] << " ";
		 }
		 j = 0;
		 cout << endl;
	 }
 }

 // copys the input vector to the state vectord
 void AES2::generateState() {
	 state = input;
 }

 //substitutes each byte in all the state blocks
 void AES2::SubStateBlocks() {

	 //local variables
	 int x = 0;
	 int y = 0;

		 for (int j = 0; j < state.size(); j++) {
			 //calculate grid coordinate for subsituition look up
			 x = state[j] / 16;
			 y = state[j] % 16;

			 //look up and substitute
			 state[j] = S_Box[x][y];
		 }
	 
 }

 //Left shifts the vectors in State blocks
 void AES2::ShiftRows() {

		 //local variable
		 uint8_t temp = state[1];

		 //shift second row in state block
		 for (int j = 0; j < 3; j++) {

			 state[1 + 4 * j] = state[1 + 4 * (j + 1)];
		 }
		 state[13] = temp;

		 //shift third row in state block
		 for (int j = 0; j < 2; j++) {
			 temp = state[2 + 4 * (j + 2)];
			 state[2 + 4 * (j + 2)] = state[2 + (4 * j)];
			 state[2 + (4 * j)] = temp;
		 }

		 //shift fourth row
		 temp = state[15];
		 for (int j = 3; j > 0; --j) {
			 state[3 + (4 * (j))] = state[3 + (4 * (j - 1))];
		 }
		 state[3] = temp;
	 

 }

 //mixes the columns for state block
 void AES2::MixColmns() {
		
		//local variables
		uint8_t a, b, c, d;
		 //go through each column
		 for (int j = 0; j < 4; j++) {

			 //generate new column
			a = (GFMul2(2, state[4 * j])) ^ (GFMul2(3, state[4 * j + 1])) ^ state[4 * j + 2] ^ state[4 * j + 3];
			b = state[4 * j] ^ (GFMul2(2, state[4 * j + 1])) ^ (GFMul2(3, state[4 * j + 2])) ^ state[4 * j + 3];
			c = state[4 * j] ^ state[4 * j + 1] ^ (GFMul2(2, state[4 * j + 2])) ^ (GFMul2(3, state[4 * j + 3]));
			d = (GFMul2(3, state[4 * j])) ^ state[4 * j + 1] ^ state[4 * j + 2] ^ (GFMul2(2, state[4 * j + 3]));

			state[4 * j] = a;
			state[4 * j +1] = b;
			state[4 * j +2 ] = c;
			state[4 * j+3] = d;
		 }
	 
 }

 //adds the appropriate round keys to state blocks 
 void AES2::AddKeytoState(int rn) {

		 //apply words keys to state blocks
		 for (int j = 0; j < 4; j++) {

			 state[4 * j] = state[4 * j] ^ Keys[4 * rn + j][0];
			 state[4 * j + 1] = state[4 * j + 1] ^ Keys[4 * rn + j][1];
			 state[4 * j + 2] = state[4 * j + 2] ^ Keys[4 * rn + j][2];
			 state[4 * j + 3] = state[4 * j + 3] ^ Keys[4 * rn + j][3];
		 }
	

 }

 //perfroms Galois field multiplication with one int and a char
 uint8_t AES2::GFMul(uint8_t in, uint8_t a) {
	 //local variable
	 uint8_t p = 0;
	 uint8_t hi_bit_set;
	 for (int counter = 0; counter < 8; counter++) {
		 if (((uint8_t)a & (uint8_t)1) != 0) {
			 p ^= in;
		 }

		 hi_bit_set =(uint8_t) in & 0x80;

		 in <<= 1;

		 if (hi_bit_set != 0) {
			 in ^= 0x1b;
		 }
		 a >>= 1;
	 }

	 return p;
 }

 //second function
 uint8_t AES2::GFMul2(byte a, byte b) {
	 byte p = 0;
	 byte hi_bit_set;
	 for (int counter = 0; counter < 8; counter++) {
		 if ((b & byte(1)) != 0) {
			 p ^= a;
		 }
		 hi_bit_set = (byte)(a & byte(0x80));
		 a <<= 1;
		 if (hi_bit_set != 0) {
			 a ^= 0x1b; /* x^8 + x^4 + x^3 + x + 1 */
		 }
		 b >>= 1;
	 }



	 return p.to_ulong();
 }

 //encrypts the state vector
 void AES2::encrypt() {
	generateState();
	generateKeys();
	AddKeytoState(0);

	for (int i = 1; i < 10; i++) {
		SubStateBlocks();
		ShiftRows();
		MixColmns();
		AddKeytoState(i);
	}

	SubStateBlocks();
	ShiftRows();
	AddKeytoState(10);
	generateOutput();
	OutPutFile(0);
 }

 //copys the state vector to the output vector
 void AES2::generateOutput() {
	 //output = state;
	 for (uint8_t u : state) {
		 output.push_back(u);
	 }
 }

 //displays the output vector
 void AES2::DisplayOutput() {
	 DisplayVector(output);
 }

 //inverse shift rows
 void AES2::InvShiftRows() {

		 //local variable
		 uint8_t temp = state[13];

		 //shift second row in state block
		 for (int j = 3; j > 0; --j) {
			 state[4 * j + 1] = state[4 * (j - 1) + 1];
		 }
		 state[1] = temp;

		 //shift third row in state block
		 for (int j = 0; j < 2; j++) {
			 temp = state[2 + 4 * (j + 2)];
			 state[2 + 4 * (j + 2)] = state[2 + (4 * j)];
			 state[2 + (4 * j)] = temp;
		 }

		 //shift fourth row
		 temp = state[3];
		 for (int j = 0; j < 3; j++) {

			 state[3 + 4 * j] = state[3 + 4 * (j + 1)];
		 }
		 state[15] = temp;
	 
 }

 //inverse mix columns 
 void AES2::InvMixColumns() {
	 //local varaiables
	 uint8_t a, b, c, d;
	 //go through each state block in vector

		 //go through each column
	 for (int j = 0; j < 4; j++) {

		 //generate new column

		 a = (GFMul2(14, state[4 * j])) ^ (GFMul2(11, state[4 * j + 1])) ^ (GFMul2(13, state[4 * j + 2])) ^ (GFMul2(9, state[4 * j + 3]));
		 b = (GFMul2(9, state[4 * j])) ^ (GFMul2(14, state[4 * j + 1])) ^ (GFMul2(11, state[4 * j + 2])) ^ (GFMul2(13, state[4 * j + 3]));
		 c = (GFMul2(13, state[4 * j])) ^ (GFMul2(9, state[4 * j + 1])) ^ (GFMul2(14, state[4 * j + 2])) ^ (GFMul2(11, state[4 * j + 3]));
		 d = (GFMul2(11, state[4 * j])) ^ (GFMul2(13, state[4 * j + 1])) ^ (GFMul2(9, state[4 * j + 2])) ^ (GFMul2(14, state[4 * j + 3]));

		 state[4 * j] = a;
		 state[4 * j + 1] = b;
		 state[4 * j + 2] = c;
		 state[4 * j + 3] = d;

	 }
 }

 //reverse table susbtitution to state elements
 void AES2::InvSubStateBlocks() {

	 //local variables
	 int x = 0;
	 int y = 0;

		 for (int j = 0; j < state.size(); j++) {
			 //calculate grid coordinate for reverse subsituition look up
			 x = state[j] / 16;
			 y = state[j] % 16;

			 //look up and substitute
			 state[j] = Inv_S_Box[x][y];
		 }
	 
 }

 //decrypts state vector
 void AES2::decrypt() {
	generateState();
	generateKeys();

	AddKeytoState(10);
	
	for (int i = 9; i > 0; i--) {
		InvShiftRows();
		InvSubStateBlocks();
		AddKeytoState(i);
		InvMixColumns();
	}

	InvShiftRows();
	InvSubStateBlocks();
	AddKeytoState(0);

	generateOutput();
	OutPutFile(1);

 }

 //read a file and puts it in input vector
 void AES2::ReadFile() {
	 //local varables
	 vector<uint8_t> in;
	 fstream fin;

	 // Open input file
	 // ios::binary- reading file
	 // character by character
	 fin.open(fileName, fstream::in);


	 // Reading original file till
	 // end of file
	 while (fin >> noskipws >> c) {

		 in.push_back(c);

	 }

	 input = in;

	 // Close file
	 fin.close();

	 //data formatted into 128-bit/ 16 bytes blocks
	 cout << "File read!" << endl;

 }

 void AES2::OutPutFile(int n) {
	 //local variables
	fstream fout;
	string s;
	//determine file name based on int n
	if (n == 1) {
		s = "decryptedText.txt";
	}
	else
	{
		s = "encryptedText.txt";
	}
	fout.open(s, fstream::out);
	//go through each state block's elements
	for (int j = 0; j < output.size(); j++) {
		fout << (char)output[j];
	}

	fout.close();

	if (n == 1) {
		cout << "File decrypted!" << endl;
	}
	else
	{
		cout << "File encrypted!" << endl;
	}
 }

//reads text file and creates 16-byte Input block arrays
 void AES2::GenerateInputBlocks() {
	 //local varables
	 vector<uint8_t> in;
	 fstream fin;

	 // Open input file
	 // ios::binary- reading file
	 // character by character
	 fin.open(fileName, fstream::in);


	 // Reading original file till
	 // end of file
	 while (fin >> noskipws >> c) {

		 //filling all 16 spaces in array
		 if (in.size() < 16) {
			 in.push_back(c);
		 }
		 //adding 16-byte input block to inputBlocks vector
		 //then clears the vector for next 16-byte inputs
		 //add current input to first element of new 16-byte inputs
		 else if (in.size() == 16) {
			inputBlocks.push_back(in);
			 in.clear();
			 in.push_back(c);
		 }

	 }

	 //fill the remaining vector with empty spaces and add to input blocks
	 while (in.size() < 16) {
		 in.push_back(32);
	 }
	 inputBlocks.push_back(in);


	 // Close file
	 fin.close();

	 //data formatted into 128-bit/ 16 bytes blocks
	 cout << "File read!" << endl;

 }

 //displays the inputs as 16-byte blocks
 void AES2::DisplayInputBlocks() {
	 for (int i = 0; i < inputBlocks.size(); i++) {
		 DisplayVector(inputBlocks[i]);
	 }
 }

 //encrypts all of the inputBlocks
 void AES2::encrypt2() {
	 generateKeys();
	 //go through all inputBlocks
	 for (int i = 0; i < inputBlocks.size(); i++) {
		 input = inputBlocks[i];
		 generateState();

		 AddKeytoState(0);

		 for (int i = 1; i < 10; i++) {
			 SubStateBlocks();
			 ShiftRows();
			 MixColmns();
			 AddKeytoState(i);
		 }

		 SubStateBlocks();
		 ShiftRows();
		 AddKeytoState(10);
		 generateOutput();

	 }

	 OutPutFile(0);
 }

 //decrypts state vector
 void AES2::decrypt2() {
	
	 generateKeys();

	 //go through all inputBlocks
	 for (int i = 0; i < inputBlocks.size(); i++) {
		 input = inputBlocks[i];
		 generateState();
		 
		 AddKeytoState(10);

		 for (int i = 9; i > 0; i--) {
			 InvShiftRows();
			 InvSubStateBlocks();
			 AddKeytoState(i);
			 InvMixColumns();
		 }

		 InvShiftRows();
		 InvSubStateBlocks();
		 AddKeytoState(0);

		 generateOutput();
	 }
	 OutPutFile(1);

 }