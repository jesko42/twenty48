#include "twenty48/row.hpp"

#include <cstddef>

namespace twenty48 {

SlideResult slideLeft(const Row& row) {
    SlideResult result;
    result.row.assign(row.size(), 0);

    std::size_t write = 0;       // next free slot in the output row
    bool lastWasMerged = false;  // a tile may take part in at most one merge (R3)

    for (const int value : row) {
        if (value == 0) {
            continue;  // empty cells are skipped (R2)
        }
        if (write > 0 && !lastWasMerged && result.row[write - 1] == value) {
            result.row[write - 1] = value * 2;
            result.score += value * 2;
            lastWasMerged = true;
        } else {
            result.row[write] = value;
            ++write;
            lastWasMerged = false;
        }
    }

    result.changed = result.row != row;
    return result;
}

}  // namespace twenty48
