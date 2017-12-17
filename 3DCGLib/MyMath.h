#pragma once
#ifndef MYMATH_H
#define MYMATH_H
#include <algorithm>
#undef max
#undef min
#include <DirectXMath.h>
namespace Lib
{
    class MyMath
    {
    public:
        static const float PI;
        static const float PIDIV2;
        static const float PIDIV4;
        static const float PI2;

        // ����value_������min_�`����max_�͈̔͂ɐ�������
        template <class T>
        static T clamp(const T value_, const T max_, const T min_)
        {
            return std::min(std::max(min_, value_), max_);
        }

        // ����value_��limit�ȏ�̏ꍇvalue_��0�ɁA�܂�0�ȉ��̏ꍇvalue_��limit_��
        template <class T>
        static T rollup(const T value_, const T limit_) {
            if (value_ < 0) {
                return limit_;
            }
            else if (value_ > limit_){
                return 0;
            }
            return value_;
        }
    };
}

#endif