#include <iostream>

#include "RemoteTuning.h"

bool testRoundTrip()
{
    double inputFrequencies[128];
    double outputFrequencies[128];

    MTSClient *client = MTS_RegisterClient();
    for(int i = 0; i < 128; ++i)
    {
        inputFrequencies[i] = MTS_NoteToFrequency(client, i, 0);
    }
    MTS_DeregisterClient(client);

    bool passed = true;

    jsonToFrequencies(tuningToJson(), outputFrequencies);
    for(int i = 0; i < 128; ++i)
    {
        if (inputFrequencies[i] != outputFrequencies[i])
        {
            passed = false;
            std::cout << "Different frequency at " << i << " " << inputFrequencies[i] - outputFrequencies[i] << std::endl;
        }
    }
    return passed;
}

int main()
{
    bool passed;
    passed = testRoundTrip();
    return !passed;
}
