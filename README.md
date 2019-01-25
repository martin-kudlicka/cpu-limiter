# CPU Limiter

Limits CPU power to applications.

![](http://martink.cz/sites/default/files/inline-images/cpu-limiter-mainwindow.png)
![](http://martink.cz/sites/default/files/inline-images/cpu-limiter-rule.png)

## Description
User defined rules consisting of condition and action to limit CPU for specific processes. Condition consists of:
* AND list of processes (wildcard supported)
* status of listed processes (running or not running)
* state of listed processes in case of running status (any state, foreground, background)
* optional status of internet connection

Action consists of:
* list of processes to limit CPU (wildcard supported)
* percentage CPU limit (per target process)
* or suspend target processes
* delay in seconds to apply action

## Requirements
* Windows 8
* [Visual C++ Redistributable for Visual Studio 2015](https://www.microsoft.com/download/details.aspx?id=48145)

## Components
* [Qt](https://www.qt.io/)
