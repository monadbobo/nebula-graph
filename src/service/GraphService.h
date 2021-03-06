/* Copyright (c) 2018 vesoft inc. All rights reserved.
 *
 * This source code is licensed under Apache 2.0 License,
 * attached with Common Clause Condition 1.0, found in the LICENSES directory.
 */

#ifndef SERVICE_GRAPHSERVICE_H_
#define SERVICE_GRAPHSERVICE_H_

#include "common/base/Base.h"
#include "common/interface/gen-cpp2/GraphService.h"
#include "service/Authenticator.h"
#include "service/QueryEngine.h"
#include "service/SessionManager.h"

namespace folly {
class IOThreadPoolExecutor;
}   // namespace folly

namespace nebula {
namespace graph {

class GraphService final : public cpp2::GraphServiceSvIf {
public:
    GraphService() = default;
    ~GraphService() = default;

    Status MUST_USE_RESULT init(std::shared_ptr<folly::IOThreadPoolExecutor> ioExecutor);

    folly::Future<cpp2::AuthResponse> future_authenticate(
        const std::string& username,
        const std::string& password) override;

    void signout(int64_t /*sessionId*/) override;

    folly::Future<cpp2::ExecutionResponse>
    future_execute(int64_t sessionId, const std::string& stmt) override;

    const char* getErrorStr(cpp2::ErrorCode result);

private:
    void onHandle(RequestContext<cpp2::AuthResponse>& ctx, cpp2::ErrorCode code);

    bool auth(const std::string& username, const std::string& password);

    std::unique_ptr<SessionManager>             sessionManager_;
    std::unique_ptr<QueryEngine>                queryEngine_;
};

}   // namespace graph
}   // namespace nebula
#endif   // GRAPH_GRAPHSERVICE_H_
