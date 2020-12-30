

#include <ripple/basics/Buffer.h>
#include <ripple/beast/unit_test.h>
#include <cstdint>
#include <type_traits>

namespace ripple {
namespace test {

struct Buffer_test : beast::unit_test::suite
{
    bool sane (Buffer const& b) const
    {
        if (b.size() == 0)
            return b.data() == nullptr;

        return b.data() != nullptr;
    }

    void run() override
    {
        std::uint8_t const data[] =
        {
            0xa8, 0xa1, 0x38, 0x45, 0x23, 0xec, 0xe4, 0x23,
            0x71, 0x6d, 0x2a, 0x18, 0xb4, 0x70, 0xcb, 0xf5,
            0xac, 0x2d, 0x89, 0x4d, 0x19, 0x9c, 0xf0, 0x2c,
            0x15, 0xd1, 0xf9, 0x9b, 0x66, 0xd2, 0x30, 0xd3
        };

        Buffer b0;
        BEAST_EXPECT (sane (b0));
        BEAST_EXPECT (b0.empty());

        Buffer b1 { 0 };
        BEAST_EXPECT (sane (b1));
        BEAST_EXPECT (b1.empty());
        std::memcpy(b1.alloc(16), data, 16);
        BEAST_EXPECT (sane (b1));
        BEAST_EXPECT (!b1.empty());
        BEAST_EXPECT (b1.size() == 16);

        Buffer b2 { b1.size() };
        BEAST_EXPECT (sane (b2));
        BEAST_EXPECT (!b2.empty());
        BEAST_EXPECT (b2.size() == b1.size());
        std::memcpy(b2.data(), data + 16, 16);

        Buffer b3 { data, sizeof(data) };
        BEAST_EXPECT (sane (b3));
        BEAST_EXPECT (!b3.empty());
        BEAST_EXPECT (b3.size() == sizeof(data));
        BEAST_EXPECT (std::memcmp (b3.data(), data, b3.size()) == 0);

        BEAST_EXPECT (b0 == b0);
        BEAST_EXPECT (b0 != b1);
        BEAST_EXPECT (b1 == b1);
        BEAST_EXPECT (b1 != b2);
        BEAST_EXPECT (b2 != b3);

        {
            testcase ("Copy Construction / Assignment");

            Buffer x{ b0 };
            BEAST_EXPECT (x == b0);
            BEAST_EXPECT (sane (x));
            Buffer y{ b1 };
            BEAST_EXPECT (y == b1);
            BEAST_EXPECT (sane (y));
            x = b2;
            BEAST_EXPECT (x == b2);
            BEAST_EXPECT (sane (x));
            x = y;
            BEAST_EXPECT (x == y);
            BEAST_EXPECT (sane (x));
            y = b3;
            BEAST_EXPECT (y == b3);
            BEAST_EXPECT (sane (y));
            x = b0;
            BEAST_EXPECT (x == b0);
            BEAST_EXPECT (sane (x));
#if defined(__clang__) && \
    (!defined(__APPLE__) && (__clang_major__ >= 7)) || \
    (defined(__APPLE__) && (__apple_build_version__ >= 10010043))
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-assign-overloaded"
#endif

            x = x;
            BEAST_EXPECT (x == b0);
            BEAST_EXPECT (sane (x));
            y = y;
            BEAST_EXPECT (y == b3);
            BEAST_EXPECT (sane (y));

#if defined(__clang__) && \
    (!defined(__APPLE__) && (__clang_major__ >= 7)) || \
    (defined(__APPLE__) && (__apple_build_version__ >= 10010043))
#pragma clang diagnostic pop
#endif
        }

        {
            testcase ("Move Construction / Assignment");

            static_assert(std::is_nothrow_move_constructible<Buffer>::value, "");
            static_assert(std::is_nothrow_move_assignable<Buffer>::value, "");

            { 
                Buffer x;
                Buffer y { std::move(x) };
                BEAST_EXPECT (sane(x));
                BEAST_EXPECT (x.empty());
                BEAST_EXPECT (sane(y));
                BEAST_EXPECT (y.empty());
                BEAST_EXPECT (x == y);
            }

            { 
                Buffer x { b1 };
                Buffer y { std::move(x) };
                BEAST_EXPECT (sane(x));
                BEAST_EXPECT (x.empty());
                BEAST_EXPECT (sane(y));
                BEAST_EXPECT (y == b1);
            }

            { 
                Buffer x;
                Buffer y;

                x = std::move(y);
                BEAST_EXPECT (sane(x));
                BEAST_EXPECT (x.empty());
                BEAST_EXPECT (sane(y));
                BEAST_EXPECT (y.empty());
            }

            { 
                Buffer x;
                Buffer y { b1 };

                x = std::move(y);
                BEAST_EXPECT (sane(x));
                BEAST_EXPECT (x == b1);
                BEAST_EXPECT (sane(y));
                BEAST_EXPECT (y.empty());
            }

            { 
                Buffer x { b1 };
                Buffer y;

                x = std::move(y);
                BEAST_EXPECT (sane(x));
                BEAST_EXPECT (x.empty());
                BEAST_EXPECT (sane(y));
                BEAST_EXPECT (y.empty());
            }

            { 
                Buffer x { b1 };
                Buffer y { b2 };
                Buffer z { b3 };

                x = std::move(y);
                BEAST_EXPECT (sane(x));
                BEAST_EXPECT (!x.empty());
                BEAST_EXPECT (sane(y));
                BEAST_EXPECT (y.empty());

                x = std::move(z);
                BEAST_EXPECT (sane(x));
                BEAST_EXPECT (!x.empty());
                BEAST_EXPECT (sane(z));
                BEAST_EXPECT (z.empty());
            }
        }

        {
            testcase ("Slice Conversion / Construction / Assignment");

            Buffer w { static_cast<Slice>(b0) };
            BEAST_EXPECT(sane(w));
            BEAST_EXPECT(w == b0);

            Buffer x { static_cast<Slice>(b1) };
            BEAST_EXPECT(sane(x));
            BEAST_EXPECT(x == b1);

            Buffer y { static_cast<Slice>(b2) };
            BEAST_EXPECT(sane(y));
            BEAST_EXPECT(y == b2);

            Buffer z { static_cast<Slice>(b3) };
            BEAST_EXPECT(sane(z));
            BEAST_EXPECT(z == b3);

            w = static_cast<Slice>(b0);
            BEAST_EXPECT(sane(w));
            BEAST_EXPECT(w == b0);

            w = static_cast<Slice>(b1);
            BEAST_EXPECT(sane(w));
            BEAST_EXPECT(w == b1);

            x = static_cast<Slice>(b2);
            BEAST_EXPECT(sane(x));
            BEAST_EXPECT(x == b2);

            y = static_cast<Slice>(z);
            BEAST_EXPECT(sane(y));
            BEAST_EXPECT(y == z);

            z = static_cast<Slice>(b0);
            BEAST_EXPECT(sane(z));
            BEAST_EXPECT (z == b0);
        }

        {
            testcase ("Allocation, Deallocation and Clearing");

            auto test = [this](Buffer const& b, std::size_t i)
            {
                Buffer x{b};

                x(i);
                BEAST_EXPECT (sane(x));
                BEAST_EXPECT (x.size() == i);
                BEAST_EXPECT ((x.data() == nullptr) == (i == 0));

                x(i + 1);
                BEAST_EXPECT (sane(x));
                BEAST_EXPECT (x.size() == i + 1);
                BEAST_EXPECT (x.data() != nullptr);

                x.clear();
                BEAST_EXPECT (sane(x));
                BEAST_EXPECT (x.size() == 0);
                BEAST_EXPECT (x.data() == nullptr);

                x.clear();
                BEAST_EXPECT (sane(x));
                BEAST_EXPECT (x.size() == 0);
                BEAST_EXPECT (x.data() == nullptr);
            };

            for (std::size_t i = 0; i < 16; ++i)
            {
                test (b0, i);
                test (b1, i);
            }
        }
    }
};

BEAST_DEFINE_TESTSUITE(Buffer, ripple_basics, ripple);

}  
}  






