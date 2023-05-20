//
// Created by Andrea Antonello on 28/04/2023.
//

#ifndef URDF_C_PARSER_PARSER_H
#define URDF_C_PARSER_PARSER_H

typedef struct {
    char name[256];
    double position[3];
    double orientation[4];
} URDFJoint;

typedef struct {
    char name[256];
    char parent[256];
    URDFJoint joint;
} URDFLink;

typedef struct {
    URDFLink* links;
    int numLinks;
} URDFModel;


void parseURDF(const char* filename, URDFModel* model);

#endif /* URDF_C_PARSER_PARSER_H */
