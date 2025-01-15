# Final Project Proposal

## Group Members:

Otto Buff
       
# Intentions:

This project will allow clients to access files from a server – a collaborative text editor. The clients will be able to read and modify the files in the server, and the files will be stored in a linked list. Files will be stored in a linked list, and the client will be able to select a file to create, open, close, modify, read, or clear. The contents of the file will be printed to the client, who will be able to modify it by adding a line or clearing the file. The client sends back the modified file to the server.
    
# Intended usage:

Users will be able to view a list of files and select one to view or modify. The contents of the file they select will be printed to the user, and they will be able to modify the file by entering a line number and altering it.

  
# Technical Details:

I will use files, memory allocation, semaphores, pipes, and forks. As I am working alone, I will be responsible for all parts. The files will be stored in a linked list (memory allocation), pipes will allow information to be passed back and forth between the client and server, the sempahores will synchronize the process between multiple users, and the forks will create a process for each user. 

# Intended pacing:

I will create the linked list portion first, followed by pipes, followed by semaphores and forks. I hope to finish before the last day of in person school, January 17.
