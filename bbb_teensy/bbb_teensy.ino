#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce2.h>

#include "AudioSampleSnare.h"
#include "AudioSampleCh.h"

#include "MsTimer2.h"

// GUItool: begin automatically generated code
AudioSynthWaveformSine   sine1;          //xy=333,282
AudioSynthWaveformSine   sine2;          //xy=346,361
AudioOutputAnalogStereo  dacs1;          //xy=659,282
AudioSynthSimpleDrum drum1;
AudioSynthSimpleDrum drum2;
AudioPlayMemory playMem1;
AudioPlayMemory playMem2;
AudioSynthNoiseWhite noise1;
AudioEffectEnvelope noise1_envelope;

AudioMixer4              mixer1;

AudioConnection          patchCord1(drum1, 0, mixer1, 0);
AudioConnection          patchCord2(playMem1, 0, mixer1, 1);

AudioConnection          pathCord3(noise1, noise1_envelope);
AudioConnection          pathCord4(noise1_envelope, 0, mixer1, 2);

AudioConnection          pathCord5(playMem2, 0, mixer1, 3);

AudioConnection          patchCord5(mixer1, 0, dacs1, 0);
AudioConnection          patchCord6(mixer1, 0, dacs1, 1);


//AudioConnection          patchCord2(sine2, 0, dacs1, 1);
// GUItool: end automatically generated code


constexpr int LED_PIN = 13;
constexpr int SW_1_PIN = 2;
constexpr int SW_2_PIN = 3;
constexpr int SW_3_PIN = 4;
constexpr int SW_4_PIN = 5;

Bounce sw1 = Bounce();
Bounce sw2 = Bounce();
Bounce sw3 = Bounce();
Bounce sw4 = Bounce();

struct Bpm
{
    float bpm;    
    uint32_t count;
    uint32_t countMax;
    uint8_t pos; // 0 - 15
    Bpm() : bpm(120.0), count(0), countMax(0xFFFFFFFF), pos(0) {}

    auto tick1ms() -> void {
        count++;

        if (count >= countMax) {
            count = 0;
            pos++;
            if (pos == 16) {
                pos = 0;
            }
        }

        if (count <= countMax / 2) {
            digitalWrite(LED_PIN, HIGH);
        } else {
            digitalWrite(LED_PIN, LOW);
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
    pinMode(SW_1_PIN, INPUT_PULLUP);
    pinMode(SW_2_PIN, INPUT_PULLUP);
    pinMode(SW_3_PIN, INPUT_PULLUP);
    pinMode(SW_4_PIN, INPUT_PULLUP);

    drum1.frequency(60);
    drum1.length(1500);
    drum1.secondMix(1.0);
    drum1.pitchMod(0.53);

    noise1.amplitude(0.5);
    noise1_envelope.attack(1);
    noise1_envelope.decay(100);
    noise1_envelope.sustain(0);

    sw1.attach(SW_1_PIN);
    sw1.interval(10);

    sw2.attach(SW_2_PIN);
    sw2.interval(10);

    sw3.attach(SW_3_PIN);
    sw3.interval(10);

    sw4.attach(SW_4_PIN);
    sw4.interval(10);


    MsTimer2::set(1, timer2Interrupt);
    MsTimer2::start();

    changeBpm(120.0);

    Serial.begin(115200);
    Serial.println("Hello World");
    AudioMemory(15);

    AudioNoInterrupts();

    AudioInterrupts();
}

auto mappedBpm = 1200;

auto loop() -> void {
    // update switches
    sw1.update();
    sw2.update();
    sw3.update();
    sw4.update();

    if (sw1.fell()) {
        drum1.noteOn();
    }

    if (sw2.fell()) {
        //drum2.noteOn();
        playMem1.play(AudioSampleSnare);
    }

    if (sw3.fell()) {
        noise1_envelope.noteOn();
    }

    if (sw4.fell()) {
        playMem2.play(AudioSampleCh);
    }

    // update bpm
    const auto bpmAdValue = analogRead(0) >> 2 << 2;
    const auto mappedValue = map(bpmAdValue, 0, 1023, 200, 3000);

    if (mappedBpm != mappedValue) {
        mappedBpm = mappedValue;
        changeBpm(mappedValue / 10.0);
    }
}
