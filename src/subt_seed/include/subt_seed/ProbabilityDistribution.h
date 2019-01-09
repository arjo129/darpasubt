#ifndef _PROBABILITY_DISTRIBUTION_H_
#define _PROBABILITY_DISTRIBUTION_H_

#include <limits>
#include <set>
#include <tuple>
#include <unordered_map>

#include <boost/functional/hash.hpp>

#include <Eigen/Dense>

#define THRESHOLD_PROBABILITY 0.00000001

/**
 * This class represents
 */ 
namespace std
{

template<typename... T>
struct hash<tuple<T...>>
{
    size_t operator()(tuple<T...> const& arg) const noexcept
    {
        return boost::hash_value(arg);
    }
};

}


/**
 * This class represents a probability distribution in 3D. It is essentially
 * a 3D sparse matrix with values adding up to 1.
 */ 
class ProbabilityDistribution {

private:
    int index;
    std::set<std::tuple<int, int, int> > positions;
    std::unordered_map<std::tuple<int, int, int>, double> probability;
    std::unordered_map<std::tuple<int, int, int>, int> insertion_index;

public:
    ProbabilityDistribution();
    
    /**
     * This function performs an and operation over two probability distributions
     */ 
    ProbabilityDistribution& operator && (ProbabilityDistribution& max);
    
    bool operator == (const ProbabilityDistribution& other) const;

    bool operator != (const ProbabilityDistribution& other) const;
    /**
     * Update the probability at given position 
     */ 
    void updateProbability(Eigen::Vector3f position, double probability);

    /**
     * Clears the matrix
     */ 
    void clear();

    /**
     * get the probability at the given point
     */
    double get(Eigen::Vector3f position) const;

    /**
     * Sample
     */ 
    void sample(int n, std::vector<Eigen::Vector3f>& samples);

    /**
     * Normalize the distribution such that the sum of probabilities = 1
     */ 
    void normalize();
};
#endif