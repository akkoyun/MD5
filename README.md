# MD5 Hash Library

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Arduino](https://img.shields.io/badge/Platform-Arduino-blue.svg)](https://www.arduino.cc)
![Version](https://img.shields.io/badge/Version-1.3.0-blue.svg)

Lightweight, low-RAM, heap-free MD5 library for Arduino and embedded targets.

This implementation is written and maintained in this repository as its own library, with a single default low-RAM mode.

## Why This Library

- Default low-RAM design only (no high-performance mode toggle)
- Small instance footprint: `sizeof(MD5) = 84` bytes
- Heap allocation: `0` bytes
- Streaming support for chunked hashing (`Update` + `Finalize`)
- Defensive API checks for null pointers
- Portable naming to avoid ESP macro collisions
- Works on Arduino Uno (32KB flash), Mega, ESP8266, ESP32

## RAM-Based Comparison

| Metric | This Library (v1.2.0) | tzikis/ArduinoMD5 (Reference) |
| --- | --- | --- |
| RAM per `MD5` instance | **84 bytes** | ~152 bytes |
| Heap usage | 0 bytes | 0 bytes |
| Internal buffer | 64 bytes | 64 bytes |
| `digits[]` lookup table | 0 bytes RAM (PROGMEM on AVR) | 17 bytes RAM |
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

    Serial.println(digest);  // 5eb63bbbe01eeed093cb22bb8f5acdc3
}

void loop() {}
```

## Memory Notes

- `sizeof(MD5) = 84` bytes (stack-allocated, no heap)
- Heap usage: `0` bytes
- Internal buffer: `64` bytes (required for streaming)
- `digits[]` lookup: `0` bytes RAM on AVR (stored in PROGMEM)

### PlatformIO Build Results (v1.2.0)

| Board | Flash | RAM (static) |
| ----- | ----- | ------------ |
| Arduino Uno (ATmega328P, 2KB) | 13,708 / 32,256 bytes (42%) | 756 / 2,048 bytes (37%) |
| Arduino Mega (ATmega2560, 8KB) | 14,056 / 253,952 bytes (5%) | 756 / 8,192 bytes (9%) |
| ESP32 | 272,725 / 1,310,720 bytes (21%) | 21,464 / 327,680 bytes (7%) |
| ESP8266 | 271,427 / 1,044,464 bytes (26%) | 28,648 / 81,920 bytes (35%) |

Static RAM on AVR is dominated by the Arduino Serial library (~190 bytes). The MD5 library itself contributes **0 bytes** of static RAM.

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
- PlatformIO: `pio pkg install --library "akkoyun/MD5"`
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
