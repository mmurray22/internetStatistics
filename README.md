# Internet Statistics

Current Status:
On the way to timestamping packets on the server side in order to estimate
jitter. Similarly, the client will change its packet generation to contain packets which
keep track of time stamps to keep track of RTT and thus estimate latency when this system
is run for a long period of time. 

Note this functionality is not currently finished as there is a bug in the server 
timestamping code. Stay tuned!

And yes that is correct, I don't have any makefiles yet! Don't worry-these are coming soon :)

# Project Overview:
/*NOTE: ALL INFORMATION BELOW WILL BE UPDATED AND CHANGED WITHIN THE NEXT 24 HRS TO MAKE MORE REASONABLE*/
This repo aims to provide accurate network testing software that allows for characterization and profiling
of jitter and latency of a new, unknown network. 


Project Introduction:
When creating any network-based system, one will have to take into account latency, jitter, and
other such uncertainties of transferring data over a network. In some cases, if one has the ability
to run performance tests on the network before using it to get a sense of the average latency/jitter/etc.
of that network AND be able to integrate performance tests directly into code being used on the 
network to get live performance statistics after each use, this can be a great source of data that will
direct the system design to be most compatible to the particular network. 

Breakdown of /src folder contents:
client.cpp: Program to be run on the client machine which will send 400 packets/sec to the server. The client
will be the program aimed at measuring latency. Specifically, it will have three time values-t_c, t_s, t_c'-
from which it can calculate the RTT and give a tight estimate of MIN_{RTT} and then the latency can be found.

server.cpp: Program to be run on the server machine which will receive packets from the client. The
server will be the program aimed at measuring jitter. Jitter here is defined as the variance of the
latency over time. Thus, to estimate the 'jitter' over the given network, we will use the metric
J = |ts_diff - 2.5ms|, where 2.5ms is the time difference that should exist between each packet and ts_diff is defined as 
ts_diff = ts_{i+1} - ts_{i}
