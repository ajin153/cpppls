#ifndef CPPPLS_LIFECYCLE_FACTORY_H
#define CPPPLS_LIFECYCLE_FACTORY_H

#include "request_handler.h"

class LifeCycleFactory: public RequestHandlerFactory {
public:
    enum class LifeCycleType : int {
        INITIALIZE = 0,
        INITIALIZED,
        OTHER
    };

    LifeCycleFactory(std::string);
    ~LifeCycleFactory() = default;

    RequestHandler* create_handler() override;
private:
    // 记录当前request属于initialize、initialized等的哪一种
    LifeCycleType req_type;
};


#endif