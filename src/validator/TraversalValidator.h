/* Copyright (c) 2020 vesoft inc. All rights reserved.
 *
 * This source code is licensed under Apache 2.0 License,
 * attached with Common Clause Condition 1.0, found in the LICENSES directory.
 */

#ifndef VALIDATOR_TRAVERSALVALIDATOR_H_
#define VALIDATOR_TRAVERSALVALIDATOR_H_

#include "common/base/Base.h"
#include "validator/Validator.h"
#include "planner/Query.h"

namespace nebula {
namespace graph {

// some utils for the validator to traverse the graph
class TraversalValidator : public Validator {
protected:
    enum FromType {
        kInstantExpr,
        kVariable,
        kPipe,
    };

    struct Starts {
        FromType                fromType{kInstantExpr};
        Expression*             srcRef{nullptr};
        std::string             userDefinedVarName;
        std::string             firstBeginningSrcVidColName;
        std::vector<Value>      vids;
    };

    struct Over {
        bool                            isOverAll{false};
        std::vector<EdgeType>           edgeTypes;
        storage::cpp2::EdgeDirection    direction;
        std::vector<std::string>        allEdges;
    };

    struct Steps {
        StepClause::MToN*     mToN{nullptr};
        uint32_t              steps{1};
    };

protected:
    TraversalValidator(Sentence* sentence, QueryContext* qctx) : Validator(sentence, qctx) {}

    Status validateStarts(const VerticesClause* clause, Starts& starts);

    Status validateOver(const OverClause* clause, Over& over);

    Status validateStep(const StepClause* clause, Steps& step);

    PlanNode* projectDstVidsFromGN(PlanNode* gn, const std::string& outputVar);

    std::string buildConstantInput();

    PlanNode* buildRuntimeInput();

    Expression* buildNStepLoopCondition(uint32_t steps) const;

protected:
    Starts                from_;
    Steps                 steps_;
    std::string           srcVidColName_;
    Expression*           src_{nullptr};
    ExpressionProps       exprProps_;
    PlanNode*             projectStartVid_{nullptr};
};

}  // namespace graph
}  // namespace nebula

#endif
