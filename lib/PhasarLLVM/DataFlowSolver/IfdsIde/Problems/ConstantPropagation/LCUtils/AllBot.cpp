#include "phasar/PhasarLLVM/DataFlowSolver/IfdsIde/Problems/ConstantPropagation/LCUtils/AllBot.h"
#include "phasar/PhasarLLVM/DataFlowSolver/IfdsIde/EdgeFunctions/EdgeIdentity.h"
#include "phasar/PhasarLLVM/DataFlowSolver/IfdsIde/Problems/ConstantPropagation/LCUtils/JoinEdgeFunction.h"

namespace CCPP::LCUtils {
std::shared_ptr<AllBot::type> AllBot::getInstance() {
  static std::shared_ptr<type> ret =
      std::make_shared<type>(IDELinearConstantPropagation::v_t{nullptr});
  return ret;
}
bool AllBot::isBot(
    const psr::EdgeFunction<IDELinearConstantPropagation::v_t> *edgeFn,
    bool nonRec) {
  if (edgeFn == nullptr)
    return false;
  if (edgeFn == getInstance().get())
    return true;
  if (dynamic_cast<const type *>(edgeFn))
    return true;
  if (!nonRec) {
    if (auto joinEFn = dynamic_cast<const JoinEdgeFunction *>(edgeFn))
      return isBot(joinEFn->getFirst(), true) &&
             isBot(joinEFn->getSecond(), true);
  }
  return false;
}
bool AllBot::isBot(
    const std::shared_ptr<psr::EdgeFunction<IDELinearConstantPropagation::v_t>>
        &edgeFn,
    bool nonRec) {
  return isBot(edgeFn.get(), nonRec);
}
} // namespace CCPP::LCUtils