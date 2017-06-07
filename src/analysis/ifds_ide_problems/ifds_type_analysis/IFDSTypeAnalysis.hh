/*
 * IFDSTypeAnalysis.hh
 *
 *  Created on: 06.12.2016
 *      Author: pdschbrt
 */

#ifndef ANALYSIS_IFDS_IDE_PROBLEMS_IFDS_TYPE_ANALYSIS_IFDSTYPEANALYSIS_HH_
#define ANALYSIS_IFDS_IDE_PROBLEMS_IFDS_TYPE_ANALYSIS_IFDSTYPEANALYSIS_HH_

#include "../../ifds_ide/DefaultIFDSTabulationProblem.hh"
#include "../../ifds_ide/DefaultSeeds.hh"
#include "../../ifds_ide/FlowFunction.hh"
#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <map>
#include <memory>
#include <set>
#include "../../ifds_ide/ZeroValue.hh"
#include "../../icfg/LLVMBasedICFG.hh"
using namespace std;
class IFDSTypeAnalysis
    : public DefaultIFDSTabulationProblem<
          const llvm::Instruction *, const llvm::Value *,
          const llvm::Function *, LLVMBasedICFG &> {
public:
  IFDSTypeAnalysis(LLVMBasedICFG &icfg);

  virtual ~IFDSTypeAnalysis() = default;

  shared_ptr<FlowFunction<const llvm::Value *>>
  getNormalFlowFunction(const llvm::Instruction *curr,
                        const llvm::Instruction *succ) override;

  shared_ptr<FlowFunction<const llvm::Value *>>
  getCallFlowFuntion(const llvm::Instruction *callStmt,
                     const llvm::Function *destMthd) override;

  shared_ptr<FlowFunction<const llvm::Value *>>
  getRetFlowFunction(const llvm::Instruction *callSite,
                     const llvm::Function *calleeMthd,
                     const llvm::Instruction *exitStmt,
                     const llvm::Instruction *retSite) override;

  shared_ptr<FlowFunction<const llvm::Value *>>
  getCallToRetFlowFunction(const llvm::Instruction *callSite,
                           const llvm::Instruction *retSite) override;

  map<const llvm::Instruction *, set<const llvm::Value *>>
  initialSeeds() override;

  const llvm::Value *createZeroValue() override;
};

#endif /* ANALYSIS_IFDS_IDE_PROBLEMS_IFDS_TYPE_ANALYSIS_IFDSTYPEANALYSIS_HH_   \
          */
