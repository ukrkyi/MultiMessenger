#ifndef PURPLEINTERACTION_H
#define PURPLEINTERACTION_H

#include <QDateTime>
#include <QMap>
#include <QObject>
#include <QStandardPaths>
#include <QTimerEvent>
#include <QDir>
#include <QImage>
#include <QImageReader>
#include <QThread>
#include <QAbstractEventDispatcher>

#ifdef QT_DEBUG
#include "QDebug"
#endif

#include "qpurple_global.h"

#include <connection.h>
#include <core.h>
#include <eventloop.h>

#define CUSTOM_USER_DIRECTORY "/dev/null"
//#define CUSTOM_PLUGIN_PATH     ""
#define PLUGIN_SAVE_PREF "/purple/qpurple/plugins/saved"
#define UI_ID "qpurple"

#define strconv(S) (S.toStdString().c_str())

// Forward Typedefs

// GLib
typedef void* gpointer;
typedef char gchar;
typedef int gint;
typedef gint gboolean;
typedef unsigned int guint;
typedef gboolean (*GSourceFunc)(gpointer user_data);
typedef struct _GHashTable GHashTable;
// Purple
typedef void (*PurpleInputFunction)(gpointer, gint, PurpleInputCondition);
typedef struct _PurpleEventLoopUiOps PurpleEventLoopUiOps;
typedef struct _PurpleConversationUiOps PurpleConversationUiOps;
typedef struct _PurpleAccount PurpleAccount;

class PurpleIONotifier;

class QPURPLESHARED_EXPORT PurpleInteraction : public QObject {
    Q_OBJECT

    static PurpleInteraction* instance;

    // Qt eventloop interaction

    typedef QPair<GSourceFunc, gpointer> gSrcFuncData;

    QMap<guint, gSrcFuncData> timerMap;
    QMap<guint, PurpleIONotifier*> ioNotMap;
    guint unique_key_last;

    static guint q_timer_add(guint interval, GSourceFunc function, gpointer data);
    static gboolean q_timer_remove(guint handle);
    static guint qt_input_add(gint fd, PurpleInputCondition condition, PurpleInputFunction function, gpointer data);
    static gboolean qt_input_remove(guint handle);

    void timerEvent(QTimerEvent*);

    static PurpleEventLoopUiOps qt_eventloops;

    // Libpurple init

    static PurpleCoreUiOps core_uiops;
    static PurpleConversationUiOps conv_uiops;

    static void prefs_init();
    static void ui_init();
    static void on_purple_quit();
#ifdef QT_DEBUG
    static void debug_init();
#endif

    void connect_to_signals();
    int init_libpurple();

    static void acc_connecting(PurpleAccount* acc);
    static void acc_signed_on(PurpleAccount* acc);
    static void acc_signed_off(PurpleAccount* acc);
    static void acc_disabled(PurpleAccount* acc);
    static void acc_enabled(PurpleAccount* acc);
    static void acc_error(PurpleAccount* acc, PurpleConnectionError err, const gchar* desc);

    static void cont_changed(PurpleBuddy* buddy);

    static void message_recieved(PurpleConversation* conv, const char*, const char*,
        const char* message,
        PurpleMessageFlags flags,
        time_t mtime);

public:
    explicit PurpleInteraction();
    PurpleInteraction(PurpleInteraction const&) = delete;
    void operator=(PurpleInteraction const&) = delete;
    static void setInstance(PurpleInteraction*);
    ~PurpleInteraction();

    typedef struct _AccountStruct {
        QString username;
        QString password;
        QString protocol;
        bool enabled;
    } AccountStruct;

    typedef struct _ContactStruct {
        QString name;
        QString alias;
        QImage avatar;
    } ContactStruct;

    typedef struct _MessageStruct {
        QString text;
        QDateTime time;
        enum MessageType { Incoming,
            Outcoming } type;
    } MessageStruct;

signals:
    void init(bool);

    void accountConnecting(AccountStruct);
    void accountConnected(AccountStruct);
    void accountDisconnected(AccountStruct);
    void accountConnectError(AccountStruct, QString);
    void accountDisabled(AccountStruct);
    void accountEnabled(AccountStruct);
    void contactsLoaded(AccountStruct, QList<ContactStruct>);
    void messageRecieved(AccountStruct, QString, MessageStruct);
    void contactChanged(AccountStruct, QString, ContactStruct);

    void finished();

public slots:
    void run();
    void addAccount(AccountStruct);
    void putOnline(AccountStruct);
    void putInvisible(AccountStruct);
    void changeEnabled(AccountStruct);
    void connectAccount(AccountStruct);
    void createConversation(AccountStruct, QString);
    void sendMessage(AccountStruct,QString,QString);
};

Q_DECLARE_METATYPE(PurpleInteraction::ContactStruct)

#endif // PURPLEINTERACTION_H
