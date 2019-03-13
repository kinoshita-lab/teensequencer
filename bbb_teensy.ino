#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include "MsTimer2.h"

// GUItool: begin automatically generated code
AudioSynthWaveformSine   sine1;          //xy=333,282
AudioSynthWaveformSine   sine2;          //xy=346,361
AudioOutputAnalogStereo  dacs1;          //xy=659,282
AudioConnection          patchCord1(sine1, 0, dacs1, 0);
AudioConnection          patchCord2(sine2, 0, dacs1, 1);
// GUItool: end automatically generated code


constexpr int LED_PIN = 13;

struct Bpm
{
    float bpm;    
    uint32_t count;
    uint32_t countMax;
    uint8_t posIn16th;

    Bpm() : bpm(120.0), count(0), countMax(0xFFFFFFFF), posIn16th(0) {}

    auto tick1ms() -> void {
        count++;

        if (count >= countMax) {
            count = 0;
            posIn16th++;
        
            if (posIn16th >= 4) {
                posIn16th = 0;
            }

            const auto head = posIn16th == 0;
            const auto beatLedStatus = head ? HIGH : LOW;
            digitalWrite(LED_PIN, beatLedStatus);            
        }
    };
};

Bpm bpm;

auto timer2Interrupt() -> void {
    bpm.tick1ms();
}

auto changeBpm(const auto newBpm) -> void {
	auto tmp = 60 * 1000.0 / newBpm;
	tmp /= 4.0;
	bpm.countMax = (uint32_t)tmp;
}

auto setup() -> void {
    pinMode(LED_PIN, OUTPUT);
    MsTimer2::set(1, timer2Interrupt);
    MsTimer2::start();

    changeBpm(120.0);

    Serial.begin(115200);
    Serial.println("Hello World");
    AudioMemory(15);

    AudioNoInterrupts();
    sine1.amplitude(1.0);
    sine1.frequency(440.0);
    sine1.phase(0);

    sine2.amplitude(1.0);
    sine2.frequency(880.0);
    sine2.phase(0);
    AudioInterrupts();
}

auto mappedBpm = 1200;

auto loop() -> void {
    const auto bpmAdValue = analogRead(0) >> 2 << 2;
    const auto mappedValue = map(bpmAdValue, 0, 1023, 200, 3000);

    if (mappedBpm != mappedValue) {
        mappedBpm = mappedValue;
        changeBpm(mappedValue / 10.0);
    }
}
