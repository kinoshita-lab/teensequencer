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
        };

        uint8_t note;
        int8_t parameter1;
        int8_t parameter2;
        int8_t parameter3;
        int8_t parameter4;

        RecordData
        {
            note = SEQ_NONE;
            parameter1 = SEQ_NONE;
            parameter2 = SEQ_NONE;
            parameter3 = SEQ_NONE;
            parameter4 = SEQ_NONE;
        }
    };

public:
    Sequencer() {
    }
    virtual ~Sequencer() {}

    auto tick() -> void {

    }

private:
    uint8_t step; // 0 - 15
    RecordData recordedData[NUM_INSTRUMENT][NUM_STEP];
};

};