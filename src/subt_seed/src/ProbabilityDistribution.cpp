#include <subt_seed/ProbabilityDistribution.h>



ProbabilityDistribution::ProbabilityDistribution(){

}

bool ProbabilityDistribution::operator == (const ProbabilityDistribution& other) {
    
    if(other.positions.size() != positions.size())
        return false;
    
    auto myIter = positions.begin();
    auto otherIter = other.positions.begin();

    while(myIter != positions.end()) {
        
        if(*myIter != *otherIter) return false;

        if(probability[*myIter] != other.probability[*otherIter]) return false;

        myIter++;
        otherIter++;
    }

    return true;
}


ProbabilityDistribution& ProbabilityDistribution::operator && (ProbabilityDistribution& other){
    
    ProbabilityDistribution myDistr;
    auto myPtr = positions.begin();
    auto otherPtr = other.positions.begin();
    
    while(myPtr != positions.end() || otherPtr != other.positions.end()){
        
        if(*myPtr == *otherPtr){
            Eigen::Vector3f vec(std::get<0>(myPtr), std::get<1>(myPtr), std::get<2>(myPtr));
            myDistr.updateProbability(vec, probability[*myPtr]*other.probability[*myPtr]);
        }
        
        if(*myPtr <= *otherPtr){
            myPtr++;
        } else {
            otherPtr++;
        }
    }
}

void ProbabilityDistribution::updateProbability(Eigen::Vector3f position, float prob){
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

float ProbabilityDistribution::get(Eigen::Vector3f position) {
    std::tuple<int, int, int> pos = std::make_tuple(position[0], position[1], position[2]);

    if(probability.find(pos) == probability.end()) {
        return 0;
    }

    return probability[pos];
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