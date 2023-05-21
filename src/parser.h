//
// Created by Andrea Antonello on 20/05/2023.
//

#ifndef URDF_C_PARSER_PARSER_H
#define URDF_C_PARSER_PARSER_H

#include <libxml/parser.h>

//// Struct to hold the sampled URDF data
//typedef struct {
//    // Define your struct members here
//    // Example:
//     float mass;
//     float length;
//     int num_links;
//} URDFData;


typedef struct {
    double ixx;
    double ixy;
    double ixz;
    double iyy;
    double iyz;
    double izz;
} InertiaData;

typedef struct {
    double origin[3];
    double axis[3];
    double rpy[3];
    double limit_lower;
    double limit_upper;
} JointData;

typedef struct {
    double mass;
    double com[3];
    InertiaData inertia;
} LinkData;

typedef struct {
    char* name;
    LinkData link;
    JointData joint;
} URDFData;

typedef URDFData URDFDataAll[8];


// Function to sample URDF data and save it in the provided struct
int parseURDF(const char* urdfFilename, URDFDataAll* urdfData);


#endif /* URDF_C_PARSER_PARSER_H */
