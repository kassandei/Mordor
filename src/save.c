#include <time.h>
#include "types.h"

char* currentDateTime(char date[DATESIZE]) {
    time_t now = time(NULL);
    struct tm *localTime = localtime(&now);
    strftime(date, DATESIZE, "%Y-%m-%d %H:%M:%S", localTime);

    return date;
}

void addSave(GameSaves* saves) {

}

void loadSave() {

}

void showSaves() {

}

void removeSaves() {
    
}