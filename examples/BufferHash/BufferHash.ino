// Binary Buffer Hash Example
// This example demonstrates hashing binary data (not just strings)
// Useful for checksumming sensor data, buffers, or file contents

#include <MD5.h>

#ifndef Arduino_h
	#include <Arduino.h>
#endif

void setup() {
	// Start Serial Communication
	Serial.begin(115200);
	delay(100);
	
	Serial.println("=== MD5 Binary Buffer Example ===");
	Serial.println();

	// Example 1: Hash a binary buffer (sensor data simulation)
	Serial.println("Example 1: Hashing binary sensor data");
	
	// Simulate sensor data - 32 bytes of measurements
	uint8_t sensor_data[32];
	
	// Fill with simulated sensor readings
	for (int i = 0; i < 32; i++) {
		sensor_data[i] = (i * 7 + 42) & 0xFF;  // Pseudo-random data
	}
	
	// Print raw data
	Serial.print("Sensor Data: ");
	for (int i = 0; i < 32; i++) {
		if (sensor_data[i] < 16) Serial.print("0");
		Serial.print(sensor_data[i], HEX);
	}
	Serial.println();
	
	// Hash the binary data using one-shot method
	uint8_t hash1[16];
	MD5::Hash(sensor_data, 32, hash1);
	
	char digest1[33];
	MD5::Digest(hash1, digest1);
	
	Serial.print("Hash: ");
	Serial.println(digest1);
	Serial.println();

	// Example 2: Hash mixed data types
	Serial.println("Example 2: Hashing structured data");
	
	// Define a simple data structure
	struct DataPacket {
		uint16_t timestamp;
		uint8_t sensor_id;
		int16_t temperature;
		uint16_t humidity;
	};
	
	DataPacket packet;
	packet.timestamp = 1234;
	packet.sensor_id = 5;
	packet.temperature = 2345;
	packet.humidity = 6789;
	
	// Hash the entire structure
	uint8_t packet_hash[16];
	MD5::Hash(&packet, sizeof(DataPacket), packet_hash);
	
	char packet_digest[33];
	MD5::Digest(packet_hash, packet_digest);
	
	Serial.print("Packet Size: ");
	Serial.print(sizeof(DataPacket));
	Serial.println(" bytes");
	Serial.print("Packet Hash: ");
	Serial.println(packet_digest);
	Serial.println();

	// Example 3: Streaming binary data
	Serial.println("Example 3: Streaming binary data chunks");
	
	MD5 md5;
	
	// Simulate reading binary data in chunks
	uint8_t data[256];
	
	// Fill with pattern
	for (int i = 0; i < 256; i++) {
		data[i] = (i * 13) & 0xFF;
	}
	
	// Process in 64-byte chunks
	for (int chunk = 0; chunk < 4; chunk++) {
		md5.Update(&data[chunk * 64], 64);
		Serial.print("Chunk ");
		Serial.print(chunk + 1);
		Serial.println(" added");
	}
	
	uint8_t streaming_hash[16];
	md5.Finalize(streaming_hash);
	
	char streaming_digest[33];
	MD5::Digest(streaming_hash, streaming_digest);
	
	Serial.print("Streamed Hash: ");
	Serial.println(streaming_digest);
	Serial.println();

	// Example 4: Empty buffer test
	Serial.println("Example 4: Edge cases");
	
	uint8_t empty_data[1] = {0};
	uint8_t empty_hash[16];
	MD5::Hash(empty_data, 0, empty_hash);  // Hash 0 bytes
	
	char empty_digest[33];
	MD5::Digest(empty_hash, empty_digest);
	
	Serial.print("Empty buffer hash: ");
	Serial.println(empty_digest);
	Serial.println();
	
	Serial.println("=== Binary Buffer Test Complete ===");
}

void loop() {
	// Nothing to do
}
