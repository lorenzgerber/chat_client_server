# Chat Client Server Application
compile and run Server:
```bash
make EXEC=server
./build/server localhost 2000 itchy.cs.umu.se 1337
```
compile and run Client:
```bash
make EXEC=client
./build/client testuser cs localhost 2000
```

for compilation of tests:
```bash
make EXEC=test_suite
make EXEC=test_parse
make EXEC=test_tcp_socket
make EXEC=test_udp_socket
make EXEC=test_server
```
