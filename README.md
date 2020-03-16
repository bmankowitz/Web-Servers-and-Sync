# Programming Assignment 2: Web Servers and Synchronization

##The name and login information for all project partners.
Benjamin Mankowitz		bmankowi@mail.yu.edu
Ari Roffe				aroffe@mail.yu.edu
## A brief description of how you divided the work between you. 
For the majority of the code, we used the Live Share feature of Visual Studio Code and a phone call to code together and plan our design. For the times that we worked asyncronously, we used a shared git repository to work together remotely, merging changes if needed. 
##Design overview: 
More on part 1

We used the basic web server provided to us with a few modifications. We implemented the scheduling algorithm using two buffers, one high priority (HP) and one low priority (LP). For HPHC and HPIC, we pushed the high priority data to the HP buffer, and other data to the LP buffer. For FIFO and ANY we only used one buffer. To ensure the buffer size is never exceeded, we make sure not to add to the buffer if the buffer1.size + buffer2.size is greater than the maximum provided. When preprocessing the request to determine what type it is, we used the `recv` command with the `MSG_PEEK` flag so that the data is still there for the `web()` function.

For stats that we needed to keep track of, we added all of the job specifc fields in the `job_t` struct. For the thread specific stats, we created a new `t_stats` struct assigned to each thread. We used `gettimeofday()` in conjunction with a local function to get epoch time in milliseconds. To properly return all of the custom headers, we created a function `getStatHeaders()` to provide the header string.

More on client code/part 4

More on daemonization / part 5

##Complete specification: 
Describe how you handled any ambiguities in the specification. For example, how do you implement the ANY policy? 

##Known bugs or problems: 

not yet completed daemonization

A list of any features that you did not implement or that you know are not working correctly 

##Testing: 
This requirement an aspect that I am very interested in. I will assign points for answering these questions. Describe how you tested the functionality of your web server. Describe how can you use the various versions of your extended client to see if the server is handing requests concurrently and implementing the FIFO, HPSC, or HPDC policies. 
Specifically, what are the exact parameters one should pass to the client and the server to demonstrate that the server is handling requests concurrently? To demonstrate that the server is correctly running the FIFO policy? the HPSC policy? the HPDC policy? In each case, if your client and server are behaving correctly, what output and statistics should you see? 
