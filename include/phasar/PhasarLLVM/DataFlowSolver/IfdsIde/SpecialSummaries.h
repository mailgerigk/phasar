/******************************************************************************
 * Copyright (c) 2017 Philipp Schubert.
 * All rights reserved. This program and the accompanying materials are made
 * available under the terms of LICENSE.txt.
 *
 * Contributors:
 *     Philipp Schubert and others
 *****************************************************************************/

/*
 * SpecialSummaries.h
 *
 *  Created on: 05.05.2017
 *      Author: philipp
 */

#ifndef PHASAR_PHASARLLVM_IFDSIDE_SPECIALSUMMARIES_H_
#define PHASAR_PHASARLLVM_IFDSIDE_SPECIALSUMMARIES_H_

#include <map>
#include <memory>
#include <ostream>
#include <set>
#include <string>
#include <vector>

#include "phasar/Config/Configuration.h"
#include "phasar/PhasarLLVM/DataFlowSolver/IfdsIde/EdgeFunctions.h"
#include "phasar/PhasarLLVM/DataFlowSolver/IfdsIde/FlowFunctions.h"
#include "phasar/PhasarLLVM/Utils/BinaryDomain.h"
#include "phasar/Utils/IO.h" // readFile

#include "llvm/IR/Function.h"

namespace psr {

template <typename D, typename L = BinaryDomain,
          typename Container = std::set<D>>
class SpecialSummaries {
  using FlowFunctionType =
      typename FlowFunction<D, Container>::FlowFunctionType;
  using FlowFunctionPtrType =
      typename FlowFunction<D, Container>::FlowFunctionPtrType;
  using EdgeFunctionPtrType = typename EdgeFunction<L>::EdgeFunctionPtrType;

private:
  std::map<std::string, FlowFunctionPtrType> SpecialFlowFunctions;
  std::map<std::string, EdgeFunctionPtrType> SpecialEdgeFunctions;
  std::vector<std::string> SpecialFunctionNames;

  // Constructs the SpecialSummaryMap such that it contains all glibc,
  // llvm.intrinsics and C++'s new, new[], delete, delete[] with identity
  // flow functions.
  SpecialSummaries() {
    // insert default flow and edge functions
    for (auto function_name :
         PhasarConfig::getPhasarConfig().specialFunctionNames()) {
      SpecialFlowFunctions.insert(
          std::make_pair(function_name, Identity<D>::getInstance()));
      SpecialEdgeFunctions.insert(
          std::make_pair(function_name, EdgeIdentity<L>::getInstance()));
    }
  }

public:
  SpecialSummaries(const SpecialSummaries &) = delete;
  SpecialSummaries &operator=(const SpecialSummaries &) = delete;
  SpecialSummaries(SpecialSummaries &&) = delete;
  SpecialSummaries &operator=(SpecialSummaries &&) = delete;
  ~SpecialSummaries() = default;

  static SpecialSummaries<D, L> &getInstance() {
    static SpecialSummaries<D, L> instance;
    return instance;
  }

  // Returns true, when an existing function is overwritten, false otherwise.
  bool provideSpecialSummary(const std::string &name,
                             FlowFunctionPtrType flowfunction) {
    bool Override = containsSpecialSummary(name);
    SpecialFlowFunctions[name] = flowfunction;
    return Override;
  }

  // Returns true, when an existing function is overwritten, false otherwise.
  bool provideSpecialSummary(const std::string &name,
                             FlowFunctionPtrType flowfunction,
                             EdgeFunctionPtrType edgefunction) {
    bool Override = containsSpecialSummary(name);
    SpecialFlowFunctions[name] = flowfunction;
    SpecialEdgeFunctions[name] = edgefunction;
    return Override;
  }

  bool containsSpecialSummary(const llvm::Function *function) {
    return containsSpecialSummary(function->getName().str());
  }

  bool containsSpecialSummary(const std::string &name) {
    return SpecialFlowFunctions.count(name);
  }

  FlowFunctionPtrType
  getSpecialFlowFunctionSummary(const llvm::Function *function) {
    return getSpecialFlowFunctionSummary(function->getName().str());
  }

  FlowFunctionPtrType getSpecialFlowFunctionSummary(const std::string &name) {
    return SpecialFlowFunctions[name];
  }

  EdgeFunctionPtrType
  getSpecialEdgeFunctionSummary(const llvm::Function *function) {
    return getSpecialEdgeFunctionSummary(function->getName().str());
  }

  EdgeFunctionPtrType getSpecialEdgeFunctionSummary(const std::string &name) {
    return SpecialEdgeFunctions[name];
  }

  friend std::ostream &operator<<(std::ostream &os,
                                  const SpecialSummaries<D> &ss) {
    os << "SpecialSummaries:\n";
    for (auto &entry : ss.SpecialFunctionNames) {
      os << entry.first << " ";
    }
    return os;
  }
};
} // namespace psr

#endif
