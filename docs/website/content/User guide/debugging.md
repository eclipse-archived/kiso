---
title: "Debugging"
weight: 2
---

This guide shows how to debug a kiso application within WSL.

## Prerequisites:

### software:
 - Windows machine.
 - VS-Code.
 - Remote WSL extension for VS code.
 - cortex-Debug extension for VS code.


### Hardware:
 - Kiso supported Board.
 -  JlinkDebugger.

## Assumptions:
You are using WSL to build your kiso binaries.

## Problem:

The Windows subsystem for Linux as of version 1 does not provide support for USB devices **i.e** *directing the USB IO from the USB port to the WSL system does not work out of the box*. This applies particularly for USB serial devices **e.g.** *debugging devices*.  If you create your Kiso binary in WSL and want to debug in WSL using the Linux version of Jlink it will not work ( didn't work for me at least) because the jlink connected to the PC will be owned by windows OS and as said before : **redirection does not work**.
If you create your Kiso binary in WSL and want to debug it in Windows it will work but you will find yourself debugging in assembly because the debug information related to source paths will be in UNIX path format
which will not be recognized by the windows World.

## Solution:

There is a simple solution that works perfectly. All you need to do is to run the Windows variant Jlink GDB server from the WSL environment in that way that it is redirecting its standard input/ output / error to WSL but will still execute in windows context and accessing all windows resources. 
What you have to do is to add the windows Jlink to the **WSL PATH** :

>    **$ export PATH=$PATH:/mnt/c/"Program Files (x86)"/SEGGER/JLink**

and in your cortex-debug configuration provide the **JLinkGDBServerCL.exe** as "**serverpath**"
**Your paths have to stay in unix format and enjoy debugging on WSL.**

    {
    "version": "0.2.0",
    "configurations": [
    {
    "type": "cortex-debug",
    "request": "launch",
    "servertype": "jlink",
    "cwd": "${workspaceRoot}",
    "serverpath": "JLinkGDBServerCL.exe",
    "executable": "/mnt/c/your/app/path/application.out",
    "name": "Debug WSL",
    "device": "STM32L4R5ZI",
    "interface": "swd",
    "runToMain": true,
    "jlinkscript": "/mnt/c/if/needed/flash.jlink"
    }]
    }
