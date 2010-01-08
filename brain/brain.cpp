

int receptor_limit = 4;
int tree_depth = 4;
int target_limit = 1000;

class CellComponent{
    float activity;                   
    float threshold;                  //How much activity is required to fire
    int stage;                        //0 = development, 1=mature
}

class Lobe{
    public:
        int lobe_number;
        Neuron *child_neurons;
        float lobe_location[2][2];                      //[0][0] lower left, [1][0] lower right, [0][1] upper left, [1][1] upper right
        float target_location[2][2];                    //[0][0] lower left, [1][0] lower right, [0][1] upper left, [1][1] upper right
        float neuron_density;                           //how many neurons does this lobe grow per unit area
        int stage;                                      //0 = development, 1 = mature
        int emitters[4];                                 //a list that determines which dendrites can synapse onto neurons from this lobe
        int receptors[4];                                //a list that determines which neurons can be targets of dendrites from this lobe
        int tree_branchiness[5];                         //used to build the neuron tree. An array that holds the number of branches at each level of the dendrite tree
        int tree_thresholds[5];                          //the number of branches that have to be active at each position in the branch to fire through. Neurons have their own value that they can adapt
        int signal_duration[5];                          //how long a branch in the tree holds onto signal
        float tree_pruning[5];                           //after building the tree, this proportion of branches are pruned
        Lobe (int number, float density, float home_coordinates, float target_coordinates int emit, int receive, int tree_branch, int tree_thresh, int signal_dur, int tree_prune);
        Neuron* grow_neuron(float lobe_location, int emitters, int receptors, int tree_branchiness, int tree_thresholds, int signal_duration, int tree_pruning);
        
        //to grow neuron, call the constructor for Neuron, with pointer back to lobe
        //to delete neuron, call destructor
        
};
Lobe::Lobe(number, density, home_coordinates, target_coordinates, emit, receive, tree_branch, tree_thresh, signal_dur, tree_prune){
    lobe_number = number;
    neuron_density = density;
    emitters = emit;
    receptors = receive;
    tree_branchiness = tree_branch;
    tree_thresholds = tree_thresh;
    signal_duration = signal_dur;
    tree_pruning = tree_prune;
    lobe_location = home_coordinates;
    lobe_neuron_count = 0;
    stage = 0;
    }

Neuron* grow_neuron(lobe_location, emitters, receptors, tree_branchiness, tree_thresholds, signal_duration, tree_pruning){
    Neuron *created_neuron = new Neuron(lobe_location, emitters, receptors, tree_branchiness, tree_thresholds, signal_duration, tree_pruning);
    return created_neuron;
}


//The neuron class receives activity from the dendrite class and then passes activity onto the dendrite tips that connect to it
class Neuron public CellComponent{
private:
    ///Neuron location
    Vector3f neuron_location;
    Lobe *parent_lobe;
    //Array of child branches
    std::vector<Branch *>child_branches;
    //Array of dendrites connections
    std::vector<Dendrite_tip *>connected_dendrites;
    /**
     * Fire activates a DendriteTip
     * \param target is the DendriteTip that should be activated
     * \returns whether the signal successfully passed through the inter-neuron gap
     */
    bool fire(DendriteTip *target);
public:
    Neuron(int number, int threshold, Lobe *lobe/*parent?*/, Vector3f location);
    void fire();
    void grow_branch(Branch *child_branch);
    void detach_dendrite();
    ///Simulates one millisecond of neural time
    void tick();
};

Neuron::Neuron(int threshold,Lobe *lobe,float location,int emitters,int receptors, tree_branchiness, tree_thresholds, signal_duration, tree_pruning){
    neuron_number = number;
    cell_threshold = threshold;
    parent_lobe = lobe;
    cell_location = location;
    cell_threshold = 0;
    cell_activity = 0;
    stage = 0; 
}

void Neuron::fire(->target){
    target -> activate();
}

void Neuron::detach_dendrite()


//The branch class covers all branch points in the dendrite tree. When a dendrite is active, it passes activity
//to the previous dendrite branch (whose identity it stores as a pointer). Neurons at the base 
class Branch public CellComponent{
    public:
        int signal_strength;
        Branch *upstream_branch;            //0 for a branch at the base, signifying to fire the neuron
        Neuron *parent_neuron;              //Which neuron does this dendrite belong to
        Dendrite_tip *child_dendrite_tips;  //Which dendrite tips come from this branch
        Branch *child_branch                //Pointers to branches that come off of this branch
        int depth;                          //How many levels into the dendrite tree this branch is
        int branches;                       //How many branches come immediately off of this point
        void activate(float excitation);
        void fire_branch(Branch ->upstream_branch);
        void fire_neuron(Neuron ->parent_neuron);
        void strengthen(Branch ->child_branch, Dendrite_tip ->child_dendrite_tip, float reward); //for punishment, use negative reward

};

void Branch::fire_branch(Branch ->upstream_branch, signal_strength){
    upstream_branch -> set_activity(1.00);
}

void Branch::fire_neuron(Neuron ->parent_neuron){
    parent_neuron -> set_activity(1.00);
}

void Branch::strengthen(->child_branch, ->child_dendrite_tip, reward){
    if(child_dendrite_tip > 0){
        child_dendrite_tip -> strenghen_tip(reward);
    }
    child_branch -> strengthen(reward);
}

void activate(excitation){
    activity += excitation
    if(activity ) threshold){
        upstream_branch -> fire_branch;
    }
}


//The dendrite_tip represents connections from the dendrite tree onto other neurons. Neurons pass activity onto
//dendrite_tips and dendrite_tips pass that activity onto branches
class Dendrite_tip public CellComponent{
    public:
        Branch *parent_branch
        Neuron *recipient;
        float connection_strength;
        void fire(Branch ->parent_branch, Dendrite_tip ->self);
        void connect_tip();
        int detach(Neuron ->recipient, Dendrite_tip ->self);                                 //Detach dendrite tip from target neuron return 1 for success
        void activate(void);
        void fire_branch(Branch ->upstream_branch);
        void strengthen_tip(float reward, Dendrite_tip ->self, Neuron ->recipient);
};

int Dendrite_tip::detach(->recipient, ->self){
    recipient -> detach_dendrite(self);
    *recipient = 0;
}
    
void Dendrite_tip::fire(->parent_branch, ->self){
    self -> set_acivity(0);
    parent_branch -> activate(1)
    
}

void Dendrite_tip::strengthen_tip(reward, ->self, ->recipient){
    connection_strength += reward;          //could use a more complicated reward function, this is rather naive
    if(connection_strength > 1){
        connection_strength = 1;
    }
    if(connection_strength <= 0){
        detach_dendrite(-> recipient, ->self);
    }
}

void Dendrite_tip::set_activity(float signal_strength){
    activity = signal_strength;
}
//==============================================================================MAIN
void main(){
int Neuron::neuron_limit = 200;
Neuron neuron_list[];
Branch branch_list[];
Dendrite_tip tip_list[];
}









