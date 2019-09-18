#include <iostream>
#include "pbuilder/Runner.h"
#include "SimpleWeb/server_http.hpp"

using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;

//#define DEBUG

int main() {
    HttpServer server;
    server.config.port = 9005;
    server.config.thread_pool_size = 4;
    server.config.timeout_content = 10;

    auto runner = pbuilder::Runner::create();

    server.resource["^/$"]["POST"] = [runner](std::shared_ptr<HttpServer::Response> response,
                                              std::shared_ptr<HttpServer::Request> request) {
#ifndef DEBUG
        try {
#endif
            response->write(runner->run(request->content.string()));
#ifndef DEBUG
        }
        catch (const std::exception &e) {
            response->write(SimpleWeb::StatusCode::client_error_bad_request, e.what());
        }
#endif
    };

    server.on_error = [](std::shared_ptr<HttpServer::Request> /*request*/, const SimpleWeb::error_code & /*ec*/) {
        // Handle errors here
        // Note that connection timeouts will also call this handle with ec set to SimpleWeb::errc::operation_canceled
    };

    std::thread server_thread([&server]() {
        // Start server
        server.start();
    });

    server_thread.join();

    return 0;
}