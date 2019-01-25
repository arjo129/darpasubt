#include <subt_seed/LocalizationMethodManager.h>


LocalizationMethodManager::LocalizationMethodManager() {
    lastUpdate = std::chrono::system_clock::now();
}

void LocalizationMethodManager::registerLocalizationMethod(LocalizationMethod& method) {
    method.localizationManager = this;
    localizationMethods.push_back(&method);
}

void LocalizationMethodManager::newPositionSensorDataAvailable(ProbabilityDistribution &ProbabilityDistribution) {
    auto currTime = std::chrono::system_clock::now();
    long seconds = std::chrono::duration_cast<std::chrono::seconds>(lastUpdate - currTime).count();
    position.gaussianBlur((int)seconds*DRIFT_RATE);
}

void LocalizationMethodManager::setPositionDistribution(ProbabilityDistribution& position){
    this->position = position;
}

ProbabilityDistribution& LocalizationMethodManager::getPositionDistribution() {
    return this->position;
}


