/*
 * cyBot_FindObjects.h
 *
 *  Created on: Apr 23, 2021
 *      Author: brnorman
 */

#ifndef PROJECT_CODE_FILES_CYBOT_FINDOBJECTS_H_
#define PROJECT_CODE_FILES_CYBOT_FINDOBJECTS_H_

typedef struct{
    int angle; //degrees
    float distance; //cm
    float width; //cm
} cyBot_Object_t;

int cyBot_FindObjects(cyBot_Object_t getObjects[], int size_of_input);




#endif /* PROJECT_CODE_FILES_CYBOT_FINDOBJECTS_H_ */


