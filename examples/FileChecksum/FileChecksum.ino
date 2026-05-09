// File Checksum Example
// This example demonstrates how to calculate MD5 checksums of data
// Perfect for verifying file integrity, firmware validation, or data integrity checks

#include <MD5.h>

#ifndef Arduino_h
	#include <Arduino.h>
#endif

// Simulate file data stored in PROGMEM (flash memory)
const char file_data[] PROGMEM = 
	"This is a simulated file content.\n"
	"It contains multiple lines of text.\n"
	"In a real scenario, this could be read from:\n"
	"- SD Card\n"
	"- EEPROM\n"
	"- External Flash\n"
	"- Network stream\n"
	"For now, we simulate file reading.";

void setup() {
	// Start Serial Communication
	Serial.begin(115200);
	delay(100);
	
	Serial.println("=== MD5 File Checksum Example ===");
	Serial.println();

	// Example 1: Calculate checksum of file in memory
	Serial.println("Example 1: File Checksum from PROGMEM");
	
	size_t file_size = strlen_P(file_data);
	
	Serial.print("File size: ");
	Serial.print(file_size);
	Serial.println(" bytes");
	
	// Calculate checksum by reading in chunks (simulates SD card reading)
	MD5 md5_file;
	
	// Read file in 32-byte chunks
	const size_t CHUNK_SIZE = 32;
	uint8_t buffer[CHUNK_SIZE];
	
	for (size_t offset = 0; offset < file_size; offset += CHUNK_SIZE) {
		// Simulate reading from file
		size_t to_read = (file_size - offset > CHUNK_SIZE) ? CHUNK_SIZE : (file_size - offset);
		
		// Copy from PROGMEM
		memcpy_P(buffer, (const void*)(file_data + offset), to_read);
		
		md5_file.Update(buffer, to_read);
		
		Serial.print("Read chunk at offset ");
		Serial.print(offset);
		Serial.print(": ");
		Serial.print(to_read);
		Serial.println(" bytes");
	}
	
	uint8_t file_hash[16];
	md5_file.Finalize(file_hash);
	
	char file_digest[33];
	MD5::Digest(file_hash, file_digest);
	
	Serial.print("File MD5: ");
	Serial.println(file_digest);
	Serial.println();

	// Example 2: Verify file integrity (one-shot method)
	Serial.println("Example 2: Quick verification with One-shot Method");
	
	// Simulate reading entire file at once
	char file_copy[256];
	size_t file_len = 0;
	
	while (file_len < sizeof(file_copy) - 1) {
		char ch = (char)pgm_read_byte(&file_data[file_len]);
		if (ch == '\0') {
			break;
		}
		file_copy[file_len] = ch;
		file_len++;
	}
	file_copy[file_len] = '\0';
	
	uint8_t quick_hash[16];
	MD5::Hash(file_copy, file_len, quick_hash);
	
	char quick_digest[33];
	MD5::Digest(quick_hash, quick_digest);
	
	Serial.print("Quick Hash: ");
	Serial.println(quick_digest);
	
	// Verify it matches
	if (strcmp(file_digest, quick_digest) == 0) {
		Serial.println("✓ Checksums match - File integrity verified!");
	} else {
		Serial.println("✗ Checksums don't match - File corrupted!");
	}
	Serial.println();

	// Example 3: Compare two files
	Serial.println("Example 3: File Comparison");
	
	// Simulate different file data
	const char other_file[] = "Different file content here.";
	size_t other_size = strlen(other_file);
	
	uint8_t other_hash[16];
	MD5::Hash(other_file, other_size, other_hash);
	
	char other_digest[33];
	MD5::Digest(other_hash, other_digest);
	
	Serial.print("Original file hash: ");
	Serial.println(file_digest);
	Serial.print("Other file hash:    ");
	Serial.println(other_digest);
	
	if (strcmp(file_digest, other_digest) == 0) {
		Serial.println("Files are identical");
	} else {
		Serial.println("Files are different");
	}
	Serial.println();

	// Example 4: Track file modifications
	Serial.println("Example 4: Version tracking with checksums");
	
	const char versions[][32] = {
		"Version 1 Content",
		"Version 2 Modified",
		"Version 3 Updated"
	};
	
	Serial.println("File Version Checksums:");
	
	for (int i = 0; i < 3; i++) {
		uint8_t version_hash[16];
		MD5::Hash(versions[i], strlen(versions[i]), version_hash);
		
		char version_digest[33];
		MD5::Digest(version_hash, version_digest);
		
		Serial.print("v");
		Serial.print(i + 1);
		Serial.print(": ");
		Serial.println(version_digest);
	}
	
	Serial.println();
	Serial.println("=== File Checksum Test Complete ===");
}

void loop() {
	// Nothing to do
}
