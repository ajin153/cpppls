#ifndef CPPPLS_LIFECYCLE_FACTORY_H
#define CPPPLS_LIFECYCLE_FACTORY_H

#include "handler.h"

class LifeCycleFactory: public HandlerFactory {
public:
    enum class LifeCycleType : int {
        INITIALIZE = 0,
        INITIALIZED,
        OTHER
    };

    LifeCycleFactory(const std::string&);
    ~LifeCycleFactory() = default;

    Handler* create_handler() override;
private:
    // 记录当前request属于initialize、initialized等的哪一种
    LifeCycleType req_type;
};


#endif
