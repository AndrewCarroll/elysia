#include "RotatingDevelopment.hpp"
#include "DevelopmentFactory.hpp"
namespace Elysia {
static Development*makeRotatingDevelopment() {
    return new RotatingDevelopment;
}

bool RotatingDevelopment::initRotatingDevelopmentLibrary() {
    DevelopmentFactory::getSingleton().registerConstructor ("Refined",&makeRotatingDevelopment,true);
    return true;
}
bool RotatingDevelopment::deinitRotatingDevelopmentLibrary() {
    DevelopmentFactory::getSingleton().unregisterConstructor ("Refined");
    return true;
}

RotatingDevelopment::RotatingDevelopment() {
    mDevelopmentSignal=0;
    mBestDevelopmentSignal=0;
}

/**
 *	@param float signal - signal to pass to parent component
 *
 *	Description:	For Neurons: end recursion and store the development signal
 *                  For Branches: If the activity is above a threshold, then add weight to the signal.
 *					Passes this signal argument, with possible added weight, to its parent
 *					component.
**/
void RotatingDevelopment::passDevelopmentSignal(CellComponent*component,
                                             float signal){
    CellComponent *parentComponent=mParent;
    if (component==parentComponent) {
        //assume it's a neuron
        mDevelopmentSignal+=signal;
        if (mDevelopmentSignal>mBestDevelopmentSignal) {
            //HAVING THIS BREAKS THE ALGORITHM BY SHUTTLING TO LATER DEVELOPMENT
			mBestDevelopmentSignal=mDevelopmentSignal;//FIXME is this the best place to set mBestDevelopmentSignal
        }
    }else {
        Branch* b=dynamic_cast<Branch*>(component);//a little ugly we have to dynamic cast here
        //but I think that's the price we pay for unifying the class functions into a single glob here in the plugin
        assert(b);
        if(b->activityGreaterThanThreshold()){
            signal += b->signalWeight();
        }
        //recursion on parent component
        if (b->parent()){
            passDevelopmentSignal(b->parent(),signal);
        }
    }
}
///Simply route call to private overload
void RotatingDevelopment::passDevelopmentSignal(Synapse*s,
                                             CellComponent*sParent,
                                             float signalWeight){
    //FIXME: do we want to muck with  s->mConnectionStrength? That might be sensical based on some return value of recursion
    passDevelopmentSignal(sParent,signalWeight);
}

/**
 *	@param const ActivityStats& stats - activity statistics
 *
 *	Description:	If the neuron is in its early states then give it some initial strengthening or weakening.
 *					Otherwise, the connection is strengthened or weakened based on some other function (see code).
 *					If this synapse isn't connected to anything, then the above does not occur and the synapse is connected
 *					to a neuron based on the connect() function
**/



//FIXME have RotatingDevelopment read the following from the genes file: perhaps have some default that's modified by the genes present or absent in the region?
#define _STRENGTHEN_AMOUNT_		0.0f
#define _EARLY_DEV_WINDOW_		30	
#define _INITIAL_STRENGTHEN_	0.04f
#define _INITIAL_WEAKEN_		-0.2f
#define _CHANGE_SIZE_			0.1f
#define _MAX_STRENGTHEN_		0.1f
#define _MAX_WEAKEN_			-0.3f
#define _STRENGTHEN_RANGE_		2.0f
#define _DISCONNECT_THRESHOLD_	0.3f
#define _LATE_DEV_WINDOW_		270
//ENDFIXME


//Ok, the current dilemma is this. If we weaken with every timestep, we are going to weaken out "rare" events. If the training set exceeds
//greatly the development evaluation threshold, we may not be able to learn at all. So we need a way to deal with this.
//Possible solutions are:
//--Enforcing a "maximum" size of any given training set
//--Only weakening/detaching the weakest set of connections if no activity approaches the former best
//--Simply keep the weaken rate per round low


//THIS IS WHAT I AM GOING TO DO HERE FOR NOW...
//--Scale the weaken rate to the best signal observed.
//--------Weaken a lot if the signal in this round is close to the "best signal"
//--------Do not weaken if the signal this round is far from the "best signal"
//
//One potential downside is that this can lead to some unstable equilibrium where multiple "close" models end up weakening each other
//so this may need further refinement

void RotatingDevelopment::developSynapse(Synapse *s, const ActivityStats&stats){
	float strengthenAmount=_STRENGTHEN_AMOUNT_;
	int earlyDevelopmentWindow = _EARLY_DEV_WINDOW_;		//How many concurrent synapses firing is required to move to "mid-development"
	float initialStrengthen = _INITIAL_STRENGTHEN_;			//How much to strengthen a firing synapse in early development
	float initialWeaken = _INITIAL_WEAKEN_;
	float changeSize = _CHANGE_SIZE_;				//Controls how quickly strengthening and weakening happen in mid-development
	float maxStrengthen = _MAX_STRENGTHEN_;			//Maximum amount to strengthen in mid-development
	float maxWeaken = _MAX_WEAKEN_;					//Maximum amount to weaken in mid-development
	float strengthenRange = _STRENGTHEN_RANGE_;		//The multiplier to the level of signal that determines how much and whether to strengthen the synapse 
	int fixationThreshold = _LATE_DEV_WINDOW_;      //After this threshold has been reached, the neuron will not continue to weaken connections


	if(mBestDevelopmentSignal > fixationThreshold){
		s->setDevelopmentStage(1);
	}
	int developmentStage = s->getDevelopmentStage();

	if(s->recipient()){
		if(developmentStage == 0){
    		if(mBestDevelopmentSignal < earlyDevelopmentWindow){				//Neuron still in early state
	    		if(s->mFiringCounter > 0){										//If the synapse is active and not helping the neuron, weaken. If it is active and is helping, strengthen
	    			s->mConnectionStrength += initialStrengthen;				//Strengthen weakly in beginning
	    		}
	    		else{
	    			s->mConnectionStrength += initialWeaken;
					if(s->mConnectionStrength < _DISCONNECT_THRESHOLD_){
						s->detach();
					}
	    		}
			}
			//Neuron is in late development stage, alter strategy to more quickly prune bad connections
			else{
	    		if(s->mFiringCounter > 0){
	    			strengthenAmount = changeSize*(strengthenRange*mDevelopmentSignal - mBestDevelopmentSignal)/(mDevelopmentSignal+0.001f);
	    			if(strengthenAmount > maxStrengthen){strengthenAmount = maxStrengthen;}
	    			if(strengthenAmount < maxWeaken){strengthenAmount = maxWeaken;}
	    			s->mConnectionStrength += strengthenAmount;
	    		}
	    		else{
	    			strengthenAmount = -1*mDevelopmentSignal/mBestDevelopmentSignal;
	    			if(strengthenAmount < maxWeaken){strengthenAmount = maxWeaken;}
	    			s->mConnectionStrength += strengthenAmount;
					if(s->mConnectionStrength < _DISCONNECT_THRESHOLD_){
						s->detach();
					}
				}
			}
		}
	}
    else{
        s->connect();
    }
    
}
void RotatingDevelopment::mature(){
    mDevelopmentStage=1;
}

}
