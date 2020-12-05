#include "insert.h"

#include <utility>

#include "movement.h"
#include "../models/text-model.h"

namespace actions {
Insert::Insert(InsType value, int n, std::unique_ptr<Movement> mvt) : 
    IAction{value, n}, mvt{std::move(mvt)} { }

Movement *Insert::getMvt() { return mvt.get(); }

/*
        {'i', InsType::BEFORE},
        {'a', InsType::AFTER},
        {'O', InsType::NEW_LINE_ABOVE},
        {'o', InsType::NEW_LINE_BELOW},
        {'I', InsType::BEG_LINE}, // before char
        {'A', InsType::END_LINE},
        // {"c", InsType::CH_MVT},
        // {"cc", InsType::CH_LINE},
        {'S', InsType::CH_LINE},
        {'C', InsType::CH}*/

void Insert::execAction(models::TextModel &t) {
    switch (getValue()) {
        case InsType::BEFORE:
        case InsType::AFTER:
        case InsType::NEW_LINE_ABOVE:
        case InsType::NEW_LINE_BELOW:
        case InsType::BEG_LINE:
        case InsType::END_LINE:
        case InsType::CH_MVT:
        case InsType::CH_LINE:
        case InsType::CH:
    }
}
}
