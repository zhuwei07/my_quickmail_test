#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define NOCURL

#include "quickmail.h"
#include <stdio.h>
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/timeb.h>

//#define FROM        "aldenzhu0908@126.com"
#define FROM        "aldenzhu0908@aliyun.com"
#define TO          "aldenzhu0908@gmail.com"
//#define TO          "1922243344@qq.com"
//#define CC          "otheruser@domain.com"
//#define BCC         "otheruser@domain.com"
//#define SMTPSERVER  "smtp.126.com"
#define SMTPSERVER  "smtp.aliyun.com"
#define SMTPPORT    25
#define SMTPUSER    "aldenzhu0908@aliyun.com"
//#define SMTPPASS    "DCXZKJUTOJSEHHQQ"
//#define SMTPPASS    "YCNZSANQWPVDKJTG"
#define SMTPPASS    "Zhuwei.930908"

void list_attachment_callback(quickmail mailobj, const char* filename, quickmail_attachment_open_fn email_info_attachment_open, quickmail_attachment_read_fn email_info_attachment_read, quickmail_attachment_close_fn email_info_attachment_close, void* callbackdata)
{
    printf("[%i]: %s\n", ++ * (int*)callbackdata, filename);
}

int main()
{
#if defined(NOCURL) && defined(_WIN32)
    //initialize winsock
    static WSADATA wsaData;
    int wsaerr = WSAStartup(MAKEWORD(1, 0), &wsaData);
    if (wsaerr)
        exit(1);
    atexit((void(*)())WSACleanup);
#endif

    printf("libquickmail %s\n", quickmail_get_version());
    quickmail_initialize();
    quickmail mailobj = quickmail_create(FROM, "libquickmail test e-mail");
#ifdef TO
    quickmail_add_to(mailobj, TO);
#endif
#ifdef CC
    quickmail_add_cc(mailobj, CC);
#endif
#ifdef BCC
    quickmail_add_bcc(mailobj, BCC);
#endif
    /*quickmail_add_header(mailobj, "Importance: Low");
    quickmail_add_header(mailobj, "X-Priority: 5");
    quickmail_add_header(mailobj, "X-MSMail-Priority: Low");*/

    char message_id[256];
    struct timeb tb = { 0 };
    ftime(&tb);
    snprintf(message_id, sizeof(message_id), "Message-ID: <%lld@%s>", (unsigned long long)tb.time * 1000LL + (unsigned long long)tb.millitm, SMTPSERVER);
    quickmail_add_header(mailobj, message_id);

    quickmail_set_subject(mailobj, "QDP Login Verification Code");

    char* body = "Hi,\nThanks for signing up to QDP Apex.\nPlease use the code below to verify your email address.\n123456\nYour friends at QDP";
    quickmail_add_body_memory(mailobj, "text/plain", body, strlen(body), 0);
    //quickmail_add_body_memory(mailobj, NULL, "This is a test e-mail.\nThis mail was sent using libquickmail.", 64, 0);
    //quickmail_add_body_memory(mailobj, "text/html", "This is a <b>test</b> e-mail.<br/>\nThis mail was sent using <u>libquickmail</u>.", 80, 0);
    /**/
    //quickmail_add_attachment_file(mailobj, "test_quickmail.c", NULL);
    //quickmail_add_attachment_file(mailobj, "test_quickmail.cbp", NULL);
    //quickmail_add_attachment_memory(mailobj, "test.log", NULL, "Test\n123", 8, 0);
    /**/
    /*/
      quickmail_fsave(mailobj, stdout);

      int i;
      i = 0;
      quickmail_list_attachments(mailobj, list_attachment_callback, &i);

      quickmail_remove_attachment(mailobj, "test_quickmail.cbp");
      i = 0;
      quickmail_list_attachments(mailobj, list_attachment_callback, &i);

      quickmail_destroy(mailobj);
      return 0;
    /**/

    const char* errmsg;
    quickmail_set_debug_log(mailobj, stderr);
    if ((errmsg = quickmail_send(mailobj, SMTPSERVER, SMTPPORT, SMTPUSER, SMTPPASS)) != NULL)
        fprintf(stderr, "Error sending e-mail: %s\n", errmsg);
    quickmail_destroy(mailobj);
    quickmail_cleanup();
    return 0;
}
