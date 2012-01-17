#include "TrAdaBoost.h"

TrAdaBoost::TrAdaBoost(const std::vector<Feature> &features, bool caching, BaseLearnerGenerator baseLearner, const Json::Value &baseLearnerOptions, unsigned int maxBoostingIterations):
  AdaBoost(features,caching,baseLearner,baseLearnerOptions,maxBoostingIterations)
{
  targetDataStart = -1;
}

void TrAdaBoost::addData(const InstancePtr &instance) {
  if (targetDataStart < 0)
    targetDataStart = data.size();
  AdaBoost::addData(instance);
}

void TrAdaBoost::addSourceData(const InstancePtr &instance) {
  assert(targetDataStart < 0);
  AdaBoost::addData(instance);
}

void TrAdaBoost::reweightData(double alpha) {
  assert(targetDataStart >= 0);
  // source data
  double N = maxBoostingIterations;
  double n = targetDataStart + 1.0; // +1 for 0 indexing
  double beta = 1.0 / (1.0 + sqrt((2.0 / N) * log(n)));
  double sourceAlpha = log(beta);
  for (int i = 0; i < targetDataStart; i++)
    data[i]->weight *= exp(sourceAlpha * absError[i]);
  // target data
  for (unsigned int i = targetDataStart; i < data.size(); i++)
    data[i]->weight *= exp(alpha * absError[i]);
}