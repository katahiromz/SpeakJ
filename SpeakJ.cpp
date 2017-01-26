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
    //speak_j.PlayText(SJ_TEXT("������A�R���s���[�^�[���B"));
    //speak_j.PlayText(SJ_TEXT("�����������������������������������Ă�"));
    //speak_j.PlayText(SJ_TEXT("�Ȃɂʂ˂̂͂Ђӂւق܂݂ނ߂��������������B"));
    //speak_j.PlayText(SJ_TEXT("�������������������������Âłǂ΂тԂׂڂς҂Ղ؂ہA"));
    //speak_j.PlayText(SJ_TEXT("���Ⴋ�カ��A���Ⴕ�サ��A���Ⴟ�タ��A"));
    //speak_j.PlayText(SJ_TEXT("�ɂ�ɂ�ɂ�A�Ђ�Ђ�Ђ�A�݂�݂�݂�A"));
    //speak_j.PlayText(SJ_TEXT("�������A���Ⴌ�ガ��A���Ⴖ�ザ��A"));
    //speak_j.PlayText(SJ_TEXT("����������A�т�т�т�A�҂�҂�҂�B"));
    //speak_j.PlayText(SJ_TEXT("�Ă��ł��ł���B"));

    speak_j.PlayText(SJ_TEXT("�т����肵���Ȃ��A�����B"));
    //speak_j.PlayText(SJ_TEXT("���Ƃ΂́A���ׂĂ��A����҂�[���[��"));
    //speak_j.PlayText(SJ_TEXT("�Ƃ����傤�Ƃ����傫�傩���傭���傭���傤�B"));

    //speak_j.PlayText(SJ_TEXT("�����������������������������������������������������������������������Ă��Ƃ�"));
    //speak_j.PlayText(SJ_TEXT("�Ȃ��ɂ��ʂ��˂��̂��͂��Ђ��ӂ��ւ��ق��܂��݂��ނ��߂����������������������������������������B"));
    //speak_j.PlayText(SJ_TEXT("�������������������������������������������������Â��ł��ǂ��΂��т��Ԃ��ׂ��ڂ��ς��҂��Ղ��؂��ۂ��A"));
    //speak_j.PlayText(SJ_TEXT("����������������A����������������A����������������A"));
    //speak_j.PlayText(SJ_TEXT("�ɂ���ɂ���ɂ���A�Ђ���Ђ���Ђ���A�݂���݂���݂���A"));
    //speak_j.PlayText(SJ_TEXT("�������������A����������������A����������������A"));
    //speak_j.PlayText(SJ_TEXT("����������������A�т���т���т���A�҂���҂���҂���B"));
    //speak_j.PlayText(SJ_TEXT("�Ă����ł����ł�������B"));

    return 0;
}
#endif  // def SPEAKJ_UNITTEST
