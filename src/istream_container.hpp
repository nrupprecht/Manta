//
// Created by Nathaniel Rupprecht on 1/30/21.
//

#ifndef MANTA_ISTREAM_CONTAINER_H
#define MANTA_ISTREAM_CONTAINER_H

#include "utility.hpp"

class istream_container {
private:
    struct container {
        virtual std::istream& get_stream() const = 0;
        virtual bool is_good() const = 0;
    };

    struct container_ptr : public container {
        explicit container_ptr(const std::shared_ptr<std::istream>& ptr) : in_(ptr) {};

        std::istream& get_stream() const override {
            return *in_;
        }

        bool is_good() const override {
            return in_ != nullptr;
        }

        std::shared_ptr<std::istream> in_;
    };

    struct container_ref : public container {
        explicit container_ref(std::istream& in) : in_(in) {};

        std::istream& get_stream() const override {
            return in_;
        }

        bool is_good() const override {
            return true;
        }

        std::istream& in_;
    };

    std::shared_ptr<container> stream_container;

    explicit istream_container(std::shared_ptr<container>& ptr)
            : stream_container(std::move(ptr)) {};

public:
    //! \brief Default constructor.
    istream_container() {};

    std::istream& get_istream() {
        return stream_container->get_stream();
    }

    static istream_container open_file(const std::string& filename) {
        std::shared_ptr<std::istream> strm = std::make_shared<std::ifstream>(filename);
        std::shared_ptr<container> con = std::make_shared<container_ptr>(strm);
        return istream_container(con);
    }

    static istream_container stream_string(const std::string& sentence) {
        std::shared_ptr<std::stringstream> sstrm = std::make_shared<std::stringstream>();
        (*sstrm) << sentence;
        std::shared_ptr<std::istream> strm = sstrm;
        std::shared_ptr<container> con = std::make_shared<container_ptr>(strm);
        return istream_container(con);
    }

    bool is_good() const {
        return stream_container != nullptr && stream_container->is_good();
    }

    istream_container& operator=(std::istream& stream) {
        stream_container = std::make_shared<container_ref>(stream);
        return *this;
    }

    istream_container& operator=(const istream_container& stream) {
        stream_container = stream.stream_container;
        return *this;
    }

    std::istream* operator->() const {
        return &stream_container->get_stream();
    }
};

#endif //MANTA_ISTREAM_CONTAINER_H
