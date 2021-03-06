# Programming Assignment 2: Web Servers and Synchronization

## Contact Info
Benjamin Mankowitz		bmankowi@mail.yu.edu
Ari Roffe				aroffe@mail.yu.edu
## A brief description of how you divided the work between you. 
For the majority of the code, we used the Live Share feature of Visual Studio Code and a phone call to code together and plan our design. For the times that we worked asyncronously, we used a shared git repository to work together remotely, merging changes when needed. Our git server is https://github.com/bmankowitz/Web-Servers-and-Sync and our commit history is at the end of this file.

## Design overview and specification: 
- A basic, single-threaded server was provided. We initially implemented the a [scheduling algorithm](https://www.tutorialspoint.com/operating_system/os_process_scheduling_algorithms.htm) using two buffers, one high priority (HP) and one low priority (LP). For HPHC and HPIC, we pushed the high priority data to the HP buffer, and other data to the LP buffer. For the FIFO and ANY schedulinh algorithm, we only used one buffer (ANY defaulted to FIFO). To ensure the buffer size is never exceeded, we checked that buffer1.size + buffer2.size never exceeds the maximum amount of threads provided. When preprocessing the request to determine what type it is, we used the `recv` command with the `MSG_PEEK` flag so that the data is still there for the `web()` function.

- For `stats`, we added all of the job specific fields in the `job_t` struct. For the thread specific stats, we created a new `t_stats` struct assigned to each thread. We used `gettimeofday()` in conjunction with a local function to get [epoch time](https://en.wikipedia.org/wiki/Epoch_(computing)) in milliseconds. To properly return all of the custom headers, we created a function `getStatHeaders()` to provide the header string.

- For [daemonization](https://en.wikipedia.org/wiki/Daemon_(computing)), we used the stackoverflow link in the project instructions to implement daemonization.

## Issues:

### Guiding Questions:
Describe how you tested the functionality of your web server. Describe how can you use the various versions of your extended client to see if the server is handing requests concurrently and implementing the FIFO, HPSC, or HPDC policies. Specifically, what are the exact parameters one should pass to the client and the server to demonstrate that the server is handling requests concurrently? To demonstrate that the server is correctly running the FIFO policy? the HPSC policy? the HPDC policy? In each case, if your client and server are behaving correctly, what output and statistics should you see? 

### Git Commit Log
```
commit 6f610a8823c15cb2ef537a3a43e12eecb50e0f5e
Author: bmankowitz <bmankowi@mail.yu.edu>
Date:   Wed Mar 18 23:43:00 2020 -0400

    tentative daemon code

commit 73f95b194c12e389bde11d75077d4299c1ae3732
Merge: c2ee886 8ea533d
Author: Ari <aroffe@mail.yu.edu>
Date:   Wed Mar 18 21:47:15 2020 -0500

    Merge branch 'master' of https://github.com/bmankowitz/Web-Servers-and-Sync

commit c2ee886dc59488d1cba9e223d09eea654ff22b85
Author: Ari <aroffe@mail.yu.edu>
Date:   Wed Mar 18 21:44:33 2020 -0500

    newest

commit 8ea533d5d2c431e3839cb91f0882109257547831
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Wed Mar 18 17:08:04 2020 -0500

    Add files via upload
    
    This update make the file appear in the buf in server.c

commit f1b35fdeec7253eeb3b8424b3a1e6fc199c06a57
Author: bmankowitz <bmankowi@mail.yu.edu>
Date:   Wed Mar 18 10:09:40 2020 -0400

    removed double work adding

commit 2264ebaeade1b9b3ed3a51a30a115b6ad795ad03
Merge: 1448292 d87c442
Author: Ari <aroffe@mail.yu.edu>
Date:   Tue Mar 17 21:59:06 2020 -0500

    Merge branch 'master' of https://github.com/bmankowitz/Web-Servers-and-Sync

commit d87c4425d7d952a896058ff5e26840d23c8c7e69
Author: bmankowitz <bmankowi@mail.yu.edu>
Date:   Tue Mar 17 22:56:51 2020 -0400

    zubats are tasty

commit 87e931a9ad3c2f8d639bf22f6eb2eab9cbca3a9a
Author: bmankowitz <bmankowi@mail.yu.edu>
Date:   Tue Mar 17 21:59:07 2020 -0400

    fixed nweb

commit 71e1e111fb44bc98264a49f81f1b89c5c239ee23
Author: bmankowitz <bmankowi@mail.yu.edu>
Date:   Tue Mar 17 14:41:32 2020 -0400

    Co-authored-by: Ari Roffe <aroffe@mail.yu.edu>

commit 671566aeeeee62bcab81f2cc47bcab34df4dafcc
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Tue Mar 17 13:00:19 2020 -0500

    Add files via upload
    
    fixed seg fault

commit 1a2772f8be37ead2fabd55baadf62f5ed15777a5
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Tue Mar 17 12:30:54 2020 -0500

    Add files via upload
    
    added the correct args for client.c

commit b0eee96bcdf156ceb62a71cf3f100e57fcaf295f
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Tue Mar 17 12:23:14 2020 -0500

    Add files via upload

commit 1448292fabfcc7a37b11db7a4a10c3ef38a54060
Author: Ari <aroffe@mail.yu.edu>
Date:   Mon Mar 16 22:33:40 2020 -0500

    just cause

commit 3332a9950f06e2eb0e592fee5e0d6690b62084eb
Author: bmankowitz <bmankowi@mail.yu.edu>
Date:   Mon Mar 16 23:30:10 2020 -0400

    Co-authored-by: Ari Roffe <aroffe@mail.yu.edu>

commit 8f6557716b52b5d880f5df798117a7bfdd3800af
Author: Ari <aroffe@mail.yu.edu>
Date:   Mon Mar 16 16:58:54 2020 -0500

    g

commit ce5fad9cbd4141725c2f265f3c5d281400ba6555
Author: bmankowitz <bmankowi@mail.yu.edu>
Date:   Mon Mar 16 14:47:24 2020 -0400

    Co-authored-by: Ari Roffe <aroffe@mail.yu.edu>

commit ca14567981f9b6b3aec8e51b5e98903cf9e5d6f4
Author: bmankowitz <bmankowi@mail.yu.edu>
Date:   Mon Mar 16 13:56:28 2020 -0400

    server.c

commit ee4e403523960e2e0cf82b1a27ebd81681a9d381
Author: bmankowitz <bmankowi@mail.yu.edu>
Date:   Mon Mar 16 13:55:01 2020 -0400

    Co-authored-by: Ari Roffe <aroffe@mail.yu.edu>

commit a154eeb8faf96e3c9ebca19f04387e30729f1a95
Author: Ari <aroffe@mail.yu.edu>
Date:   Mon Mar 16 12:51:22 2020 -0500

    l

commit 28fac15d2141d19a16dfaf38dae46363c0cf100d
Author: Ari <aroffe@mail.yu.edu>
Date:   Mon Mar 16 12:50:50 2020 -0500

    l

commit 36f25ed4c89f603c3394ead90c4b96d8a96f0de3
Author: bmankowitz <bmankowi@mail.yu.edu>
Date:   Mon Mar 16 13:46:55 2020 -0400

    edit readme

commit aa29147bbebacddd90634cd07f9a35d7b2807abb
Author: Ari <aroffe@mail.yu.edu>
Date:   Mon Mar 16 12:46:15 2020 -0500

    s

commit 56ccb6d7df096005e4bd44d53aaf95e8182c16dc
Author: Ari <aroffe@mail.yu.edu>
Date:   Mon Mar 16 12:43:02 2020 -0500

    misj

commit 4942809e4b769b318e1d490a0ed6f7697115faa3
Author: Ari <aroffe@mail.yu.edu>
Date:   Mon Mar 16 12:30:34 2020 -0500

    client changes

commit b2f5fe3a94129ef9b41b10e5e1c262323520f5bf
Author: Ari <aroffe@mail.yu.edu>
Date:   Mon Mar 16 12:06:06 2020 -0500

    l

commit 8ff6e689bc3a28227346c2c1ba079a2973e46080
Author: Ari <aroffe@mail.yu.edu>
Date:   Mon Mar 16 12:02:48 2020 -0500

    c

commit 9bc5086f9607e31ec75204270f6dd99b16b5a57b
Author: Ari <aroffe@mail.yu.edu>
Date:   Mon Mar 16 12:01:50 2020 -0500

    chnages erver

commit 3ede44ef410df11b3209138b90608848760bf78c
Author: bmankowitz <bmankowi@mail.yu.edu>
Date:   Mon Mar 16 00:14:41 2020 -0400

    bug fixes (i think)
    
    Co-authored-by: Ari Roffe <aroffe@mail.yu.edu>

commit 3e2c4673e3905ccee34828c76157679172801cc8
Author: bmankowitz <bmankowi@mail.yu.edu>
Date:   Sun Mar 15 19:03:53 2020 -0400

    Revert "Merge branch 'master' of https://github.com/bmankowitz/Web-Servers-and-Sync"
    
    This reverts commit 271c56b43213fb74e1f7a0b071d26a8826c2255d, reversing
    changes made to 2a1c3353b8e7a7c4ccfddd65c67cc66fdb20d4b8.

commit aa9927bd4b05caf159055a3e894f07782b6ee34a
Author: bmankowitz <bmankowi@mail.yu.edu>
Date:   Sun Mar 15 19:03:47 2020 -0400

    Revert "t"
    
    This reverts commit 2a1c3353b8e7a7c4ccfddd65c67cc66fdb20d4b8.

commit 271c56b43213fb74e1f7a0b071d26a8826c2255d
Merge: 2a1c335 695965e
Author: bmankowitz <bmankowi@mail.yu.edu>
Date:   Sun Mar 15 19:02:24 2020 -0400

    Merge branch 'master' of https://github.com/bmankowitz/Web-Servers-and-Sync

commit 2a1c3353b8e7a7c4ccfddd65c67cc66fdb20d4b8
Author: bmankowitz <bmankowi@mail.yu.edu>
Date:   Sun Mar 15 19:01:48 2020 -0400

    t

commit 695965e966a7431a8a58fb716df9992476e5f2c5
Author: shuyttr <aroffe@mail.yu.edu>
Date:   Sun Mar 15 16:12:44 2020 -0500

    coronavirus was caused by eating a zubat.png file mwahaahaha

commit 2e6208f7c7011b3a6939534873017b2a74dcd607
Author: bmankowitz <bmankowi@mail.yu.edu>
Date:   Sun Mar 15 16:18:55 2020 -0400

    Co-authored-by: Ari Roffe <aroffe@mail.yu.edu>

commit 89210827f7b80ba93a2fdf374cef92004b31b6db
Author: bmankowitz <bmankowi@mail.yu.edu>
Date:   Sun Mar 15 01:31:36 2020 -0400

    Big Update: finished all of part 3 including updating timing system, finishing getStatHeader, and implementing full header return. Now all we need to do is test

commit 6aacd9bbc5d43b7fc14f17df8cfb34b5261c6e18
Author: bmankowitz <bmankowi@mail.yu.edu>
Date:   Fri Mar 13 18:14:12 2020 -0400

    Co-authored-by: Ari Roffe <aroffe@mail.yu.edu>

commit 1dd486b2494af8a575b5d374500f753c0b3c042d
Author: bmankowitz <bmankowi@mail.yu.edu>
Date:   Fri Mar 13 17:00:27 2020 -0400

    update all

commit 749d7cfd12e98c2710d87d9512956f00cbf8de8c
Author: bmankowitz <bmankowi@mail.yu.edu>
Date:   Fri Mar 13 01:07:25 2020 -0400

    added debug (may not work on your system). added some fixme comments

commit bcd45c51b7f118824fe6a9a0b92be92946f5dc66
Author: bmankowitz <54521042+bmankowitz@users.noreply.github.com>
Date:   Thu Mar 12 22:50:46 2020 -0400

    Co-authored-by: Ari Roffe <aroffe@mail.yu.edu>

commit 4f2a87ba8e35434f505097b298eb2c8e74dc16d4
Author: bmankowitz <54521042+bmankowitz@users.noreply.github.com>
Date:   Thu Mar 12 21:46:36 2020 -0400

    Co-authored-by: Ari Roffe <aroffe@mail.yu.edu>

commit b7fce588c3eb47d9eb5ece4ee60f0d4e2a5abd79
Author: bmankowitz <54521042+bmankowitz@users.noreply.github.com>
Date:   Thu Mar 12 15:24:19 2020 -0400

    Co-authored-by: Ari Roffe <aroffe@mail.yu.edu>

commit ca939705587937a126690c146ba21b82329efb9b
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Wed Mar 11 22:07:04 2020 -0500

    Co-authored-by: bmankowitz <bmankowi@mail.yu.edu>

commit 2b7205161148ebb77ef28e432ddbf210592ad3c1
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Wed Mar 11 17:09:51 2020 -0500

    com

commit c4b6d936939a257751d2460f437f2481ec540964
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Wed Mar 11 16:40:33 2020 -0500

    Co-authored-by: bmankowitz <bmankowi@mail.yu.edu>

commit fb85076572940fb12a97121040d5ba26a3b1748f
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Mon Mar 9 13:52:06 2020 -0500

    not sure where the work is being added to the_pool

commit c52b5ea8d8551603e322718359146803649a7148
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Sun Mar 8 10:44:08 2020 -0400

    getJob should wrap to the front

commit eb0ae80d5350d212b2e26485760c816ebec3752a
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Sun Mar 8 10:39:21 2020 -0400

    added comments

commit e1a7332f9b259fedc23d65fa6161016363e87ad4
Author: bmankowitz <54521042+bmankowitz@users.noreply.github.com>
Date:   Thu Mar 5 22:51:33 2020 -0500

    add include for stdint so we have zero compiler errors

commit 0b71e4feeebb6e346483f2f7999c111824a70384
Author: bmankowitz <54521042+bmankowitz@users.noreply.github.com>
Date:   Thu Mar 5 22:46:51 2020 -0500

    added logic to tpool_add_work and to getJob

commit 6694c0cba424b7402cf259fb9d25ddb6f48def0b
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Thu Mar 5 19:18:00 2020 -0500

    malloc for tpool_worker

commit 4aa97b7fe9f24bd34f8b69feb14de06fde91764c
Author: bmankowitz <54521042+bmankowitz@users.noreply.github.com>
Date:   Thu Mar 5 17:58:48 2020 -0500

    fixed bad comment, added logic for setting schedalg (but not implementation yet)

commit 03784c559d2fe96f6b6e281fdeadb35258c6740c
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Thu Mar 5 17:02:27 2020 -0500

    more info for main

commit c0bd655b13338236bb2386073aff460287ea0499
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Thu Mar 5 16:35:53 2020 -0500

    the only errors are the unused variables...should probably fix those later plus test our code

commit d4c47501d2ea0db6d786fddcaf0520d98b8aca6c
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Thu Mar 5 16:14:15 2020 -0500

    indent plus something else...i forgot if it was importnant or not

commit 9d03e229b6927e01bc0f20d6dd9d43ac58f8ae64
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Thu Mar 5 16:08:24 2020 -0500

    fixed the squigels in the imports

commit e131aac757bc542c5520dbbfc35a0fd2e14f0d03
Author: bmankowitz <54521042+bmankowitz@users.noreply.github.com>
Date:   Thu Mar 5 15:53:19 2020 -0500

    added vscode settings. included stdbool. we need to talk about inlining getJob

commit bb9468720c702239f0ccf3680caf6f764a92ce70
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Thu Mar 5 15:13:46 2020 -0500

    nice code

commit d78c121d0341dab984c008136a677d80023b2d16
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Thu Mar 5 15:01:32 2020 -0500

    ben busting my chops

commit d222846485da243fc084d3454c5e6cbd8dd6e965
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Thu Mar 5 14:36:56 2020 -0500

    ben likes liveshare more than natty light

commit 1dc41d69eaefbd48e1ef8849880e3e4c9077ab31
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Thu Mar 5 13:59:22 2020 -0500

    Co-authored-by: bmankowitz <bmankowi@mail.yu.edu>

commit 5bc11f56843eee5d28c004634851424558734851
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Thu Mar 5 13:45:24 2020 -0500

    indentations etc

commit c1efae887a8067ddf0521548213feb025cb0bafd
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Thu Mar 5 12:01:48 2020 -0500

    corona doesn't our web server from operating

commit 4366b2b496b9e7596a70206517508d22fb505df5
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Tue Mar 3 13:45:19 2020 -0500

    added a variable to track the actual amount of elems in the buffer

commit b7aa0f77ba5565b32fee778f12b5657bb7c5ad92
Merge: a80003d 5eb1705
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Tue Mar 3 13:32:53 2020 -0500

    Merge branch 'master' of https://github.com/bmankowitz/Web-Servers-and-Sync

commit a80003d742903c521ac4448a91478904a3f10a42
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Tue Mar 3 13:27:07 2020 -0500

    merging

commit 5eb17056ad26880d0be70211e4cc5562e33b2319
Author: bmankowitz <54521042+bmankowitz@users.noreply.github.com>
Date:   Mon Mar 2 21:45:33 2020 -0500

    added getJob, and imporoved tpool_add_work. added some comments

commit d37afb9a99e55b68d744d870d6e71b9788860322
Merge: 4bb2a49 9470a12
Author: bmankowitz <54521042+bmankowitz@users.noreply.github.com>
Date:   Mon Mar 2 21:33:40 2020 -0500

    Merge branch 'master' of https://github.com/bmankowitz/Web-Servers-and-Sync

commit 9470a12a6c73b260b24dfacf8da0302789c4c790
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Mon Mar 2 21:14:55 2020 -0500

    ada spark

commit a6756d728ac508b67c26df8a14f7e109ded57f1b
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Mon Mar 2 20:16:19 2020 -0500

    comments

commit 6434390973e350fd6feeed8bc4f6b44fc14e4d57
Merge: b8590ed 76cbe68
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Mon Mar 2 15:55:57 2020 -0500

    Merge branch 'master' of https://github.com/bmankowitz/Web-Servers-and-Sync

commit b8590edf3f438541576ac9158bcc025f5ade56e3
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Mon Mar 2 15:55:39 2020 -0500

    some more notes, and implementing web, see how the web function is used in last line of main

commit 4bb2a496b468d5963b221a32af5d8f06d7740c84
Merge: 22146db 76cbe68
Author: bmankowitz <54521042+bmankowitz@users.noreply.github.com>
Date:   Sun Mar 1 22:52:37 2020 -0500

    Merge branch 'master' of https://github.com/bmankowitz/Web-Servers-and-Sync

commit 76cbe68d62bae48f9daec658d65c669b692e0d0c
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Sun Mar 1 21:53:23 2020 -0500

    good README shtick

commit e9f2caf557b4dadae66a0128dbe7c7bb86ca7024
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Sun Mar 1 21:46:59 2020 -0500

    not sure what the arguments are for the web method we call in the tpool_worker method, wtf is a socket file descriptor?

commit 41ba5cba180dcfd18bc6509a4a9f11bb46fcfce2
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Sun Mar 1 21:25:14 2020 -0500

    natty light

commit 522173e0886187d7fcd1fea58d339e3a3b5db376
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Sun Mar 1 21:22:05 2020 -0500

    annotated the producer, avoiding the Keystone cops nonsense

commit 26ab2984cb82802b12c76880527240c33da1e8c6
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Sun Mar 1 21:14:45 2020 -0500

    more notes on the consumer and chapping why we are locking to unlock at the end of the method

commit 6bbec1bdc28280a0a66ae9c318f816c34101c119
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Sun Mar 1 21:00:40 2020 -0500

    changed line 76/95 to reflect the dynamic allocation of the jobBuffer and now we access it by pashut index

commit b56921676db20e2069c9b47f68a9b0db7ff63720
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Sun Mar 1 20:59:28 2020 -0500

    allocated memory for the jobBuffer and now we can access it

commit e83622c279d39f594e815d8829e827bd0567533d
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Sun Mar 1 18:50:35 2020 -0500

    tpool_add_work was modified, CHECK OUT THE LOOP on lines 87-89 see if its ou certified

commit fd28b2c58f22bae8eab26de087ec5606a13e75ec
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Sun Mar 1 18:42:26 2020 -0500

    worked on the tpool_worker, still needs a fix on line 75

commit f5fde685e42327786841e74814248aed7bc8444f
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Fri Feb 28 15:48:03 2020 -0500

    spaces

commit 22146db15f31b5b9a860f6e10c930e88ba2d4e62
Author: bmankowitz <54521042+bmankowitz@users.noreply.github.com>
Date:   Thu Feb 27 16:17:55 2020 -0500

    added missing semicolon

commit 12920304b113abe28a0cf5e0a3afd2d6cb6f02f0
Author: bmankowitz <54521042+bmankowitz@users.noreply.github.com>
Date:   Wed Feb 26 22:18:02 2020 -0500

    added policy skeleton

commit 445f27ebc9ae676d82066d34925b49be835e6070
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Wed Feb 26 21:47:48 2020 -0500

    my indentation rox

commit 5f6759a691eb2459a7ac02a4f5f66c6d4579b5ce
Merge: fe0f17f 0e16e57
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Wed Feb 26 21:17:19 2020 -0500

    Merge branch 'master' of https://github.com/bmankowitz/Web-Servers-and-Sync

commit fe0f17fe7e75fc336e9dc65e9b4958ee04c3e9c1
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Wed Feb 26 21:15:18 2020 -0500

    lets do a merge shall we

commit 0e16e570f98638f7df8c75fdb4df9bf93e6cfe77
Author: bmankowitz <54521042+bmankowitz@users.noreply.github.com>
Date:   Wed Feb 26 21:14:32 2020 -0500

    piush1

commit 8a370476a4626ae2a76fdcabbae61fc417aee3ff
Merge: 41ef261 75ccf8d
Author: bmankowitz <54521042+bmankowitz@users.noreply.github.com>
Date:   Wed Feb 26 21:07:15 2020 -0500

    Merge branch 'master' of https://github.com/bmankowitz/Web-Servers-and-Sync

commit 41ef261131649cb4490f6d60a3d80a73a334eab0
Author: bmankowitz <54521042+bmankowitz@users.noreply.github.com>
Date:   Wed Feb 26 21:06:25 2020 -0500

    testarino

commit 75ccf8df2b2da825e61c6be1833d7bbf7151cedd
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Wed Feb 26 21:06:16 2020 -0500

    hey ben

commit 0b50baa0b6b279cd0dd3a7e4c0e2769eb7900b67
Merge: 517928e 2d570ea
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Wed Feb 26 21:04:45 2020 -0500

    Merge branch 'master' of https://github.com/bmankowitz/Web-Servers-and-Sync

commit 517928e52681e8f9cde4c04fdd33bb1c19f8e9a9
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Wed Feb 26 20:53:45 2020 -0500

    hey mergesort

commit 2d570ea297fd8a19b6c65195b97971e6ea99a046
Author: bmankowitz <54521042+bmankowitz@users.noreply.github.com>
Date:   Wed Feb 26 20:50:17 2020 -0500

    removed extra stuff

commit bb8e76cca6f03cb9acd988816dc4565b804029a3
Author: bmankowitz <54521042+bmankowitz@users.noreply.github.com>
Date:   Wed Feb 26 20:46:21 2020 -0500

    test for ari

commit 4ce02f06b99237776c5637e66b3b63f1418060e0
Author: Ari Roffe <aroffe@mail.yu.edu>
Date:   Wed Feb 26 20:44:44 2020 -0500

    see this ben

commit db6a08c4f1cdb6ca6e3ecd705d3fdb025f60d6b1
Author: bmankowitz <54521042+bmankowitz@users.noreply.github.com>
Date:   Wed Feb 26 20:34:43 2020 -0500

    Update README.md

commit 0d6594d4bed0c476e5af650fc326c1c23f1750a8
Author: bmankowitz <54521042+bmankowitz@users.noreply.github.com>
Date:   Wed Feb 26 18:53:18 2020 -0500

    Update .gitattributes

commit 4added264e20515b7af275b4ccd1a102488ac89c
Author: bmankowitz <54521042+bmankowitz@users.noreply.github.com>
Date:   Wed Feb 26 17:45:59 2020 -0500

    Update .gitattributes

commit 49fc2330295dfbee7b0ec643a3a0ebadd14ef5b4
Author: bmankowitz <54521042+bmankowitz@users.noreply.github.com>
Date:   Wed Feb 26 17:13:15 2020 -0500

    init

commit 6bc342d42ff91136f4f89db9f9d4a13918bda911
Author: bmankowitz <54521042+bmankowitz@users.noreply.github.com>
Date:   Wed Feb 26 17:11:23 2020 -0500

    Initial commit

```
