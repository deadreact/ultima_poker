#pragma once

namespace ns_model
{
    namespace ns_data
    {
        struct header {
            const unsigned int class_id;
            const unsigned int version;
        };

        template <typename model_type>
        struct Item;

    } // namespace ns_data
} // namespace ns_model
