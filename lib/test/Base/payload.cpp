#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <Base/Event/Payload.hpp>
#include <doctest/doctest.h>

using namespace mentor;

struct pod
{
    int a, b;
};

struct npod
{
    std::string s;
    std::vector<int> v;
};

int del_counter1, del_counter2;

void deleter1(void *x)
{
    ++del_counter1;
    delete static_cast<pod *>(x);
}

void deleter2(void *x)
{
    ++del_counter2;
    delete static_cast<npod *>(x);
}

TEST_CASE("testing base payload")
{
    SUBCASE("view")
    {
        pod a;
        a.a = 1;
        a.b = 2;
        npod b;
        b.s = "1";
        b.v = {1};
        Payload view4;
        Payload view5;
        Payload view6;
        {
            Payload view1 = &a;
            Payload view2 = &b;
            Payload view3 = nullptr;
            CHECK(view1.get<pod *>()->a == a.a);
            CHECK(view1.get<pod *>()->b == a.b);
            CHECK(view2.get<npod *>()->s == b.s);
            CHECK(view2.get<npod *>()->v == b.v);
            CHECK(!view3.get<pod *>());
            view4 = std::move(view1);
            view5 = std::move(view2);
            view6 = std::move(view3);
        }
        CHECK(view4.get<pod *>()->a == a.a);
        CHECK(view4.get<pod *>()->b == a.b);
        CHECK(view5.get<npod *>()->s == b.s);
        CHECK(view5.get<npod *>()->v == b.v);
        CHECK(!view6.get<pod *>());
    }

    SUBCASE("owner")
    {
        {
            Payload *p1, *p2;
            Payload owner3, owner4;
            p1 = &owner3;
            p2 = &owner4;
            CHECK(!p1->get<pod *>());
            CHECK(!p2->get<npod *>());
            {
                Payload owner1 = Payload::makeOwned<pod>(1, 2, deleter1);
                Payload owner2 = Payload::makeOwned<npod>("1", std::vector<int>{1}, deleter2);
                CHECK(del_counter1 == 0);
                CHECK(del_counter2 == 0);
                CHECK(owner1.get<pod *>()->a == 1);
                CHECK(owner1.get<pod *>()->b == 2);
                CHECK(owner2.get<npod *>()->s == "1");
                CHECK(owner2.get<npod *>()->v == std::vector<int>{1});
                owner3 = std::move(owner1);
                owner4 = std::move(owner2);
            }
            CHECK(del_counter1 == 0);
            CHECK(del_counter2 == 0);
            CHECK(p1->get<pod *>());
            CHECK(p2->get<npod *>());
            CHECK(owner3.get<pod *>()->a == 1);
            CHECK(owner3.get<pod *>()->b == 2);
            CHECK(owner4.get<npod *>()->s == "1");
            CHECK(owner4.get<npod *>()->v == std::vector<int>{1});
        }
        CHECK(del_counter1 == 1);
        CHECK(del_counter2 == 1);
    }
}
