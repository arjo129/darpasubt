#include <subt_seed/ProbabilityDistribution.h>


ProbabilityDistribution::ProbabilityDistribution(){
    std::srand(std::time(nullptr));
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

void ProbabilityDistribution::accumulate(Eigen::Vector3f position, double prob) {
    updateProbability(position ,get(position) + prob);
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

    std::map<double, std::tuple<int,int,int> > probab_map;

    double accumulated_prob = 0;
    for(auto prob: probability) {
        accumulated_prob += prob.second;
        probab_map.insert(std::pair<double, std::tuple<int, int, int> >(accumulated_prob, prob.first));
    }

    for(int i  = 0 ; i < n ; i++) {
        double num = (double)std::rand()/RAND_MAX;
        auto location = probab_map.upper_bound(num);
        std::tuple<int,int,int> result = location->second;
        Eigen::Vector3f res(std::get<0>(result), std::get<1>(result), std::get<2>(result));
        samples.push_back(res);
    }
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

ProbabilityDistribution& ProbabilityDistribution::gaussianBlur(int radius){

    for(auto position: positions) {
        Eigen::Vector3f pos(std::get<0>(position), std::get<1>(position), std::get<2>(position));
        double val = get(pos);
        for(int i = -radius; i <= radius; i++){
            for(int j = -radius; j <= radius; j++){
                for(int k = -radius; k <= radius; k++){
                    Eigen::Vector3f offset(i,j,k);
                    double distSq = i*i+k*k+j*j;
                    accumulate(pos + offset, val*exp(-(distSq)/(2*radius)));
                }
            }
        }
    }

    normalize();
}
