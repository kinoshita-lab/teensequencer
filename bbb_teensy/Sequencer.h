namespace bbb
{

class Sequencer
{
    enum 
    {
        NUM_INSTRUMENT = 4,
        NUM_STEP = 16,
    };

    struct RecordData
    {
        enum {
            SEQ_NONE = -1,
            NOTE_ON = 1,
        };

        int8_t note;
        int8_t parameter1;
        int8_t parameter2;
        int8_t parameter3;
        int8_t parameter4;

        RecordData()
        {
            clear();
        }

        auto clear() -> void {
            note = SEQ_NONE;
            parameter1 = SEQ_NONE;
            parameter2 = SEQ_NONE;
            parameter3 = SEQ_NONE;
            parameter4 = SEQ_NONE;
        }
    };

public:
    Sequencer() {
        clear();
    }
    virtual ~Sequencer() {}

    auto nextStep() -> void {
        step++;

        if (step >= 16) {
            step = 0;
        }

        Serial.print("Seq: step = ");
        Serial.println(step);
    }

    auto clear() -> void {
        for (auto&& part : recordedData) {
            for (auto&& step : part) {
                step.clear();
            }
        }
    }

    auto rec(const auto index) {
        recordedData[index][step].note = RecordData::NOTE_ON;
    }

    auto noteOnStep(const int index) -> bool {
        Serial.println(recordedData[index][step].note);
        return recordedData[index][step].note != RecordData::SEQ_NONE;
    }



private:
    uint8_t step; // 0 - 15
    RecordData recordedData[NUM_INSTRUMENT][NUM_STEP];
};

};