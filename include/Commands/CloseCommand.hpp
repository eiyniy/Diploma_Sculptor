#pragma once

#include <Command.hpp>

class MainWindow;

class CloseCommand : public Command
{
public:
    explicit CloseCommand(MainWindow &_mainWindow);

    ~CloseCommand() override = default;

    void execute() override;

private:
    MainWindow &mainWindow;
};
