# Option 2: Unix Domain Sockets with a Structured Protocol (Simpler Code)
If D-Bus complexity is a showstopper, a simpler, language-independent approach is using Unix domain sockets and defining a simple, structured message format that all languages can easily parse.

- How it works:
- Your C interceptor opens a Unix domain socket at a known file path (e.g., /tmp/kbd_sock).
- It sends a fixed-size binary structure or a simple newline-terminated string (e.g., "KEY_CODE=30\n") when a key is pressed.
- Any other application connects to that socket path and reads the predictable data format.
- Advantage: The C code is much simpler, using standard socket APIs available in every language.
- Disadvantage: It's less integrated into the desktop environment than D-Bus and you must manage the socket file's lifecycle and permissions yourself.

# Method 2: Consuming via Unix Domain Sockets (Simpler Code)
If you chose the Unix Domain Socket approach, consumption is simpler across all languages, as nearly all have standard socket libraries.
Node.js (JavaScript)
Node.js uses its built-in net module to connect to the socket path.
javascript

## Example using Node.js
```bash
const net = require('net');
const SOCKET_PATH = '/tmp/kbd_sock';

const client = net.createConnection({ path: SOCKET_PATH }, () => {
    console.log('Connected to interceptor socket!');
});

client.on('data', (data) => {
    console.log(`Node.js App Received: ${data.toString().trim()}`);
});

client.on('end', () => {
    console.log('Disconnected from server');
});
```


### Go uses its standard library networking package (net).
```bash

// Example using Go
package main

import (
    "fmt"
    "net"
    "os"
    "bufio"
)

func main() {
    conn, err := net.Dial("unix", "/tmp/kbd_sock")
    if err != nil {
        fmt.Println("Error connecting:", err)
        os.Exit(1)
    }
    defer conn.Close()

    reader := bufio.NewReader(conn)
    for {
        message, err := reader.ReadString('\n')
        if err != nil {
            fmt.Println("Connection closed:", err)
            break
        }
        fmt.Printf("Go App Received: %s", message)
    }

```


## Writing to a Unix domain socket involves the standard socket programming API in C, but with a specific address family (
`AF_UNIX or AF_LOCAL)` and a file path rather than an IP address and port.
Here is a simple example of a C program that acts as a client, connecting to a specific Unix domain socket path and sending a message.
Prerequisites
You need a separate process running as a server that creates the socket file and listens for connections and messages first. For this example, we assume a server is listening on `/tmp/kbd_sock`.
C Client: Writing to a Unix Domain Socket
This code connects to the socket and sends a string message.

```bash

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/kbd_sock"
#define MESSAGE "Key code: 30"

int main(void) {
    int socket_fd;
    struct sockaddr_un addr;
    int bytes_sent;

    // 1. Create the socket file descriptor
    // AF_UNIX specifies a Unix domain socket
    // SOCK_STREAM provides a reliable, connection-oriented service (like TCP)
    if ((socket_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    // 2. Set up the socket address structure
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    // Copy the path to the socket structure
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    // 3. Connect to the server
    // The size of the address structure must be calculated correctly for Unix sockets
    if (connect(socket_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("connect error");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to socket %s\n", SOCKET_PATH);

    // 4. Send the data
    bytes_sent = write(socket_fd, MESSAGE, strlen(MESSAGE));
    if (bytes_sent == -1) {
        perror("write error");
    } else {
        printf("Sent %d bytes: %s\n", bytes_sent, MESSAGE);
    }

    // 5. Close the socket
    close(socket_fd);
    return 0;
}
```