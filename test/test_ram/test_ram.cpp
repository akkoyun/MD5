#include <Arduino.h>
#include <MD5.h>

// ---------------------------------------------------------------------------
// Free-RAM helpers
// ---------------------------------------------------------------------------

#ifdef __AVR__
extern unsigned int __bss_end;
extern void* __brkval;

static int freeRam() {
    int v;
    return (int)&v - (__brkval ? (int)__brkval : (int)&__bss_end);
}
#else
static int freeRam() { return -1; }
#endif

// ---------------------------------------------------------------------------
// RAM probe: returns how many bytes the stack grew during a call
// ---------------------------------------------------------------------------

static volatile int _before;

static void ramBefore() { _before = freeRam(); }
static int  ramDelta()  { return _before - freeRam(); }

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

static void printSep(const char* title) {
    Serial.println(F("--------------------------------------------"));
    Serial.println(title);
    Serial.println(F("--------------------------------------------"));
}

static void printRam(const char* label, int bytes) {
    Serial.print(F("  "));
    Serial.print(label);
    Serial.print(F(": "));
    if (bytes < 0) Serial.print(F("n/a (non-AVR)"));
    else { Serial.print(bytes); Serial.print(F(" bytes")); }
    Serial.println();
}

// ---------------------------------------------------------------------------
// setup
// ---------------------------------------------------------------------------

void setup() {
    Serial.begin(115200);
    while (!Serial) { delay(10); }
    delay(200);

    Serial.println(F(""));
    Serial.println(F("============================================"));
    Serial.println(F("  MD5 Library  —  RAM Usage Report"));
    Serial.println(F("============================================"));

    // -----------------------------------------------------------------------
    // 1. Class metadata
    // -----------------------------------------------------------------------
    printSep("1. Compile-time sizes");
    Serial.print(F("  sizeof(MD5)              : ")); Serial.print(sizeof(MD5));  Serial.println(F(" bytes"));
    Serial.print(F("  MD5::HASH_SIZE           : ")); Serial.print(MD5::HASH_SIZE);   Serial.println(F(" bytes  (raw hash)"));
    Serial.print(F("  MD5::DIGEST_SIZE         : ")); Serial.print(MD5::DIGEST_SIZE);  Serial.println(F(" bytes  (hex string)"));
    Serial.print(F("  MD5::BLOCK_SIZE          : ")); Serial.print(MD5::BLOCK_SIZE);   Serial.println(F(" bytes  (internal block)"));
    Serial.println();

    // -----------------------------------------------------------------------
    // 2. Free RAM baseline
    // -----------------------------------------------------------------------
    printSep("2. Free RAM baseline");
    int baseline = freeRam();
    printRam("Free RAM at startup", baseline);
    Serial.println();

    // -----------------------------------------------------------------------
    // 3. Instance allocation cost (stack)
    // -----------------------------------------------------------------------
    printSep("3. Instance allocation (stack)");
    {
        ramBefore();
        MD5 md5;
        int after_ctor = ramDelta();
        printRam("Stack consumed by MD5 ctor", after_ctor);
        Serial.print(F("  (expected: ")); Serial.print((int)sizeof(MD5)); Serial.println(F(" bytes)"));
    }
    Serial.println();

    // -----------------------------------------------------------------------
    // 4. Update() stack overhead
    // -----------------------------------------------------------------------
    printSep("4. Update() stack overhead");
    {
        const char* msg = "Hello World!";
        MD5 md5;

        ramBefore();
        md5.Update(msg, strlen(msg));
        int update_stack = ramDelta();
        printRam("Stack during Update()", update_stack);
        Serial.println();
    }

    // -----------------------------------------------------------------------
    // 5. Finalize() stack overhead
    // -----------------------------------------------------------------------
    printSep("5. Finalize() stack overhead");
    {
        const char* msg = "Hello World!";
        MD5 md5;
        md5.Update(msg, strlen(msg));

        uint8_t hash[16];

        ramBefore();
        md5.Finalize(hash);
        int fin_stack = ramDelta();
        printRam("Stack during Finalize()", fin_stack);
        Serial.println();

        // Print the hash to verify correctness
        char digest[33];
        MD5::Digest(hash, digest);
        Serial.print(F("  Hash(\"Hello World!\") : "));
        Serial.println(digest);
        Serial.println(F("  Expected             : 5eb63bbbe01eeed093cb22bb8f5acdc3"));
        Serial.println();
    }

    // -----------------------------------------------------------------------
    // 6. Digest() stack overhead
    // -----------------------------------------------------------------------
    printSep("6. Digest() stack overhead");
    {
        uint8_t hash[16] = {0};
        char digest[33];

        ramBefore();
        MD5::Digest(hash, digest);
        int dig_stack = ramDelta();
        printRam("Stack during Digest()", dig_stack);
        Serial.println();
    }

    // -----------------------------------------------------------------------
    // 7. Full one-shot Hash() stack overhead
    // -----------------------------------------------------------------------
    printSep("7. One-shot Hash() stack overhead");
    {
        const char* msg = "Arduino";
        uint8_t hash[16];

        ramBefore();
        MD5::Hash(msg, hash);
        int hash_stack = ramDelta();
        printRam("Stack during Hash(str)", hash_stack);
        Serial.print(F("  (includes MD5 instance + internals)"));
        Serial.println();
    }

    // -----------------------------------------------------------------------
    // 8. Streaming: multiple Update() calls
    // -----------------------------------------------------------------------
    printSep("8. Streaming (multiple Update calls)");
    {
        const char* parts[] = { "Part1:", "Part2:", "Part3" };
        MD5 md5;
        uint8_t hash[16];

        ramBefore();
        for (int i = 0; i < 3; i++) {
            md5.Update(parts[i], strlen(parts[i]));
        }
        md5.Finalize(hash);
        int stream_stack = ramDelta();

        char digest[33];
        MD5::Digest(hash, digest);

        printRam("Stack during streaming+Finalize", stream_stack);
        Serial.print(F("  Combined hash        : "));
        Serial.println(digest);
        Serial.println();
    }

    // -----------------------------------------------------------------------
    // 9. Buffer hash (binary data)
    // -----------------------------------------------------------------------
    printSep("9. Binary buffer hash");
    {
        uint8_t buf[32];
        for (int i = 0; i < 32; i++) buf[i] = (i * 7 + 42) & 0xFF;

        uint8_t hash[16];

        ramBefore();
        MD5::Hash(buf, 32, hash);
        int buf_stack = ramDelta();

        char digest[33];
        MD5::Digest(hash, digest);

        printRam("Stack during Hash(buf,32)", buf_stack);
        Serial.print(F("  Buffer hash          : "));
        Serial.println(digest);
        Serial.println();
    }

    // -----------------------------------------------------------------------
    // 10. RAM remaining after all tests
    // -----------------------------------------------------------------------
    printSep("10. Final free RAM");
    int final_ram = freeRam();
    printRam("Free RAM after all tests", final_ram);
    if (baseline > 0 && final_ram > 0) {
        printRam("Net RAM consumed (leaks)", baseline - final_ram);
    }
    Serial.println();

    Serial.println(F("============================================"));
    Serial.println(F("  Report Complete"));
    Serial.println(F("============================================"));
}

void loop() {
    // intentionally empty
}
