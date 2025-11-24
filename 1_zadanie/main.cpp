#include <UnitTest++/UnitTest++.h>
#include "modAlphaCipher.h"

SUITE(KeyTest)
{
    TEST(ValidKey) {
        CHECK_EQUAL("АГААГ",modAlphaCipher("АГА").encrypt("ААААА"));
    }
    TEST(LongKey) {
        CHECK_EQUAL("АГАПО",modAlphaCipher("АГАПОВ").encrypt("ААААА"));
    }
    TEST(LowCaseKey) {
        CHECK_EQUAL("АГААГ",modAlphaCipher("ага").encrypt("ААААА"));
    }
    TEST(DigitsInKey) {
        CHECK_THROW(modAlphaCipher cp("Г1"),cipher_error);
    }
    TEST(PunctuationInKey) {
        CHECK_THROW(modAlphaCipher cp("А,Г"),cipher_error);
    }
    TEST(WhitespaceInKey) {
        CHECK_THROW(modAlphaCipher cp("А Г П"),cipher_error);
    }
    TEST(EmptyKey) {
        CHECK_THROW(modAlphaCipher cp(""),cipher_error);
    }
    TEST(WeakKey) {
        CHECK_THROW(modAlphaCipher cp("ААА"),cipher_error);
    }
}

struct KeyB_fixture {
    modAlphaCipher * p;
    KeyB_fixture()
    {
        p = new modAlphaCipher("Г");
    }
    ~KeyB_fixture()
    {
        delete p;
    }
};

SUITE(EncryptTest)
{
    TEST_FIXTURE(KeyB_fixture, UpCaseString) {
        CHECK_EQUAL("ТЗОЯПЗРЯ", p->encrypt("ПЕЛЬМЕНЬ"));
    }
    TEST_FIXTURE(KeyB_fixture, LowCaseString) {
        CHECK_EQUAL("ТЗОЯПЗРЯ", p->encrypt("пельмень"));
    }
    TEST_FIXTURE(KeyB_fixture, StringWithWhitspaceAndPunct) {
        CHECK_EQUAL("ТЗОЯПЗРЯ", p->encrypt("пель мень!"));
    }
    TEST_FIXTURE(KeyB_fixture, StringWithNumbers) {
        CHECK_EQUAL("ТЗОЯПЗРЯ", p->encrypt("52ПЕЛЬМЕНЬ"));
    }
    TEST_FIXTURE(KeyB_fixture, EmptyString) {
        CHECK_THROW(p->encrypt(""), cipher_error);
    }
    TEST_FIXTURE(KeyB_fixture, NoAlphaString) {
        CHECK_THROW(p->encrypt("6*6=36?"), cipher_error);
    }
    TEST(MaxShiftKey) {
        CHECK_EQUAL("ОДКЫЛДМЫ", modAlphaCipher("Я").encrypt("ПЕЛЬМЕНЬ"));
    }
}

SUITE(DecryptText)
{
    TEST_FIXTURE(KeyB_fixture, UpCaseString) {
        CHECK_EQUAL("НЦЖЛРФИОСПЦЙФ", p->decrypt("РЩЙОУЧЛСФТЩМЧ"));
    }
    TEST_FIXTURE(KeyB_fixture, LowCaseString) {
        CHECK_THROW(p->decrypt("рщйОУЧЛСФТЩМЧ"), cipher_error);
    }
    TEST_FIXTURE(KeyB_fixture, WhitespaceString) {
        CHECK_THROW(p->decrypt("РЩЙОУ ЧЛСФТЩМЧ"), cipher_error);
    }
    TEST_FIXTURE(KeyB_fixture, DigitsString) {
        CHECK_THROW(p->decrypt("55РЩЙОУЧЛСФТЩМЧ"), cipher_error);
    }
    TEST_FIXTURE(KeyB_fixture, PunctString) {
        CHECK_THROW(p->decrypt("РЩЙОУЧЛСФТЩМЧ!"), cipher_error);
    }
    TEST_FIXTURE(KeyB_fixture, EmptyString) {
        CHECK_THROW(p->decrypt(""), cipher_error);
    }
    TEST(MaxShiftKey) {
        CHECK_EQUAL("ОФЖОБГЁРЫЧСЛБ", modAlphaCipher("Я").decrypt("НУЁНАВЕПЪЦРКА"));
    }
}

int main(int argc, char **argv)
{
    return UnitTest::RunAllTests();
}