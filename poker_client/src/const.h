#pragma once

#include <QSize>
#include "../../common/api/gbp_int.hpp"

namespace ns_const
{
    constexpr const float eplisonF = 0.001f;
    constexpr const double eplison = 0.001;


    const gbp_i64 c = 1;
    const gbp_i64 n = 100*c;
    const gbp_i64 k = 1000*n;
    const gbp_i64 m = 1000*k;
    constexpr const gbp_i64 chipDenomination[] = {  1*c, 5*c, 25*c, 50*c
                                                  , 1*n, 5*n, 25*n, 100*n, 500*n
                                                  , 1*k, 5*k, 25*k, 100*k, 500*k
                                                  , 1*m, 5*m};
    constexpr const char* errorStyleSheet = "#%0 { border: 1px solid #de123a; background-color: #ffcdd2; padding-top: 24px; margin-top: -24px;}";

    namespace ns_avatar_size
    {
        const int largeRadius = 122;
        const int baseRadius  = 60;
        const int smallRadius = 44;
        const int microRadius = 33;

        const QSize large = QSize(largeRadius*2, largeRadius*2);
        const QSize base  = QSize(baseRadius*2, baseRadius*2);
        const QSize small = QSize(smallRadius*2, smallRadius*2);
        const QSize micro = QSize(microRadius*2, microRadius*2);
    }
}
