#include <fstream>
#include <util/StringUtil.h>
#include <glog/logging.h>
#include "StaticController.h"

using namespace std;
using namespace restbed;

void StaticController::publish(restbed::Service &service)
{
    auto indexResc = make_shared<Resource>();
    indexResc->set_path("/");
    indexResc->set_method_handler("GET", StaticController::indexHandler);

    auto staticResc = make_shared<Resource>();
    staticResc->set_paths({
        "/app/{filename: .*\\.html}",
        "/app/js/{filename: .*\\.js}",
        "/app/css/{filename: .*\\.css}",
        "/app/templates/{filename: .*\\.hbs}"
    });
    staticResc->set_method_handler("GET", StaticController::staticHandler);

    service.publish(indexResc);
    service.publish(staticResc);
}

/**
 * 首页跳转处理
*/
void StaticController::indexHandler(const std::shared_ptr<restbed::Session> session)
{
    const multimap<string, string> headeres{
        {"Location", "/app/index.html"}
    };

    session->close(FOUND, "", headeres);
}