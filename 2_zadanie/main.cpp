#include <UnitTest++/UnitTest++.h>
#include "code.h"
#include <string>

std::string s = "BREAKING";

SUITE(KeyTest) {
    TEST(ValidKey) {
        code cipher(4, "BREAKING");
        CHECK_EQUAL("AGENRIBK", cipher.encryption(s));
    }
    TEST(LongKey) {
        CHECK_THROW(code(1, "BREAKING"), cipher_error);
    }
}

struct KeyB_fixture {
    code * t;
    KeyB_fixture() {
        t = new code(4, "BREAKING");
    }
    ~KeyB_fixture() {
        delete t;
    }
};

SUITE(EncryptTest) {
    TEST_FIXTURE(KeyB_fixture, UpCaseString) {
        std::string input = "BREAKING";
        CHECK_EQUAL("AGENRIBK", t->encryption(input));
    }
    TEST_FIXTURE(KeyB_fixture, LowCaseString) {
        std::string input = "breaking";
        CHECK_EQUAL("agenribk", t->encryption(input));
    }
    TEST_FIXTURE(KeyB_fixture, StringWithWhitspaceAndPunct) {
        std::string input = "BREA KING";
        CHECK_EQUAL("AGENRIBK", t->encryption(input));
    }
    TEST_FIXTURE(KeyB_fixture, StringWithNumbers) {
        std::string input = "BREAK4NG";
        CHECK_THROW(t->encryption(input), cipher_error);
    }
    TEST_FIXTURE(KeyB_fixture, EmptyString) {
        std::string input = "";
        CHECK_THROW(t->encryption(input), cipher_error);
    }
    TEST_FIXTURE(KeyB_fixture, NoAlphaString) {
        std::string input = "3*3=9?";
        CHECK_THROW(t->encryption(input), cipher_error);
    }
    TEST(MaxShiftKey) {
        code cipher(8, "BREAKING");
        std::string input1 = "AGENRIBK";
        std::string input2 = "BREAKING";
        CHECK_EQUAL("KBIRNEGA", cipher.transcript(input1, input2));
    }
}

SUITE(DecryptText) {
    TEST_FIXTURE(KeyB_fixture, UpCaseString) {
        std::string input1 = "AGENRIBK";
        std::string input2 = "BREAKING";
        CHECK_EQUAL("BREAKING", t->transcript(input1, input2));
    }
    TEST_FIXTURE(KeyB_fixture, LowCaseString) {
        std::string input1 = "agenribk";
        std::string input2 = "breaking";
        CHECK_EQUAL("breaking", t->transcript(input1, input2));
    }
    TEST_FIXTURE(KeyB_fixture, WhitespaceString) {
        std::string input1 = "BREAKING";
        std::string input2 = "BREA KING";
        CHECK_THROW(t->transcript(input1, input2), cipher_error);
    }
    TEST_FIXTURE(KeyB_fixture, DigitsString) {
        std::string input1 = "BREAK4NG";
        std::string input2 = "BREAK4NG";
        CHECK_THROW(t->transcript(input1, input2), cipher_error);
    }
    TEST_FIXTURE(KeyB_fixture, PunctString) {
        std::string input1 = "BREAKING!";
        std::string input2 = "BREAKING!";
        CHECK_THROW(t->transcript(input1, input2), cipher_error);
    }
    TEST_FIXTURE(KeyB_fixture, EmptyString) {
        std::string input1 = "";
        std::string input2 = "";
        CHECK_THROW(t->transcript(input1, input2), cipher_error);
    }
    TEST(MaxShiftKey) {
        code cipher(8, "Serafime");
        std::string input1 = "AGENRIBK";
        std::string input2 = "BREAKING";
        CHECK_EQUAL("KBIRNEGA", cipher.transcript(input1, input2));
    }
}

int main(int argc, char **argv) {
    return UnitTest::RunAllTests();
}