Instructions to run:
For Server:
make server
sudo ./server
For Client:
make client
./client

Details:
Previlige Seperation. Set the process id of child to nobody's id. Then they wont have sudo previliges. Parent will wait until the clild process/ Previlige drop is done.
