# MD5 Hash Library

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Arduino](https://img.shields.io/badge/Platform-Arduino-blue.svg)](https://www.arduino.cc)
![Version](https://img.shields.io/badge/Version-1.1.0-blue.svg)

Lightweight, low-RAM, heap-free MD5 library for Arduino and embedded targets.

This implementation is written and maintained in this repository as its own library, with a single default low-RAM mode.

## Why This Library

- Default low-RAM design only (no high-performance mode toggle)
- Small instance footprint: `sizeof(MD5) = 88` bytes
- Heap allocation: `0` bytes
- Streaming support for chunked hashing (`Update` + `Finalize`)
- Defensive API checks for null pointers
- Portable naming to avoid ESP macro collisions

## RAM-Based Comparison

| Metric | This Library (Current) | tzikis/ArduinoMD5 (Reference) |
| --- | --- | --- |
| RAM per `MD5` instance | 88 bytes | ~152 bytes |
| Heap usage | 0 bytes | 0 bytes |
| Internal buffer | 64 bytes | 64 bytes |
| Runtime mode | Single default low-RAM path | Block-cache-oriented path |

Note: The comparison is included as a practical RAM benchmark reference.

## Features

- Low-RAM by default
- No heap allocation
- Streaming hash support (`Update` + `Finalize`)
- One-shot helpers (`Hash`, `Digest`)
- RFC 1321 compatible output
- Works on Arduino Uno/Nano/Mega, ESP8266, ESP32

## Quick Start

```cpp
#include <MD5.h>

void setup() {
    Serial.begin(115200);

    MD5 md5;
    md5.Update("Hello World!", 12);

    uint8_t hash[16];
    md5.Finalize(hash);

    char digest[33];
    MD5::Digest(hash, digest);

    Serial.println(digest);
}

void loop() {}
```

## Memory Notes

- `sizeof(MD5) = 88` bytes
- Heap usage: `0` bytes
- Internal buffer: `64` bytes

## API Summary

- `Reset()`
- `Update(const void* data, uint32_t size)`
- `Finalize(uint8_t* result)`
- `static Hash(const char* str, unsigned char* hash)`
- `static Hash(const void* buffer, size_t size, unsigned char* hash)`
- `static Digest(const unsigned char* hash, char* digest)`

## Examples

- [examples/Hash/Hash.ino](examples/Hash/Hash.ino): Basic string hashing
- [examples/StreamingHash/StreamingHash.ino](examples/StreamingHash/StreamingHash.ino): Chunked/streaming hashing
- [examples/BufferHash/BufferHash.ino](examples/BufferHash/BufferHash.ino): Binary buffer hashing
- [examples/FileChecksum/FileChecksum.ino](examples/FileChecksum/FileChecksum.ino): File checksum and verification flow

## Installation

- Arduino IDE Library Manager: search for `MD5`
- Manual ZIP install: `Sketch > Include Library > Add .ZIP Library`
- Git:

```bash
cd ~/Documents/Arduino/libraries
git clone https://github.com/akkoyun/MD5.git
```

## Security Note

MD5 is not secure for cryptographic use cases (passwords, signatures, or modern security-sensitive workflows).
Use this library for compatibility and integrity-check scenarios where MD5 is explicitly required.

## License

MIT - see [LICENSE](LICENSE)
