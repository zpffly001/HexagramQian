#include "App.h"
#include "json.hpp"
#include <glog/logging.h>

using namespace std;
using namespace restbed;
using json = nlohmann::json;

void App::start()
{
    LOG(INFO) << "Start web server at port: " << port;
}

