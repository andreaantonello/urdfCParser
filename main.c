#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "src/parser.h"


int main(int argc, const char *argv[]) {
    URDFModel model;
    model.links = NULL;
    model.numLinks = 0;


    parseURDF("../data/test.urdf", &model);

    for (int i = 0; i < model.numLinks; i++) {
        URDFLink link = model.links[i];
        printf("Link name: %s\n", link.name);
        printf("Parent link: %s\n", link.parent);
        printf("Joint name: %s\n", link.joint.name);
        printf("Joint position: %.2f %.2f %.2f\n", link.joint.position[0], link.joint.position[1], link.joint.position[2]);
        printf("Joint orientation: %.2f %.2f %.2f %.2f\n", link.joint.orientation[0], link.joint.orientation[1],
               link.joint.orientation[2], link.joint.orientation[3]);
        printf("\n");
    }

    free(model.links);


    printf("First executable test. URDF parser project \n");
    return 0;
}
