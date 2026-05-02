#pragma once
#include "announcement.h"
#include <unordered_map>
#include <vector>
#include <string>

class AS; // forward declare

// BGP policy: stores local RIB and received queue
class BGP {
    friend class AS;
public:
    virtual ~BGP() = default;

    void receive(const Announcement& ann) {
        recv_queue[ann.prefix].push_back(ann);
    }

    const std::unordered_map<std::string, Announcement>& getLocalRib() const {
        return local_rib;
    }

    void seed(const Announcement& ann) {
        local_rib[ann.prefix] = ann;
    }

protected:
    std::unordered_map<std::string, Announcement> local_rib;
    std::unordered_map<std::string, std::vector<Announcement>> recv_queue;

    virtual bool shouldAccept(const Announcement& ann) {
        (void)ann;
        return true;
    }
};

// ROV policy: drops rov_invalid announcements
class ROV : public BGP {
protected:
    bool shouldAccept(const Announcement& ann) override {
        return !ann.rov_invalid;
    }
};
