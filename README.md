# Sleeping Teaching Assistant
This program simulates a teaching assistant (TA) who assists students needing help in a university setting. The TA sleeps when no students are around and is awakened by the arrival of students. The simulation uses multithreading and synchronization mechanisms to manage the interaction between the TA and the students.

# Features
* Multithreading : Concurrent execution of TA and student threads using the pthread library.
* Semaphores : Used for managing the availability of the TA and the chairs in the waiting room.
* Mutex Locks : Ensure safe access to shared resources, preventing race conditions.
* Realistic Simulation : Models the scenario of a TA helping students, where students wait if the TA is busy or return later if all chairs are occupied.

# Requirements
C Compiler (e.g., gcc)

POSIX Threads library (pthread)

POSIX Semaphores library (semaphore.h)


# How It Works
# TA Thread:

The TA sleeps when no students are present.
When a student arrives, the TA is awakened and helps the student.
The TA continues to help students as long as there are students in the waiting room.

# Student Threads:

Each student arrives and checks if there is an available chair in the waiting room.
If a chair is available, the student sits and waits for the TA.
If no chairs are available, the student leaves and returns later.
Synchronization :

* Semaphores : Used to manage the state of the TA (awake/asleep) and the availability of chairs.
* Mutex Locks : Used to ensure that chair counts are updated safely without race conditions.
  
# Code Overview
* TA_check : Function for the TA thread. The TA wakes up when a student arrives and helps students sequentially.
* Student_Check : Function for student threads. Each student tries to find a chair and get help from the TA.
* main : Initializes semaphores and mutexes, creates TA and student threads, and manages thread joining.
