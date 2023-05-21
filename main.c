#include <stdio.h>
#include "src/parser.h"

void printDoubleArray(const double arr[], int size) {
    printf("[");
    for (int i = 0; i < size; i++) {
        printf("%lf", arr[i]);
        if (i < size - 1) {
            printf(", ");
        }
    }
    printf("]");
}

void writeURDFDataToJSONFile(const URDFDataAll data, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    printf("Saving into file\n");
    fprintf(file, "[\n");
    for (int i = 0; i < 8; i++) {
        fprintf(file, "  {\n");
//        fprintf(file, "    \"name\": \"%s\",\n", data[i].name);
        fprintf(file, "    \"link\": {\n");
        fprintf(file, "      \"mass\": %lf,\n", data[i].link.mass);
        fprintf(file, "      \"com\": [");
        fprintf(file, "%lf, %lf, %lf],\n", data[i].link.com[0], data[i].link.com[1], data[i].link.com[2]);
        fprintf(file, "      \"inertia\": {\n");
        fprintf(file, "        \"ixx\": %lf,\n", data[i].link.inertia.ixx);
        fprintf(file, "        \"ixy\": %lf,\n", data[i].link.inertia.ixy);
        fprintf(file, "        \"ixz\": %lf,\n", data[i].link.inertia.ixz);
        fprintf(file, "        \"iyy\": %lf,\n", data[i].link.inertia.iyy);
        fprintf(file, "        \"iyz\": %lf,\n", data[i].link.inertia.iyz);
        fprintf(file, "        \"izz\": %lf\n", data[i].link.inertia.izz);
        fprintf(file, "      }\n");
        fprintf(file, "    },\n");
        fprintf(file, "    \"joint\": {\n");
        fprintf(file, "      \"origin\": [");
        fprintf(file, "%lf, %lf, %lf],\n", data[i].joint.origin[0], data[i].joint.origin[1], data[i].joint.origin[2]);
        fprintf(file, "      \"axis\": [");
        fprintf(file, "%lf, %lf, %lf],\n", data[i].joint.axis[0], data[i].joint.axis[1], data[i].joint.axis[2]);
        fprintf(file, "      \"rpy\": [");
        fprintf(file, "%lf, %lf, %lf],\n", data[i].joint.rpy[0], data[i].joint.rpy[1], data[i].joint.rpy[2]);
        fprintf(file, "      \"limit_lower\": %lf,\n", data[i].joint.limit_lower);
        fprintf(file, "      \"limit_upper\": %lf\n", data[i].joint.limit_upper);
        fprintf(file, "    }\n");
        fprintf(file, "  }%s\n", (i < 7) ? "," : "");
    }
    fprintf(file, "]\n");

    fclose(file);
}


void printURDFDataAll(URDFDataAll* data) {
    for (int i = 0; i < 7; i++) {
        printf("URDFDataAll[%d]:\n", i);
        printf("Link Data:\n");
        printf(" - Mass: %lf\n", (*data)[i].link.mass);
        printf(" - Center of Mass: (%lf, %lf, %lf)\n", (*data)[i].link.com[0], (*data)[i].link.com[1], (*data)[i].link.com[2]);
        printf(" - Inertia Data:\n");
        printf("    - Inertia diag: (%lf, %lf, %lf)\n", (*data)[i].link.inertia.ixx, (*data)[i].link.inertia.iyy, (*data)[i].link.inertia.izz);
        printf("    - Inertia off-diag: (%lf, %lf, %lf)\n", (*data)[i].link.inertia.ixy, (*data)[i].link.inertia.ixz, (*data)[i].link.inertia.iyz);
        printf("Joint Data:\n");
        printf(" - Origin: (%lf, %lf, %lf)\n", (*data)[i].joint.origin[0], (*data)[i].joint.origin[1], (*data)[i].joint.origin[2]);
        printf(" - Axis: (%lf, %lf, %lf)\n", (*data)[i].joint.axis[0], (*data)[i].joint.axis[1], (*data)[i].joint.axis[2]);
        printf(" - RPY: (%lf, %lf, %lf)\n", (*data)[i].joint.rpy[0], (*data)[i].joint.rpy[1], (*data)[i].joint.rpy[2]);
        printf(" - Limits: (%lf, %lf)\n", (*data)[i].joint.limit_lower, (*data)[i].joint.limit_upper);
        printf("\n");
    }
}


int main() {
    URDFDataAll urdfData;
    const char* urdfFilename = "../data/test.urdf";

    int result = parseURDF(urdfFilename, &urdfData);

    if (result == 0) {
        printURDFDataAll(&urdfData);
    }

    writeURDFDataToJSONFile(urdfData, "../data/urdf_data.json");

    return 0;
}