// Streaming Hash Example - Large Data Processing
// This example shows how to hash large data by processing it in chunks
// Perfect for reading from EEPROM, SD card, or serial stream

#include <MD5.h>

#ifndef Arduino_h
	#include <Arduino.h>
#endif

void setup() {
	// Start Serial Communication
	Serial.begin(115200);
	delay(100);
	
	Serial.println("=== MD5 Streaming Example ===");
	Serial.println();

	// Example 1: Streaming a large string in chunks
	Serial.println("Example: Hashing 200 bytes in 64-byte chunks");
	
	// Create a large data block (simulating reading from file/EEPROM)
	char large_data[] = "The quick brown fox jumps over the lazy dog. "
	                    "The quick brown fox jumps over the lazy dog. "
	                    "The quick brown fox jumps over the lazy dog. "
	                    "The quick brown fox jumps over the lazy dog.";
	
	size_t data_length = strlen(large_data);
	size_t chunk_size = 64;
	
	MD5 md5;
	
	// Process data in 64-byte chunks
	for (size_t i = 0; i < data_length; i += chunk_size) {
		size_t remaining = data_length - i;
		size_t to_process = (remaining > chunk_size) ? chunk_size : remaining;
		
		md5.Update(&large_data[i], to_process);
		
		Serial.print("Processed chunk: ");
		Serial.print(i);
		Serial.print(" - ");
		Serial.print(i + to_process);
		Serial.print(" (");
		Serial.print(to_process);
		Serial.println(" bytes)");
	}
	
	// Finalize and get result
	uint8_t hash[16];
	md5.Finalize(hash);
	
	// Convert to hex string
	char digest[33];
	MD5::Digest(hash, digest);
	
	Serial.print("Total data length: ");
	Serial.print(data_length);
	Serial.println(" bytes");
	Serial.print("MD5 Hash: ");
	Serial.println(digest);
	Serial.println();
	
	// Example 2: Streaming multiple data sources
	Serial.println("Example: Combining multiple data sources");
	
	MD5 md5_combined;
	
	// Simulate reading from multiple sources
	const char* source1 = "Part1:";
	const char* source2 = "Part2:";
	const char* source3 = "Part3";
	
	md5_combined.Update(source1, strlen(source1));
	Serial.println("Added source 1");
	delay(100);
	
	md5_combined.Update(source2, strlen(source2));
	Serial.println("Added source 2");
	delay(100);
	
	md5_combined.Update(source3, strlen(source3));
	Serial.println("Added source 3");
	
	// Finalize
	uint8_t combined_hash[16];
	md5_combined.Finalize(combined_hash);
	
	char combined_digest[33];
	MD5::Digest(combined_hash, combined_digest);
	
	Serial.print("Combined Hash: ");
	Serial.println(combined_digest);
	Serial.println();
	
	Serial.println("=== Streaming Test Complete ===");
}

void loop() {
	// Nothing to do
}
