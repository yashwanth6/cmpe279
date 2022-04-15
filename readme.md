Instructions to run:<br />
For Server:<br />
make server<br />
sudo ./server<br />
For Client:<br />
make client<br />
./client<br />

Details:<br />
Previlige Seperation.<br /> Set the process id of child to nobody's id. Then they wont have sudo previliges. Parent will wait until the clild process/ Previlige drop is done.
