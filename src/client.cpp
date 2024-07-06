#include <iostream>
#include <chrono>
#include <csignal>

#include "httplib.h"
#include "libMTSMaster.h"

#include "constants.h"
#include "receive_tuning.h"

void signalHandler(int signal)
{
    MTS_DeregisterMaster();
    std::exit(128 + signal);
};

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Expected to be called with IP address as argument" << std::endl;
        std::exit(1);
    }
    if (!MTS_CanRegisterMaster())
    {
        std::cerr << "MTS-ESP master already present" << std::endl;
        std::exit(1);
    }
    std::signal(SIGINT, signalHandler);
    std::signal(SIGKILL, signalHandler);

    MTS_RegisterMaster();

    httplib::Client cli(std::string(argv[1]), PORT);

    while (true)
    {
        auto res = cli.Get(ENDPOINT);
        if (res && res->status == httplib::StatusCode::OK_200)
        {
            double frequencies[128];
            jsonToFrequencies(res->body, frequencies);
            MTS_SetNoteTunings(frequencies);
        }
        else
        {
            auto err = res.error();
            std::cout << "HTTP error: " << httplib::to_string(err) << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
