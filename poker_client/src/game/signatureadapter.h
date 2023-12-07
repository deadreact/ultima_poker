#pragma once

#include <functional>
#include <qabstractitemmodel.h>
#include <tools/types_fwd.hpp>

namespace ns_model {
namespace ns_detail {

using on_common_reset = std::function<void()>;
using on_slice_resize = std::function<void(const QModelIndex&,int,int)>;
using on_slice_reorder = std::function<void(const QModelIndex&,int,int,const QModelIndex&,int)>;
using on_data_changed = std::function<void(const QModelIndex&,const QModelIndex&,const QVector<int>)>;
using on_parent_specified = std::function<void(const QModelIndex&)>;
using on_range_specified = std::function<void(const QModelIndexRange&)>;
using on_complex_specified = std::function<void(const QModelIndexRangeList&)>;

} //namespace ns_detail
} //namespace ns_model
