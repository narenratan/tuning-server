#include "httplib.h"

#include "constants.h"
#include "send_tuning.h"

int main()
{
    httplib::Server svr;

    svr.Get(ENDPOINT, sendTuning);

    std::cout << "Listening on port " << PORT << std::endl;
    svr.listen("0.0.0.0", PORT);
}
