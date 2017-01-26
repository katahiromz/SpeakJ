// SpeakJ.cpp --- Japanese voice synthesis by katahiromz
// This file is public domain software (PDS).
//////////////////////////////////////////////////////////////////////////////

#if (__cplusplus <  201103L)
    #include "unboost/thread.hpp"
    #define SJ_Thread   unboost::thread
    #include "unboost/mutex.hpp"
    #define SJ_Mutex    unboost::mutex
#endif

#include "SpeakJ.hpp"

#ifdef SPEAKJ_UNITTEST
int main(void)
{
    #if 1
        SpeakJ speak_j(SJ_TEXT("sounds"));
    #else
        SpeakJ speak_j(GetModuleHandle(NULL));
    #endif
    //speak_j.PlayText(SJ_TEXT("われわれわ、コンピューターだ。"));
    //speak_j.PlayText(SJ_TEXT("あいうえおかきくけこさしすせそたちつてと"));
    //speak_j.PlayText(SJ_TEXT("なにぬねのはひふへほまみむめもやゆよらりるれろわをん。"));
    //speak_j.PlayText(SJ_TEXT("がぎぐげござじずぜぞだぢづでどばびぶべぼぱぴぷぺぽ、"));
    //speak_j.PlayText(SJ_TEXT("きゃきゅきょ、しゃしゅしょ、ちゃちゅちょ、"));
    //speak_j.PlayText(SJ_TEXT("にゃにゅにょ、ひゃひゅひょ、みゃみゅみょ、"));
    //speak_j.PlayText(SJ_TEXT("りゃりゅりょ、ぎゃぎゅぎょ、じゃじゅじょ、"));
    //speak_j.PlayText(SJ_TEXT("ぢゃぢゅぢょ、びゃびゅびょ、ぴゃぴゅぴょ。"));
    //speak_j.PlayText(SJ_TEXT("てぃでぃでゅつぃ。"));

    speak_j.PlayText(SJ_TEXT("びっくりしたなあ、もう。"));
    //speak_j.PlayText(SJ_TEXT("ことばの、すべてを、こんぴゅーたーえ"));
    //speak_j.PlayText(SJ_TEXT("とうきょうとっきょきょかきょくきょくちょう。"));

    //speak_j.PlayText(SJ_TEXT("あっいっうっえっおっかっきっくっけっこっさっしっすっせっそったっちっつってっとっ"));
    //speak_j.PlayText(SJ_TEXT("なっにっぬっねっのっはっひっふっへっほっまっみっむっめっもっやっゆっよっらっりっるっれっろっわっをっんっ。"));
    //speak_j.PlayText(SJ_TEXT("がっぎっぐっげっごっざっじっずっぜっぞっだっぢっづっでっどっばっびっぶっべっぼっぱっぴっぷっぺっぽっ、"));
    //speak_j.PlayText(SJ_TEXT("きゃっきゅっきょっ、しゃっしゅっしょっ、ちゃっちゅっちょっ、"));
    //speak_j.PlayText(SJ_TEXT("にゃっにゅっにょっ、ひゃっひゅっひょっ、みゃっみゅっみょっ、"));
    //speak_j.PlayText(SJ_TEXT("りゃっりゅっりょっ、ぎゃっぎゅっぎょっ、じゃっじゅっじょっ、"));
    //speak_j.PlayText(SJ_TEXT("ぢゃっぢゅっぢょっ、びゃっびゅっびょっ、ぴゃっぴゅっぴょっ。"));
    //speak_j.PlayText(SJ_TEXT("てぃっでぃっでゅっつぃっ。"));

    return 0;
}
#endif  // def SPEAKJ_UNITTEST
