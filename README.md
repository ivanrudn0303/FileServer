# FileServer
Client Server application for sending files. And uses non-blocking `send/recv` with `epoll` also it informs client and server about download status.
and about any errors occured:
* network errors
* filesystem errors
* connect other client when server has started receiving
## Getting Started
For UNIX:
```Bash
git clone https://github.com/ivanrudn0303/FileServer.git
cd build
make
```
## Usage
### client:
* `-a Address` responsible for the `IPv4` address of server. required
* `-p Port` responsible for the `IPv4` port of server. required
* alternativly `-a "Address:Port"`
* `-o Filename` responsible for the file to be sended to server. required

### server:
* `-a Address` responsible for the `IPv4` address to be listened. by default from any `INADDR_ANY`
* `-p Port` responsible for the `IPv4` port to be listened. required
* alternatively  `-a "Address:Port"`
* `-i Filename` responsible for the file to which data will appended. required

### run:
firstly start server:
```Bash
./build/server -i SaveThere.txt -p 8888
```
then call client:
```Bash
./build/client -o Send.txt -p 8888 -a 127.0.0.1
```
## Testing
### to build tests:
```Bash
make test_init
make test_talk
```
### run:
```Bash
./test_init
./test_talk
```
