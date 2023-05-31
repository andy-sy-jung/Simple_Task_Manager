# Simple_Task_Manager
A simple local task manager, where you can upload and delete tasks. 

# Overview
An explanation of the TaskManager application, including its capabilities and how it functions.
## Purpose
I working on improving my knowledge base and thought that creating a simple application with a GUI would be a good first step. I also noticed that the Nuklear documentation is kinda poor in showing how to use the Nuklear API (I mean, there are a bunch of great examples provided in the demo folder, but those were all so complicated and not very well documented in how the code works). The examples using GLFW and OpenGL were not documented at all so I hope that this project can provide a simple example on how to use Nuklear with GLFW. (I'll provide further documentation in a different file).
## Code
This task manager is written in C, using [Nuklear](https://github.com/Immediate-Mode-UI/Nuklear), [GLFW](https://www.glfw.org/), and [OpenGL](https://www.opengl.org/) to accomplish the GUI for the application. The TaskManager currently uses v.2.1 of OpenGL, as it was the latest version that my local machine supports (Apple had stopped OpenGL support way back). I used Nuklear because it was relatively easy to use, only needing a single header file to implement. I used GLFW as the windows and input framework because honestly, it was the first thing I looked at when going through the demo folder of the Nuklear github. Since TaskManager is mainly a text-based application, OpenGL was really underutilized in this project. I may update this project later to include some more things in the application that utilize the OpenGL API.
## Data Structure
I managed the CRUD of users and tasks through a file-based architecture. Since this was a simple project, I felt that there was no need to use an actual database to store these tasks, and opted to store data locally. I believe the code would be easily amended to use a database instead.

**File Structure:**
```
├── .users
│   ├── Andrew
│   │   └── tasks
│   └── Lexis
│       └── tasks
├── extra_font
└── icon
```
## Users and Tasks
There should exist a hidden folder .users where all the users and passwords are stored. Each user would be a directory in that hidden file, with each user directory containing one .txt file named with their password and another directory of tasks. The createUser() would create a directory named after the input username, and in that directory <password>.txt and a tasks directory. Each time a task is entered, writeToData() would create a .txt file in the tasks folder of the user logged in, each labled numerically (kept track by numOfTasks that is initialized upon login). A deleted task using deleteData() will delete that specific .txt file, and renumber all of the files (as to minimize a file read error in the main_window). Data is read and presented in the main_window by readFromData() by using a for loop.
## Planned/Future Features
I am not really sure of what to add to this simple application, but I am thinking of making it look more appealing that's for sure. I would like to work out the kinks in the application (described below), but it seems I am limited by the Nuklear GUI capabilities (or more likely that I have no idea how to fix them).
## Issues
One of the more annoying issues I have with the application is with the inability of the entry text box to wrap user input (i.e. wrap text to the next line if it reaches the end width of the box). It keeps scrolling to the right as more and more characters are input. If anyone knows how to fix it, it would be great. Also, I am sure I'm really inefficiently managing memory, so any helpful suggestions for improvement would be appreciated.
## Licence
I really don't know the difference between the licenses so I just chose randomly. This really isn't production worthy code or anything so feel free to use.
