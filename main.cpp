#include "server.h"
#include "client.h"

int main() {
    Server server("127.0.0.1", 8888);
    server.start();

    return 0;
}
