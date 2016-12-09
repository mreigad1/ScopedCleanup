#include <functional>

//class will receive pointer to dynamically allocated resource,
//will execute provided callback upon closing scope of ScopedCleanup
//this allows user to specify function for collecting resource
template <class T> class ScopedCleanup {
    std::function<void(T* resource)> _collector;
    T* _resource;
    public:
        ScopedCleanup(T* resource) :
            _collector(NULL),
            _resource(resource) {
        }
        
        ScopedCleanup(T* resource, std::function<void(T*)> collector) :
            _collector(collector),
            _resource(resource) {
        }

        ScopedCleanup(T* resource, void (*collector)(T*)) :
            _collector(bind(collector, std::placeholders::_1)),
            _resource(resource) {
        }
        
        ~ScopedCleanup() {
            if (_collector) {
                //user-specified
                _collector(_resource);
            } else if (_resource) {
                //default case
                delete [] _resource;
            }
        }
};
