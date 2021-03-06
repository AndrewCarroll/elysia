#include "Platform.hpp"
#include "Placeable.hpp"
#include "AnnSpace.hpp"

#if 0
///placeholder class
class Placeable {
  Vector3f mPlaceableLocation;
public:
  const Vector3f& getLocation()const {return mPlaceableLocation;}
};
#endif

namespace Elysia {

	TreeNNSpatialSearch::TreeNNSpatialSearch(){
		pointUpperThreshold = 10;
		pointLowerThreshold = 2;
		root = new AnnSpace;
	}

	TreeNNSpatialSearch::~TreeNNSpatialSearch(){
		delete root;
	}
/**
 *	@param const Vector3f &queryPoint - point where to begin search
 *	@param Placeable *exclude - pointer to neuron which you don't want to return (i.e. itself)
 *	@returns nearest neighbor
 * 
 *	Description:	Finds nearest neighbor from local copy of list
**/
Placeable* TreeNNSpatialSearch::findNearestNeighbor(const Vector3f &queryPoint, Placeable* exclude){
    Placeable* target = root->findNN(queryPoint.x, queryPoint.y, exclude);
	return target;
}

/**
 *	@param Placeable *neuron - pointer to neuron to insert
 * 
 *	Description:	Function to *add* neighbor/point to local copy of list
**/
void TreeNNSpatialSearch::addNeighbor(Placeable* placeable){
    root->addPoint(placeable, this);
}

/**
 *	@param Placeable *neuron - pointer to neuron to remove
 * 
 *	Description:	Function to *remove* neighbor/point from local copy of list
**/
void TreeNNSpatialSearch::removeNeighbor(Placeable* placeable){
    root->deletePoint(placeable, this);
}

/**
 *	@param Placeable *neuron - pointer to neuron to remove
 * 
 *	Description:	Function to *update* neighbor/point to local copy of list
**/
void TreeNNSpatialSearch::moveNeighbor(Placeable* placeable){
	assert(false);
}

/**
 *	@param AnnSpace* - pointer to the AnnSpace that will become the new root
 * 
 *	Description:	When a merge must occur near the base of the tree, the grandparent of the deleted
 *                  node must be known. Since AnnSpaces adjacent to the root have no grandparents,
 *                  instead the sibling of the delted AnnSpace is set to be the new root.
 *                  
**/
void TreeNNSpatialSearch::reassignRoot(AnnSpace* newRoot){
	root = newRoot;
}

}
