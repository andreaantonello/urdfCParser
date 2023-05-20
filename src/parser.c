//
// Created by Andrea Antonello on 28/04/2023.
//

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void parseURDF(const char* filename, URDFModel* model) {
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        printf("Failed to open file: %s\n", filename);
        return;
    }

    printf("File found and opened successfully: %s\n", filename);

    char line[256];
    URDFLink currentLink;
    URDFJoint currentJoint;

    while (fgets(line, sizeof(line), file)) {
        char* token = strtok(line, " ");
        if (token != NULL) {
            if (strcmp(token, "<link") == 0) {
                if (model->numLinks > 0) {
                    model->links = realloc(model->links, (model->numLinks + 1) * sizeof(URDFLink));
                } else {
                    model->links = malloc(sizeof(URDFLink));
                }

                strcpy(currentLink.name, strtok(NULL, "\""));
                strcpy(currentLink.parent, strtok(NULL, "\""));

                printf("Current link: %d\n", model->numLinks);
                model->links[model->numLinks] = currentLink;
                model->numLinks++;
            } else if (strcmp(token, "<joint") == 0) {
                strcpy(currentJoint.name, strtok(NULL, "\""));
                strtok(NULL, "\"");

                currentLink.joint = currentJoint;
                model->links[model->numLinks - 1] = currentLink;
                printf("Current joint: %s\n", currentJoint.name);
            }
        }
    }

    fclose(file);
}