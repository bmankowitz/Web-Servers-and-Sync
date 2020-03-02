# Programming Assignment 2: Web Servers and Synchronization
Due: Friday, March 13 before Shabbos 
You are encouraged to do this project with a partner.   It is ambitious to tackle alone.
This project must be implemented in C and tested on the class Linux server.
You may develop it in a Linux VM on your laptop, but when I grade, only success on the server counts.

## Four objectives to this assignment: 

1. To modify an existing code base. 
2. To learn how to create and synchronize cooperating threads in Unix. 
3. To gain exposure to how a basic web server is structured. 
4. To think more about how to test the functionality of a code base. 
Notes

## Compilation

When compiling and linking, you should use the argument -pthread to the compiler. This takes care of adding in the right libraries, etc., for using pthreads.   This is built into the Makefile I provide with your project.
It is OK to talk to others in class about what you are doing. It is even OK to explain to someone how some particular things works (e.g., how pthread_create() works). However, it is NOT OK to share code outside your team. If your team is having trouble, come talk to me instead. In this document, code and things you are expected to type are presented in Courier fixed width font.

## Background

In this assignment, you will be developing a real, working web server. To greatly simplify this project, we are providing you with the code for a very basic web server. It will be your job to make the web server multi-threaded so that it is more efficient. 
HTTP Background
Before describing what you will be implementing in this project, we will provide a very brief overview of how a web server works and the HTTP protocol. Our goal in providing you with a basic web server is that you should be shielded from most of the details of network connections and the HTTP protocol. The code that we give you already handles everything that we describe in this section. If you are really interested in the full details of the HTTP protocol, you can read the specification, but we do not require this for this initial project! HTTP is covered in more detail in COM 2512 and COM 3780.

## HTTP

Web browsers and web servers interact using a text-based protocol called HTTP (Hypertext Transfer Protocol). A web browser opens an Internet connection to a web server and requests some content with HTTP. The web server responds with the requested content and closes the connection. The browser reads the content and displays it on the screen. 

## File

Each piece of content on the server is associated with a file. If a client requests a specific disk file, then this is referred to as static content. If a client requests that a executable file be run and its output returned, then this is dynamic content. Each file has a unique name known as a URL (Universal Resource Locator). For example, the URL http://www.yu.edu:80/index.html identifies an HTML file called "/index.html" on Internet host "www.yu.edu" that is managed by a web server listening on port 80. The port number in a URL is optional and defaults to the well-known HTTP port of 80.  (Modern secure web servers use a protocol HTTPS instead of HTTP and listen by default on port 443, but this assignment is old-fashioned.)
An HTTP request (from the web browser or other client to the server) consists of a request line, followed by zero or more request headers, and finally an empty text line (the empty line is very important!). A request line has the form: 
[method] [uri] [version] 
The method is usually GET (but may be other things, such as POST, OPTIONS, or PUT). The URI is the file name and any optional arguments (for dynamic content). Finally, the version indicates the version of the HTTP protocol that the web client is using (e.g., HTTP/1.0 or HTTP/1.1).   For example, here is the request line for the "about" page on YU's main website:
GET /about HTTP/1.1
Following the request line are one or more request header lines, each consisting of name-value pair with the name separated from the value by a colon.  These lines tell the server more details about what methods of responding to the request are acceptable to the browser.  The only required one is the host, which tells
web site host the client is trying to talk to, as each server may serve several web sites.
Host: www.yu.edu
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.11; rv:72.0) Gecko/20100101 Firefox/72.0
Accept:text/html,application/xhtml+xml,application/xml;q=0.9,...
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate, br
DNT: 1
Connection: keep-alive
Upgrade-Insecure-Requests: 1
Pragma: no-cache
Cache-Control: no-cache
An HTTP response (from the server to the browser/client) is similar; it consists of a response line, zero or more response headers, an empty text line (also very important!), and finally the interesting part, the response body. A response line has the form 
[version] [status] [message] 
The status is a three-digit positive integer that indicates the state of the request; some common states are 200 for "OK", 403 for "Forbidden", and 404 for "Not found". Two important response header lines Content-Type, which tells the client the MIME type of the content in the response body (e.g., html or gif) and Content-Length, which indicates its size in bytes. 
If you would like to see the HTTP protocol in action, you can connect to any web server using telnet. For example, run 
telnet www.google.com 80 
and then type (note that there is an empty line at the end): 
GET / HTTP/1.1 
host: www.google.com 
You will then see the HTML text for that web page!   (Note: telnet will not work for modern secure web pages whose URLs start with https:// instead of http://)
Important: When running a web server and browser on the same virtual machine, the host name for your connection will be localhost.  A typical URL might be http://localhost/index.html
The main reason for telling you all this about HTTP is so you don't break the HTTP protocol when you modify the server to add more header lines to the response.  Knowing the order of things (including those blank lines, hint, hint…) will help keep you from breaking stuff.

## Basic Web Server

The code for the web server is available from Canvas. You should copy over all of the files there into your own working directory. You should compile the files by simply typing make. Your shell needs to be in the same directory as your files when you type this.  Compile, run, and test this basic web server before making any changes to it!  make clean removes .o files and lets you do a clean build. 
When you run this basic web server, you need to specify the port number that it will listen on; you should specify port numbers that are greater than about 2000 to avoid active ports. When you then connect your web browser to this server, make sure that you specify this same port. For example, assume that you are running on localhost and use port number 2003; copy your favorite html file to the directory that you start the web server from.   I provide 2 example content files, one HTML file and one image file.
To view this file from a web browser (running on the same or a different machine), use the url: 
localhost:2003/favorite.html 
To view this file using the client code we give you, use the command 
client localhost 2003 /favorite.html

## Kelly's Chesed to us

The web server that we are providing you is only about 200 lines of C code. To keep the code short and very understandable, we are providing you with the absolute minimum for a web server. For example, the web server does not handle any HTTP requests other than GET and understands only a few static content types. This web server is also not very robust; for example, if a web client closes its connection to the server (e.g., if the user presses the "stop") it may crash. We do not expect you to fix these problems! 
In the server code, all error codes returned by library functions are being checked, with orderly termination of the server if a problem is found.  One should always check error codes! However, many programmers don't like to do it because they believe that it makes their code less readable.  You may either code your error checking inline (as IBM does) or user a wrapper library (like csapp.c from COM2113).. Note the common convention of naming wrapper function the same as the underlying system call, except capitalizing the first letter, and keeping the arguments exactly the same.   In no case may you make a library call without some kind of check of the return code.

## Overview: New Functionality

In this project, you will be adding functionality to both the web server code and the web client code. 
You will be adding three key pieces of functionality to the basic web server.  First, the web server, as written, forks a separate process for each new connection.  You must make the web server multi-threaded using a pool of ready and waiting worker threads, with the appropriate synchronization. Second, you will implement different scheduling policies so that requests are serviced in different orders. Third, you will add statistics to measure how the web server is performing. You will also be modifying how the web server is invoked so that it can handle new input parameters (e.g., the number of threads to create). 
You will also be adding functionality to the web client for testing. You should think about how this new functionality will help you test that the web server is implemented correctly. You will modify the web client so that it is also multi-threaded and can initiate requests to the server in different, well-controlled groups. 

### Part 1: Multi-threaded Server

The basic web server that we start with is a single-threaded server that enters an infinite loop to handle sequential requests.  The very last paragraph contains that loop.   That loop is a prototype for how you want each service thread to behave.
The simplest approach to building a multi-threaded server is to spawn a new thread for every new http request, instead of a new process. The OS will then schedule these threads according to its own policy. When one thread is blocked (i.e., waiting for disk I/O to finish) the other threads can continue to handle other requests. However, the drawback of the one-thread-per-request approach is that the web server pays the overhead of creating a new thread on every request. 
Therefore, the generally preferred approach for a multi-threaded server is to create a fixed-size pool of worker threads when the web server is first started. With the pool-of-threads approach, each thread is blocked until there is an http request for it to handle. Therefore, if there are more worker threads than active requests, then some of the threads will be blocked, waiting for new http requests to arrive; if there are more requests than worker threads, then those requests will need to be buffered until there is a ready thread. 
In your implementation, you must have a master thread that begins by creating a pool of worker threads, the number of which is specified on the command line. Your master thread is then responsible for accepting new http connections over the network and placing a descriptor for this connection into a fixed-size buffer; in your basic implementation, the master thread should not read from the connection. The number of elements in the buffer is also specified on the command line. Note that the existing web server spawns a process as soon as a connection arrives.  Your design needs to have the main thread save the connection info in a buffer before handing it off to a worker thread. (It is up to you to determine what info should be saved, and in what format.) It then returns to its task of accepting new connections.   You should investigate how to create and manage posix threads with pthread_create and pthread_detach. 
Each worker thread is only able to handle requests for static web pages (files). A worker thread wakes when there is an http request in the queue; when there are multiple http requests available, which request is handled depends upon the scheduling policy, described below. Once the worker thread wakes, it performs the read on the network descriptor, obtains the specified content (by reading the specified static file), and then returns the content to the client by writing to the descriptor. The worker thread then waits for another http request. 
Note that the master thread and the worker threads are in a producer-consumer relationship and require that their accesses to the shared buffer be synchronized. Specifically, the master thread must block and wait if the buffer is full; a worker thread must wait if the buffer is empty. In this project, you are required to use condition variables. If your implementation performs any busy-waiting (or spin-waiting) instead, you will be heavily penalized. 

### Part 2: Scheduling Policies

In this project, you will implement a number of different scheduling policies. Note that when your web server has multiple worker threads running (the number of which is specified on the command line), you will not have any control over which thread is actually scheduled at any given time by the OS. Your role in scheduling is to determine which http request should be handled by each of the waiting worker threads in your web server. 
The scheduling policy is determined by a command line argument when the web server is started and are as follows: 
Any Concurrent Policy (ANY): When a worker thread wakes, it can handle any request in the buffer. The only requirement is that all threads are handling requests concurrently. (In other words, you can make ANY=FIFO if you have FIFO working.) 
First-in-First-out (FIFO): When a worker thread wakes, it handles the first request (i.e., the oldest request) in the buffer. Note that the http requests will not necessarily finish in FIFO order since multiple threads are running concurrently; the order in which the requests complete will depend upon how the OS schedules the active threads. 
Highest Priority to Image Content (HPIC): When a worker thread wakes, it handles the first request that is for an image file; these are files whose name ends in .jpg, .png, etc.  If there are no requests for image content, it handles the first request for HTML content. Note that this algorithm can lead to the starvation of requests for HTML content. 
Highest Priority to HTML Content (HPHC): This is the opposite of HPRC. Note that this algorithm can lead to the starvation of requests for non-HTML content. 
You will note that the HPIC and HPHC policies require that something be known about each request before the requests can be scheduled. This information cannot be obtained without reading at least one line from each incoming connection.  Thus, to support this scheduling policy, you will need to do some initial processing of the request outside of the worker threads; you will want the master thread to perform this work, which requires that it read from the network descriptor.  It may need to then pass the read information to the worker thread in some form.  This will also change the logic of the worker thread.

### Part 3: Usage Statistics

You will need to modify your web server to collect a variety of statistics. Some of the statistics will be gathered on a per-request basis and some on a per-thread basis. All statistics will be returned to the web client as HTTP response headers. Specifically, you will be embedding these statistics in the entity headers; we have already made a place-holder in the basic web server code for where these headers are written. You should add the additional statistics. Note that most web browsers will ignore these headers that it doesn't know about; to access these statistics, you will want to run our modified client.  As you add these headers, make sure to preserve the blank line following the headers and before the body of the server response.
For each request, you will record the following counts or times; all times should be recorded at the granularity of milliseconds. You may find gettimeofday() useful for gathering these statistics. 
X-stat-req-arrival-count: The number of requests that arrived before this request arrived. Note that this is a shared value across all of the threads. 
X-stat-req-arrival-time: The arrival time of this request, as first seen by the master thread. This time should be relative to the start time of the web server. 
X-stat-req-dispatch-count: The number of requests that were dispatched before this request was dispatched (i.e., when the request was picked by a worker thread). Note that this is a shared value across all of the threads. 
X-stat-req-dispatch-time: The time this request was dispatched (i.e., when the request was picked by a worker thread). This time should be relative to the start time of the web server. 
X-stat-req-complete-count: The number of requests that completed before this request completed; we define completed as the point after the file has been read and just before the worker thread starts writing the response on the socket.  Note that this is a shared value across all of the threads. 
X-stat-req-complete-time: The time at which the read of the file is complete and the worker thread begins writing the response on the socket. This time should be relative to the start time of the web server. 
X-stat-req-age: The number of requests that were given priority over this request (that is, the number of requests that arrived after this request arrived, but were dispatched before this request was dispatched). 
You should also keep the following statistics for each thread: 
X-stat-thread-id: The id of the responding thread (numbered 0 to number of threads-1) 
X-stat-thread-count: The total number of http requests this thread has handled 
X-stat-thread-html: The total number of HTML requests this thread has handled 
X-stat-thread-image: The total number of image requests this thread has handled 
Thus, for a request handled by thread number i, your web server will return the statistics for that request and the statistics for thread number i as HTTP headers.

### Part 4: Multi-threaded Client

We provide you with a basic single-threaded client that sends a single HTTP request to the server and prints out the results. This basic client prints out all response headers with the statistics that you added, so that you can verify the server is ordering requests as expected. While this basic client can help you with some testing, it doesn't stress the server enough with multiple simultaneous requests to ensure that the server is correctly scheduling or synchronizing threads. Therefore, you need to modify the web client to send more requests with multiple threads. Specifically, your new web client must implement two different request workloads (specified by a command line argument you will add). All versions take a new command line argument: N, for the number of created threads. 
Concurrent Groups (CONCUR): The client creates N threads and uses those threads to concurrently (i.e., simultaneously) perform N requests for the same file; this behavior repeats forever (until the client is killed). You should ensure that the N threads overlap sending and waiting for their requests with each other. After all of the N threads receive their responses, the threads should repeat the requests. You may find the routine pthread_barrier_wait useful for implementing this; in no case should busy-waiting be used. 
First-In-First-Out Groups (FIFO): The client creates N threads and uses those threads to perform N requests for the same file; however, the client ensures that the requests are initiated in a serial order, but that the responses can occur in any order. Specifically, after one thread sends its request, it should signal to another thread that it can now send its request, and so on for the N threads; the N threads then concurrently wait for the responses. After all of the N threads receive their responses, the threads should repeat the requests until the client is killed. You might find semaphores useful for implementing this behavior; in no case should busy-waiting be used.

### Part 5: Daemonize the Server

This has nothing whatsoever to do with multi-threading, but true web server should be running as a daemon process.  However, this is inconvenient for development, so we leave this detail until last.  A daemon process runs independent of the process that forked it and does not depend on that process to reap it if it dies.  Some Operating Systems provide utilities to convert ordinary processes into daemons, but there is a UNIX-wide standard procedure for any process that want to daemonize itself.  This applies to Linux, the BSDs, MacOS, etc.
See  https://stackoverflow.com/questions/17954432/creating-a-daemon-in-linux for a detailed description of what is entailed, including some sample code that you may want to adapt.  The server should only daemonize itself if it starts up with a –d option on the command line.  There is a Linux library function called daemon that does some of this, but it does not daemonize in a portable way across all the UNIX/Linux variants.
There is no reason at all to daemonize the client, as it is used only for testing the server.
Program Specifications
For this project, you will be implementing both the server and the client. The server code you start with in invoked as:
./server [portnum] [folder] &
Your web server must be invoked exactly as follows: 
./server [portnum] [folder] [threads] [buffers] [schedalg] &
The command line arguments to your web server are to be interpreted as follows. 
portnum: the port number that the web server should listen on; the basic web server already handles this argument. 
folder: the folder in which your files are hosted, may often be . (dot).
threads: the number of worker threads that should be created within the web server. Must be a positive integer. 
buffers: the number of request connections that can be accepted at one time. Must be a positive integer. Note that it is not an error for more or less threads to be created than buffers. 
schedalg: the scheduling algorithm to be performed. Must be one of ANY, FIFO, HPIC, or HPHC. 
For example, if you run your program as 
./server 5003 8 16 FIFO &
then your web server will listen to port 5003, create 8 worker threads for handling http requests, allocate 16 buffers for connections that are currently in progress (or waiting), and use FIFO scheduling for arriving requests. 
To stop your server from running, since it is a daemon, find its PID in the process table (using ps aux | grep server is one way) and then kill it.  Remember to do this before you start another version.
Your starting code for the web client is always invoked as
./client [host] [portnum] [filename]

## Example

For example:
./client localhost 8082 /index.html
Your finished web client must be invoked exactly as follows: 
client [host] [portnum] [threads] [schedalg] [filename1] [filename2] 
The command line arguments to your web server are to be interpreted as follows. 
host: the name of the host that the web server is running on; the basic web client already handles this argument. 
portnum: the port number that the web server is listening on and that the client should send to; the basic web client already handles this argument. 
threads: the number of threads that should be created within the web client. Must be a positive integer. 
schedalg: the scheduling algorithm to be performed. Must be one of CONCUR or FIFO. 
filename1: the name of the file that the client is requesting from the server. 
filename2: the name of a second file that the client is requesting from the server. This argument is optional. If it does not exist, then the client should repeatedly ask for only the first file. If it does exist, then each thread of the client should alternate which file it is requesting. 

## Hints

I recommend understanding how the code that we gave you works. All of the code is available as a zip file on Canvas. I provide the following files: 
server.c: Contains main() and support routines from IBM for the basic web server. 
client.c: Contains main() and the support routines for the very simple web client. 
Makefile: support file for rebuilding your system
index.html: a sample HTML file
dubat.jpg: a sample image file

## Make
You can type "make" to recompile the client and server executable. You can type "make clean" to remove the object files and the executables. You can type "make server" to create just the server program, etc. If you create new files, you will need to alter the Makefile.   Google for info on how to do this.
I recommend that you experiment with the existing code. The best way to learn about the code is to compile it and run it. Run the server we gave you with your preferred web browser. Run this server with the client code we gave you. You can even have the client code we gave you contact any other server (e.g., www.yu.edu). Make small changes to the server code (e.g., have it print out more debugging information) to see if you understand how it works. 

## Methods to Use

I anticipate that you will find the following routines useful for creating and synchronizing threads: pthread_create, pthread_detach, pthread_mutex_init, pthread_mutex_lock, pthread_mutex_unlock, pthread_cond_init, pthread_cond_wait, pthread_cond_signal. To find information on these library routines, being with the manual pages (using the Unix command man), and read the tutorials below. 
I strongly recommend that you use a good version control system for your code to prevent disastrous edits.  There are many good tutorials for systems such as git and mercurial.
You may find the following tutorials useful as well. 
Linux Tutorial for Posix threads 
POSIX threads programming 


## Grading

Hand in a zip file (or a gzipped tar) of your source code and a README file to the dropbox on Canvas.  Do not include any .o files, html files, or graphics files. Make sure that all your group’s members are listed in the README file.
In your README file you should have the following five sections: 
The name and login information for all project partners. A brief description of how you divided the work between you. 
Design overview: A few simple paragraphs describing the overall structure of your code and any important structures. 
Complete specification: Describe how you handled any ambiguities in the specification. For example, how do you implement the ANY policy? 
Known bugs or problems: A list of any features that you did not implement or that you know are not working correctly 
Testing: This requirement an aspect that I am very interested in. I will assign points for answering these questions. Describe how you tested the functionality of your web server. Describe how can you use the various versions of your extended client to see if the server is handing requests concurrently and implementing the FIFO, HPSC, or HPDC policies. 
Specifically, what are the exact parameters one should pass to the client and the server to demonstrate that the server is handling requests concurrently? To demonstrate that the server is correctly running the FIFO policy? the HPSC policy? the HPDC policy? In each case, if your client and server are behaving correctly, what output and statistics should you see? 
