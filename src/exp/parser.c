#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_BRAND_LENGTH 50

typedef struct {
    char brand[MAX_BRAND_LENGTH];
    int year;
    float engineCapacity;
} Car;

// Function to trim leading and trailing spaces
char *trim(char *str) {
    char *end;

    // Trim leading spaces
    while (isspace((unsigned char)*str)) str++;

    if (*str == '\0')  // All spaces?
        return str;

    // Trim trailing spaces
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator
    *(end + 1) = '\0';

    return str;
}

// Function to parse a single car record
int parseCar(const char *line, Car *car) {
    char temp[MAX_BRAND_LENGTH];
    if (sscanf(line, " \"%[^\"]\" , %d , %f ;", temp, &car->year, &car->engineCapacity) == 3 ||
        sscanf(line, "\"%[^\"]\",%d,%f;", temp, &car->year, &car->engineCapacity) == 3) {
        strncpy(car->brand, trim(temp), MAX_BRAND_LENGTH);
        return 1;
    }
    return 0;  // Parsing failed
}

// Function to parse the input and populate an array of Car structs
int parseCars(const char *input, Car *cars, int maxCars) {
    char buffer[1024];
    int count = 0;
    const char *start = input;
    while (*start != '\0' && count < maxCars) {
        const char *end = strchr(start, ';');
        if (!end) break;  // No more records
        size_t len = end - start + 1;
        if (len >= sizeof(buffer)) len = sizeof(buffer) - 1;
        strncpy(buffer, start, len);
        buffer[len] = '\0';8

        if (parseCar(buffer, &cars[count])) {
            count++;
        }
        start = end + 1;
    }
    return count;
}

// Example usage
int main() {
    const char *input =
        "    \"Toyota\",    2020 , 2.5 ;\n"
        "    \"Honda\",2021,1.8 ;\n"
        "\"Ford\", 2019 , 3.0;";

    Car cars[10];
    int carCount = parseCars(input, cars, 10);

    printf("Parsed Cars:\n");
    for (int i = 0; i < carCount; i++) {
        printf("Brand: %s, Year: %d, Engine Capacity: %.1f\n",
               cars[i].brand, cars[i].year, cars[i].engineCapacity);
    }

    return 0;
}

/*
spacing is irrelevant, as long as ; ends it
unlike csv and Json, it's support comments only single //'

    "Toyota",
    2020,
    2.5
;

    "Honda",
    2021,
    1.8
;

    "Ford",
    2019,
    3.0
;

or 

"Toyota",2020,2.5
;

"Honda",2021,1.8
;

  "Ford",2019,3.0
;

or 

"Toyota",2020,2.5;

"Honda",2021,1.8;

  "Ford",2019,3.0;

*/

// the above main.c parse the file to a c valid struct or union