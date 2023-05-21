#include "parser.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

NUM_JOINTS = 6;

const char* joint_prefix = "joint_";
const char* link_prefix = "link_";

char* composeString(const char* prefix, char* number) {
    // Determine the maximum size needed for the composed string
    int maxSize = strlen(prefix) + 12; // 12 accounts for the maximum digits in a signed 32-bit integer
    // Create a buffer to hold the composed string
    char* result = (char*)malloc(maxSize * sizeof(char));
    // Compose the string using sprintf
    sprintf(result, "%s%s", prefix, number);
    return result;
}

char* composeExpectedName(const char* prefix, int iter) {
    char identifier[12];
    if (iter < NUM_JOINTS) {
        sprintf(identifier, "%d", iter);
    } else if (iter == NUM_JOINTS) {
        strcpy(identifier, "ee");
    }
    char* exprectedName = composeString(prefix, identifier);
    return exprectedName;
}

static void handleParsingError(const char* errorMsg) {
    printf("Parsing error: %s\n", errorMsg);
}

static xmlNodePtr findChildNode(xmlNodePtr parentNode, const char* childName) {
    xmlNodePtr childNode = parentNode->children;
    while (childNode != NULL) {
        if (xmlStrcmp(childNode->name, (const xmlChar*)childName) == 0)
            return childNode;
        childNode = childNode->next;
    }
    return NULL;
}

static const char* getAttribute(xmlNodePtr node, const char* attributeName) {
    xmlChar* attributeValue = xmlGetProp(node, (const xmlChar*)attributeName);
    if (attributeValue == NULL)
        return NULL;
    const char* value = (const char*)attributeValue;
    return value;
}

static void extractMass(xmlNodePtr node, URDFDataAll* urdfData, int index) {
    const char* attributeName = "value";
    const char *massValue = getAttribute(node, attributeName);
    if (massValue)
        (*urdfData)[index].link.mass = atof(massValue);
}

static void extractInertia(xmlNodePtr node, URDFDataAll* urdfData, int index) {
    const char *ixxValue = getAttribute(node, "ixx");
    const char *ixyValue = getAttribute(node, "ixy");
    const char *ixzValue = getAttribute(node, "ixz");
    const char *iyyValue = getAttribute(node, "iyy");
    const char *iyzValue = getAttribute(node, "iyz");
    const char *izzValue = getAttribute(node, "izz");
    if (ixxValue && ixyValue && ixzValue && iyyValue && iyzValue && izzValue) {
        (*urdfData)[index].link.inertia.ixx = atof(ixxValue);
        (*urdfData)[index].link.inertia.ixy = atof(ixyValue);
        (*urdfData)[index].link.inertia.ixz = atof(ixzValue);
        (*urdfData)[index].link.inertia.iyy = atof(iyyValue);
        (*urdfData)[index].link.inertia.iyz = atof(iyzValue);
        (*urdfData)[index].link.inertia.izz = atof(izzValue);
    }
}

static void extractCom(xmlNodePtr node, URDFDataAll* urdfData, int index) {
    const char* attributeName = "xyz";
    const char *attribute = getAttribute(node, attributeName);
    if (attribute)
        sscanf(attribute, "%lf %lf %lf",
               &(*urdfData)[index].link.com[0],
               &(*urdfData)[index].link.com[1],
               &(*urdfData)[index].link.com[2]);
}

static void extractJointOrigin(xmlNodePtr node, URDFDataAll* urdfData, int index) {
    const char* attributeName = "xyz";
    const char *attribute = getAttribute(node, attributeName);
    if (attribute)
        sscanf(attribute, "%lf %lf %lf",
               &(*urdfData)[index].joint.origin[0],
               &(*urdfData)[index].joint.origin[1],
               &(*urdfData)[index].joint.origin[2]);
}

static void extractJointAngles(xmlNodePtr node, URDFDataAll* urdfData, int index) {
    const char* attributeName = "rpy";
    const char *attribute = getAttribute(node, attributeName);
    if (attribute)
        sscanf(attribute, "%lf %lf %lf",
               &(*urdfData)[index].joint.rpy[0],
               &(*urdfData)[index].joint.rpy[1],
               &(*urdfData)[index].joint.rpy[2]);
}

static void extractJointAxis(xmlNodePtr node, URDFDataAll* urdfData, int index) {
    const char* attributeName = "xyz";
    const char *attribute = getAttribute(node, attributeName);
    if (attribute)
        sscanf(attribute, "%lf %lf %lf",
               &(*urdfData)[index].joint.axis[0],
               &(*urdfData)[index].joint.axis[1],
               &(*urdfData)[index].joint.axis[2]);
}

static void extractJointLimits(xmlNodePtr node, URDFDataAll* urdfData, int index) {
    const char *limitLower = getAttribute(node, "lower");
    const char *limitUpper = getAttribute(node, "upper");
    if (limitLower && limitUpper) {
        (*urdfData)[index].joint.limit_lower = atof(limitLower);
        (*urdfData)[index].joint.limit_upper = atof(limitUpper);
    }
}

int parseURDF(const char* urdfFilename, URDFDataAll* urdfData) {
    xmlDocPtr doc;
    xmlNodePtr rootNode, childNode;
    int iter_joint = 0;
    int iter_link = 0;

    /* Parse the URDF file */
    doc = xmlParseFile(urdfFilename);
    if (doc == NULL) {
        handleParsingError("Failed to parse URDF file.");
        return -1;
    }

    /* Get the root node of the document */
    rootNode = xmlDocGetRootElement(doc);
    if (rootNode == NULL) {
        handleParsingError("Empty URDF file.");
        xmlFreeDoc(doc);
        return -1;
    }

    /* Traverse the URDF tree and sample the required data */
    for (childNode = rootNode->children; childNode != NULL; childNode = childNode->next) {
        if (xmlStrcmp(childNode->name, (const xmlChar*)"link") == 0) {
            xmlNodePtr linkNode = childNode;
            xmlNodePtr inertialNode = linkNode->children;

            /* Save link name */
            const char* linkName = getAttribute(childNode, "name");
            char* expectedName = composeExpectedName(link_prefix, iter_link);

            if (xmlStrcmp(expectedName, linkName) == 0) {
                while (inertialNode != NULL) {
                    if (xmlStrcmp(inertialNode->name, (const xmlChar *) "inertial") == 0) {
                        xmlNodePtr massNode = findChildNode(inertialNode, "mass");
                        xmlNodePtr originNode = findChildNode(inertialNode, "origin");
                        xmlNodePtr inertiaNode = findChildNode(inertialNode, "inertia");

                        if (massNode && originNode && inertiaNode) {
                            extractMass(massNode, urdfData, iter_link);
                            extractCom(originNode, urdfData, iter_link);
                            extractInertia(inertiaNode, urdfData, iter_link);
                            iter_link = iter_link + 1;
                        }
                    }
                    inertialNode = inertialNode->next;
                }
            }
        } else if (xmlStrcmp(childNode->name, (const xmlChar*)"joint") == 0) {
            xmlNodePtr jointNode = childNode;
            xmlNodePtr originNode = findChildNode(jointNode, "origin");
            xmlNodePtr axisNode = findChildNode(jointNode, "axis");
            xmlNodePtr limitNode = findChildNode(jointNode, "limit");

            /* Save joint name */
            const char* jointName = getAttribute(childNode, "name");
            char* expectedName = composeExpectedName(joint_prefix, iter_joint);

            if (xmlStrcmp(expectedName, jointName) == 0) {
                if (originNode && axisNode && limitNode) {
                    extractJointOrigin(originNode, urdfData, iter_joint);
                    extractJointAngles(originNode, urdfData, iter_joint);
                    extractJointAxis(axisNode, urdfData, iter_joint);
                    extractJointLimits(limitNode, urdfData, iter_joint);
                    iter_joint = iter_joint + 1;
                }
            }
        }
    }
    /* Free the XML document */
    xmlFreeDoc(doc);
    return 0;
}