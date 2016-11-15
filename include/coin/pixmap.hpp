#pragma once

#include <cstdint>

#include <vector>
#include <array>
#include <unordered_map>
#include <type_traits>  
#include <utility>
#include <string>
#include <fstream>
#include <functional>
#include <iterator>
#include <numeric>

namespace pixmap {

    enum class Anymap : std::uint8_t { PBM, PGM, PPM };
    enum class Format : bool         { BIN, ASCII };

} // namespace pixmap

namespace std {
    // specify hash for enum classes
    template<> struct hash<pixmap::Anymap> {
        std::size_t operator () (pixmap::Anymap x) const {
            using type = std::underlying_type<pixmap::Anymap>::type;
            return std::hash<type>()(static_cast<type>(x));
        }
    };
    template<> struct hash<pixmap::Format> {
        std::size_t operator () (pixmap::Format x) const {
            using type = std::underlying_type<pixmap::Format>::type;
            return std::hash<type>()(static_cast<type>(x));
        }
    };

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
        using namespace std;
        std::copy(vec.begin(), vec.end(), std::ostream_iterator<T>(os, " "));
        return os;
    }
    template<typename T>
    std::istream& operator>>(std::istream& is, std::vector<T>& vec) {
        using namespace std;
        std::copy(std::istream_iterator<T>(is), std::istream_iterator<T>(), std::back_inserter(vec));
        return is;
    }
} // namespace std

namespace pixmap {

namespace impl_pixmap_ {


    // hash for std::pair
    struct pair_hash {
    public:
        template <typename T, typename U>
        std::size_t operator()(const std::pair<T, U> &x) const {
            return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
        }
    };

    static std::unordered_map<std::pair<Anymap, Format>, std::uint8_t, pair_hash> map_magic_num{
        { { Anymap::PBM, Format::ASCII   }, 1 }, { { Anymap::PGM, Format::ASCII   }, 2 }, { { Anymap::PPM, Format::ASCII   }, 3 },
        { { Anymap::PBM, Format::BIN }, 4 }, { { Anymap::PGM, Format::BIN }, 5 }, { { Anymap::PPM, Format::BIN }, 6 }
    };

    static std::array<std::pair<Anymap, Format>, 6> map_magic_num_reverse{ {
        { Anymap::PBM, Format::BIN   }, { Anymap::PGM, Format::BIN   }, { Anymap::PPM, Format::BIN   },
        { Anymap::PBM, Format::ASCII }, { Anymap::PGM, Format::ASCII }, { Anymap::PPM, Format::ASCII }
        } 
    };

    // Binary files and comments are not handled here
    template<Anymap P, Format F = Format::ASCII>
    class Image {
    public:
        using data_storage = std::conditional_t < P == Anymap::PBM, std::vector<bool>, std::vector<std::uint16_t> > ;
        using value_type = typename data_storage::value_type;
        using reference = typename data_storage::reference;
        using pointer = typename data_storage::pointer;
        using const_reference = typename data_storage::const_reference;
        using size_type = typename data_storage::size_type;
        using iterator = typename data_storage::iterator;
        using const_iterator = typename data_storage::const_iterator;
        using pixel = std::conditional_t < P == Anymap::PPM, std::array<value_type, 3>, reference> ;

        Image() = default;
        explicit Image(size_type size) : Image(size, size) {}
        Image(size_type width, size_type height) : Image(data_storage(width * height * (P==Anymap::PPM ? 3 : 1)), width, height) {}
        Image(const data_storage& data, size_type size) : Image(data, size, size) {} // use delegate constructor
        Image(const data_storage& data, size_type width, size_type height, value_type max_val = UINT8_MAX) // 255
            : magic_number_(std::string{ "P" + std::to_string(map_magic_num[{P, F}]) })
            , width_{ width }
            , height_{ height }
            , max_val_{ max_val }
            , data_(data)
        {}

        size_type height() const { return height_; }
        size_type width() const { return width_;  }

        reference       operator[] (size_type index)        { return data_[index]; }
        const_reference operator[] (size_type index) const  { return data_[index]; }
        // template<Anymap Q = P>
        // std::enable_if_t<Q == Anymap::PPM, pixel>
        //  operator[] (size_type index)  { return pixel {{ data_[index], data_[index + 1], data_[index + 2]}}; }

        // template<Anymap Q = P>
        // std::enable_if_t<!(Q == Anymap::PPM), pixel>
        // operator[] (size_type index)  { return data_[index]; }

        // const pixel operator[] (size_type index) const {
        //  return const_cast<pixel>(static_cast<const Image&>(*this)(index));
        // };

        template<Anymap Q = P>
        std::enable_if_t<Q == Anymap::PPM, pixel>
        operator() (size_type row, size_type col)  {
            size_t idx{ row * width_ + col }; 
            return std::array <value_type, 3 > {{data_[idx], data_[idx + 1], data_[idx + 2]}};
        }

        template<Anymap Q = P>
        std::enable_if_t<!(Q == Anymap::PPM), pixel>
        operator() (size_type row, size_type col)  { 
            return data_[row * width_ + col]; 
        }

        const pixel operator() (size_type row, size_type col) const {
            return const_cast<pixel>(static_cast<const Image&>(*this)(row,col));
        };

        iterator begin()              { return data_.begin();  }
        const_iterator begin()  const { return data_.begin();  }
        const_iterator cbegin() const { return data_.cbegin(); }
        iterator end()                { return data_.end();    }
        const_iterator end()    const { return data_.end();    }
        const_iterator cend()   const { return data_.cend();   }

        void fill(data_storage& data) { std::copy(data.begin(), data.end(), data_.begin()); }

        void save(const std::string& filename) {
            std::ofstream ofs;
            ofs.open(filename, std::ios::out);
            ofs << (*this);
            ofs.close();
        }

        pointer data() { return data_.data(); }

    private:
        template<Anymap T1, Format T2> friend std::ostream& operator<<(std::ostream&, const Image<T1, T2>&);
        template<Anymap T1, Format T2> friend std::istream& operator>>(std::istream&, Image<T1, T2>&);

        std::string   magic_number_;
        size_type     width_;
        size_type     height_;
        std::uint16_t max_val_;  // 0 <= max_val_ < 65536
        data_storage  data_;
    };

    template <Anymap P, Format F>
    void load(const std::string& filename, Image<P, F>& img) {
        std::ifstream ifs;
        ifs.open(filename, std::ios::in);
        ifs >> img;
        ifs.close();
    }
    
    template<Anymap P, Format F>
    std::ostream& operator<<(std::ostream& os, const Image<P, F>& img) {
        return  os << img.magic_number_ << "\n"
            << img.width_ << " " << img.height_ << "\n"
            << img.max_val_ << "\n" << img.data_;
    }

    template<Anymap P, Format F>
    std::istream& operator>>(std::istream& is, Image<P, F>& img) {
        return is >> img.magic_number_ >> img.width_ >> img.height_ >> img.max_val_ >> img.data_;
    }

    template <Anymap P, Format F>
    void save(const std::string& filename, Image<P, F>& img) {
        img.save(filename);
    }

} // namespace impl_pixmap_

using impl_pixmap_::Image;
using impl_pixmap_::save;


template <Anymap P, Format F=Format::ASCII, typename...ARGs>
Image<P,F> create(ARGs&&...args) {
    return Image<P,F>(std::forward<ARGs>(args)...);
}

template <Anymap P, Format F=Format::ASCII>
Image<P, F> open(const std::string& filename) {
    Image<P, F> img;
    impl_pixmap_::load(filename, img);
    return img;
}

} // end namespace pixmap


