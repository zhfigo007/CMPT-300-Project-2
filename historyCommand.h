#ifndef HISTORYCOMMAND_H
#define HISTORYCOMMAND_H

void executePrintHistoryCommand();
void addCommandToHistory(char *command);
void executeNumberedHistoryCommand(int commandNo);
void execHistCommandAtIndex(int commandNoIndex);

#endif
