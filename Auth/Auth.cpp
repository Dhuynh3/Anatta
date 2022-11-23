#include "../Auth/Auth.h"



Connection::Connection() {

    wsPtr = WebSocketClient::newWebSocketClient(serverString); // Initialize a connection pointer
    auto req = HttpRequest::newHttpRequest(); //
    req->setPath(path);
    printf("im calleld\n");

}

void Connection::Run() {
    app().setLogLevel(trantor::Logger::kDebug);
    app().run();
}