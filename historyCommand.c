#include <sys/wait.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "supportFunctions.h"
#include "historyCommand.h"

void executePrintHistoryCommand() {
	char intAsStr[50];

	for (int i = 0; i < history.currentSize; i++) {
		sprintf(intAsStr, "%d",
				history.totalCommandsExecuted - history.currentSize + i + 1);// + 1 to start count from 1 instead of 0
		strcat(intAsStr, "\t");
		write(STDOUT_FILENO, intAsStr, strlen(intAsStr));
		write(STDOUT_FILENO, history.historyArray[i],
				strlen(history.historyArray[i]));
		write(STDOUT_FILENO, "\n", strlen("\n"));
	}
}

void addCommandToHistory(char *command) {
	if (history.currentSize == HISTORY_DEPTH) {
		for (int i = 1; i < HISTORY_DEPTH; i++) {
			strcpy(history.historyArray[i-1], history.historyArray[i]);
		}
		strcpy(history.historyArray[HISTORY_DEPTH-1], command);
	} else {
		//historyArray array isn't full
		strcpy(history.historyArray[history.currentSize], command);
		history.currentSize++;
	}
	history.totalCommandsExecuted++;
}

void executeNumberedHistoryCommand(int commandNo) {
	// Check input for semantic errors
	if (commandNo < 1 || commandNo > history.totalCommandsExecuted) {
		write(STDOUT_FILENO, "Invalid command number\n",
				strlen("Invalid command number\n"));
		return;
	}

	if (history.currentSize < HISTORY_DEPTH) {
			execHistCommandAtIndex(commandNo-1);
	} else {
		//history full, check if specified command is within valid history range
		if (history.totalCommandsExecuted - commandNo >= HISTORY_DEPTH) {
			write(STDOUT_FILENO, "Invalid command number\n",
							strlen("Invalid command number\n"));
			return;
		} else {
			// Valid command. convert to index within history array
			int histArrayIndex = HISTORY_DEPTH -
					(history.totalCommandsExecuted - commandNo + 1);
			execHistCommandAtIndex(histArrayIndex);
		}
	}
}
void execHistCommandAtIndex(int commandNoIndex) {
	// success
	char commandBuff[COMMAND_LENGTH];
	strcpy(commandBuff, history.historyArray[commandNoIndex]);
	char *tokens[NUM_TOKENS];
	_Bool in_background = false;

	// display history command to be executed as per assignmnet instructions
	write(STDOUT_FILENO, commandBuff, strlen(commandBuff));
	write(STDOUT_FILENO, "\n", strlen("\n"));

	tokenizeAndProcessCommand(commandBuff, tokens, &in_background);
	executeCommand(in_background, tokens);
}

