# Internet Statistics
Current Status: On the way to timestamping packets on the server side in order to estimate
jitter. Similarly, the client will change its packet generation to contain packets which
keep track of time stamps to keep track of RTT and thus estimate latency when this system
is run for a long period of time. 

Note this functionality is not currently finished as there is a bug in the server 
timestamping code. Stay tuned!

And yes that is correct, I don't have any makefiles yet! Don't worry-these are coming soon :)
