// Basic Hash Example - Simple string hashing
// This is the simplest example to get started with the MD5 library

#include <MD5.h>

// Define Arduino Library
#ifndef Arduino_h
	#include <Arduino.h>
#endif

// Setup
void setup() {

	// Start Serial
	Serial.begin(115200);
	delay(100);
	
	Serial.println("=== MD5 Basic Hash Example ===");
	Serial.println();

	// Example 1: Simple string hashing using Update + Finalize
	Serial.println("Example 1: String Hash with Update/Finalize");
	
	// Create the MD5 Hashing Class
	MD5 MD5_Hash;

	// Declare Constants
	char Data_Variable[] = "Hello World!";

	// MD5 Checksum - Add data to hash
	MD5_Hash.Update(Data_Variable, sizeof(Data_Variable) - 1);

	// Calculate MD5 - Get the result
	uint8_t _Hash[16];
	MD5_Hash.Finalize(_Hash);

	// MD5 to Char Array - Convert to hex string
	char _Etag[33];
	MD5_Hash.Digest(_Hash, _Etag);

	// Print Data
	Serial.print("MD5 Data:   "); Serial.println(Data_Variable);
	Serial.print("MD5 Hash:   "); Serial.println(_Etag);
	Serial.print("Expected:   5eb63bbbe01eeed093cb22bb8f5acdc3");
	Serial.println();
	Serial.println();

	// Example 2: One-shot method for strings
	Serial.println("Example 2: One-shot String Hash");
	
	const char* test_string = "Arduino";
	unsigned char quick_hash[16];
	
	MD5::Hash(test_string, quick_hash);
	
	char quick_digest[33];
	MD5::Digest(quick_hash, quick_digest);
	
	Serial.print("MD5 Data:   "); Serial.println(test_string);
	Serial.print("MD5 Hash:   "); Serial.println(quick_digest);
	Serial.println();

	// Example 3: Different strings produce different hashes
	Serial.println("Example 3: Different Strings");
	
	const char* strings[] = {
		"test",
		"Test",
		"TEST"
	};
	
	for (int i = 0; i < 3; i++) {
		unsigned char hash[16];
		MD5::Hash(strings[i], hash);
		
		char digest[33];
		MD5::Digest(hash, digest);
		
		Serial.print("\"");
		Serial.print(strings[i]);
		Serial.print("\" -> ");
		Serial.println(digest);
	}
	
	Serial.println();
	Serial.println("=== Basic Hash Test Complete ===");
	
}

// Loop
void loop() {

}
