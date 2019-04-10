//
// Created by israel on 3/28/19.
//

#include "OsSimulation.h"

//constructor
OsSimulation::OsSimulation() : pIdAvailable(rootPid) {
    int memory;
    int pages;
    int diskCount;
    std::cout << "Memory:(bytes)";
    std::cin >> memory;
    std::cout << "\npages:" << std::endl;
    std::cin >> pages;
    std::cout << "\nDisk:" << std::endl;
    std::cin >> diskCount;

    //create the root process
    Process rootProcess(pIdAvailable);
    pIdAvailable++;

    //add the root process to the list of process control blocks
    processes.insert({rootProcess.getId(), rootProcess});
    //cpu starts running process with pid of 1
    cpu.run(&processes.at(rootPid));

    disk.resize(diskCount);
}

void OsSimulation::promptForCommand() {
    std::string userInput;
    //flush cin buffer
    std::cin.ignore();

    while (userInput != "exit") {
        std::cout << ">> ";

        getline(std::cin, userInput);

        if (userInput == "A") {
            startNewProcess();
        } else if (userInput == "S r") {
            snapShotReadyQueue();
        } else if (userInput == "Q") {
            rotateProcess();
        } else if (userInput == "fork") {
            fork(cpu.getRunning());
        } else if (userInput == "wait") {
            waitForChildren();
        }else if (userInput == "info") {
            printProcessInfo();
        }else if (userInput == "exit") {
            //todo. running process terminates
        } else {
            std::cout << "invalid input" << std::endl;
        }
    }

}

void OsSimulation::rotateProcess() {
    //move id running process to back of readyQueue
    readyQueue.push(cpu.getRunning().getId());

    //get reference to the front of readyQueue
    Process &frontOfQ = processes.at(readyQueue.front());
    //make cpu run it (point to it)
    cpu.run(&frontOfQ);

    readyQueue.pop();
}

//starts a new process which is essentially a fork of the root
void OsSimulation::startNewProcess() {
    Process & root = processes.at(rootPid);

    fork(root);
}

void OsSimulation::snapShotReadyQueue() const {
    std::cout << "cpu: pid " << cpu.getRunning().getId() << std::endl;
    std::cout << "r-q: " << std::endl;

    std::queue<int> readyQCopy = readyQueue;

    while (!readyQCopy.empty()) {
        std::cout << "  pid " << readyQCopy.front() << std::endl;
        readyQCopy.pop();
    }

    std::cout << std::endl;
}

void OsSimulation::fork(Process &parentProcess) {
    Process * processPtr = new Process(pIdAvailable, parentProcess.getId());
    pIdAvailable++;

    parentProcess.addChild(processPtr);

    //insert it to the process container
    processes.insert({processPtr->getId(), *processPtr});

    //insert id to the ready queue
    readyQueue.push(processPtr->getId());
}

void OsSimulation::waitForChildren() {

}

void OsSimulation::printProcessInfo() {
    int pId = pIdAvailable + 1;
    while (pId >= pIdAvailable) {
        std::cout << "enter pid: ";
        std::cin >> pId;
    }

    Process & process = processes.at(pId);
    std::cout << "pid     : " << process.getId() << std::endl;
    std::cout << "parent  : " << process.getParent() << std::endl;
    std::cout << "children: " << std::endl;
    std::vector<Process *> children = process.getChildren();
    Process * currentChild = nullptr;
    for(int i = 0; i < children.size(); i++) {
        currentChild = children.at(i);
        std::cout << "--" << currentChild->getId() << " " << std::endl;
    }
}



