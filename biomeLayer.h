#ifndef BLAYER_CLASS_H
#define BLAYER_CLASS_H
#include "CSCIx229.h"
#include "vertexLayer.h"
//the biome layer doesn't determine any information related to the vertex locations/heights
//it takes in a mesh and divides it into regions
//example: vertices [0,0,0], [0,1,0] will be packed into a biome object with region type of forest
//forest regions have a flora list that contains trees and flowers
//forest regions also have a density for each of those
class biomeLayer
{
public:
	vertexLayer::vertexLayer mesh; //list of vertices

    biomeLayer(vertexLayer::vertexLayer mesh); //biome layer constructor


};
#endif