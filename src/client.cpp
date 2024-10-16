#include <iostream>
#include <chrono>
#include <csignal>

#include "tuning-library/include/Tunings.h"

#include "constants.h"
#include "RemoteTuning.h"

void signalHandler(int signal)
{
    MTS_DeregisterMaster();
    std::exit(128 + signal);
};

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cerr << "Expected to be called with IP address and default tuning scl and kbm as arguments, e.g.\n" << std::endl;
        std::cerr << "$ tuning-client 192.158.1.38 default.scl default.kbm\n" << std::endl;
        std::exit(1);
    }

    if (!MTS_CanRegisterMaster())
    {
        std::cerr << "MTS-ESP master already present" << std::endl;
        std::exit(1);
    }
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    Tunings::Scale s = Tunings::readSCLFile( argv[2] );
    Tunings::KeyboardMapping k = Tunings::readKBMFile( argv[3] );
    Tunings::Tuning t = Tunings::Tuning(s, k);

    MTS_RegisterMaster();

    // Set default tuning (used if no tuning server is running)
    for(int i = 0; i < 128; i++)
    {
        MTS_SetNoteTuning(t.frequencyForMidiNote(i), i);
    }

    httplib::Client cli(std::string(argv[1]), PORT);

    while (true)
    {
        auto res = cli.Get(ENDPOINT);
        setTuningFromResult(res);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
