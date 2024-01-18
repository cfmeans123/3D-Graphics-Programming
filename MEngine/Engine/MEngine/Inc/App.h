#pragma once;

using namespace MEngine;
class App final
{
public:
    void Run(float runTime);
    void Quit();

private:
    bool mRunning = false;
};
