#include <csignal>

#include "constants.h"
#include "RemoteTuning.h"

void deregisterMasterHandler(int signum)
{
    if (isMaster)
    {
        std::cout << "Deregistering MTS-ESP master after receiving signal " << signum << std::endl;
        MTS_DeregisterMaster();
    }
    std::exit(128 + signum);
}

int main()
{
    std::signal(SIGINT, deregisterMasterHandler);
    std::signal(SIGTERM, deregisterMasterHandler);

    httplib::Server svr;

    svr.Get(ENDPOINT, sendTuning);
    svr.Post(ENDPOINT, setTuning);

    std::cout << "Listening on port " << PORT << std::endl;
    svr.listen("0.0.0.0", PORT);
}
