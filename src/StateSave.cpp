#include <iostream>
#include <fstream>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shape.h"

int save(char const * filename, std::list<Shape> listOfShapes)
{
	char name[100];
	sprintf(name, /*strlen(filename) + 4,*/ "%s.mcg", filename);
	FILE *savefile = fopen(name, "wb");
	if(savefile == NULL) return 1;
	//free(name);
	
	fprintf(savefile, "MCG_savestate_file\n");
	std::list<Shape>::iterator it;
	for( it=listOfShapes.begin() ; it != listOfShapes.end(); it++ )
	{
		Shape s = *it;
		fprintf(savefile, "%d %f %f %f %f %f %f %f %f %f %f %f %f\n", s.type, 
			s.trans.trans.x, s.trans.trans.y, s.trans.trans.z, s.rot.trans.x, s.rot.trans.y,
			s.rot.trans.z, s.scale.trans.x, s.scale.trans.y, s.scale.trans.z,
			s.color.x, s.color.y, s.color.z/*, s.hseg, s.vseg*/);
	}
	
	fclose(savefile);
	return 0;
}

int load(char const * filename, std::list<Shape> * listOfShapes)
{
	listOfShapes->clear();
	int len = strlen(filename);
	if(len < 4 || filename[len-4] != '.' || filename[len-3] != 'm'
		|| filename[len-2] != 'c' || filename[len-1] != 'g')
		return -2;

	FILE *loadfile = fopen(filename, "r");
	if(loadfile == NULL) return -1;
	
	//char firstLine[50];
	fscanf(loadfile, "MCG_savestate_file\n");
	//if(firstline != "MCG_savestate_file") return -3;
	unsigned int idCounter = 1;
	int st, scanned;
	float t1, t2, t3, r1, r2, r3, s1, s2, s3, c1, c2, c3;
	while(true)
	{
		scanned = fscanf(loadfile, "%d %f %f %f %f %f %f %f %f %f %f %f %f\n",
			&st, &t1, &t2, &t3, &r1, &r2, &r3, &s1, &s2, &s3, &c1, &c2, &c3);
		if(scanned <= 0) break;
		Shape s = Shape((shape_type)st, idCounter, TransformTranslate(vec3(t1, t2, t3)),
			TransformRotate(vec3(r1, r2, r3)), TransformScale(vec3(s1, s2, s3)));
		s.color = vec3(c1, c2, c3);
		listOfShapes->push_back(s);
		idCounter++;
	}
	fclose(loadfile);
	return idCounter;
}
