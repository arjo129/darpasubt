#ifndef _LOCALIZATION_METHOD_MANAGER_
#define _LOCALIZATION_METHOD_MANAGER_

#define DRIFT_RATE 10

#include <chrono>
#include <vector>

#include <subt_seed/ProbabilityDistribution.h>

/**
 * This class manages multiple localization methods.
 * In order to create a new Localization Method one should simply
 * observe this class.
 */ 
class LocalizationMethodManager {

private:
    std::vector<class LocalizationMethod*> localizationMethods;
    std::chrono::system_clock::time_point lastUpdate;
    ProbabilityDistribution position, orientation;

public:
    LocalizationMethodManager();
    /**
     * Register itself with the method manager
     */ 
    void registerLocalizationMethod(LocalizationMethod& method);

    /**
     * Update the probability distribution based on new data
     */
    void newPositionSensorDataAvailable(ProbabilityDistribution& ProbabilityDistribution);

    /**
     * When using odometry data this may be prefered
     */
    void setPositionDistribution(ProbabilityDistribution& position);

    /**
     * When using
     */
    ProbabilityDistribution& getPositionDistribution(); 
};

/**
 * All localization methods should derive from this
 */ 
class LocalizationMethod {
private:
    LocalizationMethodManager* localizationManager;
    friend LocalizationMethodManager;
};

#endif
