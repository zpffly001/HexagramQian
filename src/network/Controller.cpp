#include <blockchain/Blockchain.h>
#include "Controller.h"
#include <network/Endpoint.h>
#include <network/GlogLogger.h>
#include <network/Serializer.h>
#include <blockchain/Blockchain.h>
#include <hash/Hash.h>
#include <huffman/Huffman.h>
#include <search/Search.h>
#include <fstream>

using namespace std;
using namespace restbed;
using json = nlohmann::json;

void BlockController::publish(restbed::Service &service)
{

    /**
     * 获得单个区块
     * GET /api/block/:id/
    */
    Endpoint::httpGet("/api/block/{id: [0-9]*}/", HANDLE_LOGIC(session, response)
    {
        string strId = session.request->get_path_parameter("id");
        UInt id = ::stoi(strId);
        if (id < 0 || id >= Blockchain.size())
        {
            response.code = 404;
            response.message = "请求的区块不存在！";
            return;
        }
        response.data = Blockchain::get(id);
    }).publish(service);
}