Name: Chase Alcorn
Date: 4/28/2023

NetID: chasten
Perm#: 8030603


Program Documentation:
This repository contains two types of files, a Client file and Server file.
There are 2 versions of each file, one for TCP, and one for UDP. Otherwise the functionality of each file is identical. I will not go into the details of TCP/UDP


The Client will send a string of digits to the Server, and the server will sum each individual digit into a new number. The server will then sum each individual digit of this new number as well, and repeat this process on each new calculated number until the number is reduced to a single digit.

The server does this by iterating through the string, and converting each char to an int by subtracting the char '0' from the ascii value of the digit. This returns the binary value of the digit.

The server sends each calculated new number to the client, where the client displays it.

In the case where the client does not send a string of digits to the server, the server replies: "Sorry, cannot compute!"
My server has special functionality where if it ever receives the string "exit", the server closes its socket and the program exits with status 0.

The server never outputs any text to the console.



NOTE: I do not understand why test_case_1_udp will not pass. I believe it is an error on the part of the autograder.