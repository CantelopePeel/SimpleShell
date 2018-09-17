PROC & SHELL PROGRAM:
Author: Liam Heeger (lheeger)
Course: CSC 256

Build/Run Instructions:
  This project uses Bazel as the underlying build system for this project. There is a simple Makefile wrapping the Bazel usage. If the desired target machine does not have Bazel installed, this program will download and install it.

  Proc Program:
    Building & Running: Run the following command in this directory:
      make proc_build
      ./bin/proc_info
      ./bin/proc_info_rate 1 1

  Shell Program:
    Building & Running: Run the following command in this directory:
      make shell_build
      ./bin/shell-exec
    Testing: There are some automated tests and some manual tests (see below). The program will give a status of the tests shortly after executing them. To run automated tests run:
      make shell_test

Manual Testing for Shell: 
  There are some manual tests that must be performed with the shell to ensure it is working as expected. Please execute the following commands. The tests here succeed if no errors (e.g. "Error: ...") are printed to stderr and that the last run of the 'jobs' command results in no output. Where there is a [int] below, enter Ctrl-C to trigger an interrupt. Where there is a [tstp] below, enter Ctrl-Z to trigger the stop signal. First follow the build and run instructions for shell above and enter these commands:

sleep 10
[int]

sleep 10
[tstp]
bg 1

sleep 20
[tstp]
bg 2
fg 2

sleep 10 &
fg 3

[int]
[tstp]

jobs
exit
  
	
    