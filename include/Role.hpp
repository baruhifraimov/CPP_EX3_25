#pragma once

namespace coup {
    enum class Role {
        GOVERNOR,
        SPY,
        BARON,
        GENERAL,
        JUDGE,
        MERCHANT
    };

    // Convert Role enum to its string representation
    inline std::string to_string(Role r) {
        switch (r) {
            case Role::GOVERNOR: return "GOVERNOR";
            case Role::SPY:      return "SPY";
            case Role::BARON:    return "BARON";
            case Role::GENERAL:  return "GENERAL";
            case Role::JUDGE:    return "JUDGE";
            case Role::MERCHANT:    return "MERCHANT";
        }
    }
}