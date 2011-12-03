/*
File: ClassifierFactory.cpp
Author: Samuel Barrett
Description: generates classifiers
Created:  2011-12-02
Modified: 2011-12-02
*/

#include "ClassifierFactory.h"
#include <cassert>

#include <learning/WekaParser.h>
#include <learning/ArffReader.h>

//boost::shared_ptr<Classifier> createClassifier(const std::string &name) {
//}

boost::shared_ptr<DecisionTree> createDecisionTree(const std::string &filename, const std::string &dataFilename, double minGainRatio, unsigned int minInstancesPerLeaf, int maxDepth) {
  boost::shared_ptr<DecisionTree> dt;
  if (filename != "") {
    dt = createDecisionTreeFromWeka(filename);
  } else {
    assert(dataFilename != "");
    dt = createBlankDecisionTreeFromArff(dataFilename);
  }
  dt->setLearningParams(minGainRatio, minInstancesPerLeaf, maxDepth);
  if (dataFilename != "") {
    addDataToDecisionTree(dt,dataFilename);
  }
  return dt;
}

boost::shared_ptr<DecisionTree> createDecisionTreeFromWeka(const std::string &filename) {
  WekaParser parser(filename,Action::NUM_ACTIONS,true);
  return parser.makeDecisionTree();
}

boost::shared_ptr<DecisionTree> createBlankDecisionTreeFromArff(const std::string &dataFilename) {
  std::ifstream in(dataFilename.c_str());
  ArffReader arff(in);
  in.close();
  boost::shared_ptr<DecisionTree> dt(new DecisionTree(arff.getFeatureTypes()));
  return dt;
}

void addDataToDecisionTree(boost::shared_ptr<DecisionTree> dt, const std::string &dataFilename) {
  std::ifstream in(dataFilename.c_str());
  ArffReader arff(in);
  while (!arff.isDone()) {
    InstancePtr instance = arff.next();
    dt->addData(instance);
  }
  in.close();
  dt->train();
}
