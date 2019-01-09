#include <subt_seed/ProbabilityDistribution.h>


ProbabilityDistribution::ProbabilityDistribution(){

}

bool ProbabilityDistribution::operator == (const ProbabilityDistribution& other) const {
    
    if(other.positions.size() != positions.size())
        return false;
    
    auto myIter = positions.begin();
    auto otherIter = other.positions.begin();

    while(myIter != positions.end()) {
        
        if(*myIter != *otherIter) return false;

        //Stupid IEEE floats. Cannot compare inequality without fancy comparisont
        //If probability at position is different then the result is different
        double first = probability.find(*myIter)->second;
        double second = other.probability.find(*otherIter)->second;

        if(fabs(first-second) > std::numeric_limits<double>::epsilon()) {
            return false;
        }

        myIter++;
        otherIter++;
    }

    return true;
}

bool ProbabilityDistribution::operator != (const ProbabilityDistribution& other) const {
    return !(other == *this);
}

ProbabilityDistribution& ProbabilityDistribution::operator && (ProbabilityDistribution& other){
    
    ProbabilityDistribution* myDistr = new ProbabilityDistribution();
    auto myPtr = positions.begin();
    auto otherPtr = other.positions.begin();
    
    while(myPtr != positions.end() && otherPtr != other.positions.end()) {
        
        if(*myPtr == *otherPtr){
            Eigen::Vector3f vec(std::get<0>(*myPtr), std::get<1>(*myPtr), std::get<2>(*myPtr));
            myDistr->updateProbability(vec, probability[*myPtr]*other.probability[*myPtr]);
        }
        
        if(*myPtr <= *otherPtr){
            myPtr++;
        } else {
            otherPtr++;
        }
    }

    return *myDistr;
}

void ProbabilityDistribution::updateProbability(Eigen::Vector3f position, double prob){
    std::tuple<int, int, int> pos = std::make_tuple(position[0], position[1], position[2]);

    if(prob < THRESHOLD_PROBABILITY) {
        if(probability.find(pos) != probability.end()) {
            probability.erase(pos);
            positions.erase(pos);
            insertion_index.erase(pos);
        }
    }

    if(probability.find(pos) == probability.end()){
        positions.insert(pos);
        insertion_index[pos] = ++index;
    }

    probability[pos] = prob;
}

void ProbabilityDistribution::clear() {
    positions.clear();
    probability.clear();
    insertion_index.clear();
}

double ProbabilityDistribution::get(Eigen::Vector3f position) const {
    std::tuple<int, int, int> pos = std::make_tuple(position[0], position[1], position[2]);
    auto iter = probability.find(pos);
    if(iter == probability.end()) {
        return 0;
    }

    return iter->second;
}

void ProbabilityDistribution::sample(int n, std::vector<Eigen::Vector3f>& samples) {

}

void ProbabilityDistribution::normalize() {
    double sum = 0;
    
    for(auto a: probability) {
        sum += a.second;
    }

    for(auto a: probability) {
        probability[a.first] /= sum;
    }
}