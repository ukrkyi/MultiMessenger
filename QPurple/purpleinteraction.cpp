#include "purpleinteraction.h"

#include "purpleionotifier.h"

#include <glib.h>
#include <purple.h>

PurpleInteraction::PurpleInteraction() : QObject(0) {}

PurpleInteraction::~PurpleInteraction() {
    purple_core_quit();
}

PurpleEventLoopUiOps PurpleInteraction::qt_eventloops = {
    q_timer_add,
    q_timer_remove,
    qt_input_add,
    qt_input_remove,
    NULL,
    NULL, // таймер в секундах

    /* padding */
    NULL,
    NULL,
    NULL
};

PurpleCoreUiOps PurpleInteraction::core_uiops = {
    prefs_init,
#ifdef QT_DEBUG
    debug_init,
#else
    NULL,
#endif
    ui_init,
    on_purple_quit,
    NULL, // ui_get_info,

    /* padding */
    NULL,
    NULL,
    NULL
};

PurpleConversationUiOps PurpleInteraction::conv_uiops = {
    NULL, /* create_conversation  */
    NULL, /* destroy_conversation */
    NULL, /* write_chat           */
    NULL, /* write_im             */
    message_recieved, /* write_conv           */
    NULL, /* chat_add_users       */
    NULL, /* chat_rename_user     */
    NULL, /* chat_remove_users    */
    NULL, /* chat_update_user     */
    NULL, /* present              */
    NULL, /* has_focus            */
    NULL, /* send_confirm         */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

PurpleInteraction* PurpleInteraction::instance = NULL;

void PurpleInteraction::setInstance(PurpleInteraction* one) {
    if (!one || !instance) {
        instance = one;
    } else {
        throw "Instance already created!";
    }
}

guint PurpleInteraction::qt_input_add(gint fd, PurpleInputCondition condition, PurpleInputFunction function,
    gpointer data) {
    guint handle = ++instance->unique_key_last;

    instance->ioNotMap[handle] = new PurpleIONotifier(fd, condition, function, data);

    return handle;
}

gboolean PurpleInteraction::qt_input_remove(guint handle) {
    int retVal = 0;
    if (instance->ioNotMap.contains(handle)) {
        PurpleIONotifier* ex = instance->ioNotMap.take(handle);
        ex->deleteLater();
        retVal = 1;
    }
    return retVal;
}

guint PurpleInteraction::q_timer_add(guint interval, GSourceFunc function, gpointer data) {
    guint id = instance->startTimer(interval);
    instance->timerMap[id] = qMakePair(function, data);
    return id;
}

gboolean PurpleInteraction::q_timer_remove(guint handle) {
    return instance->timerMap.remove(handle);
}

void PurpleInteraction::timerEvent(QTimerEvent* e) {
    if (e && timerMap.contains(e->timerId())) {
        if (timerMap.contains(e->timerId())) {
            gSrcFuncData f = timerMap[e->timerId()];
            if (!((*f.first)(f.second)))
                killTimer(e->timerId());
        } else
            killTimer(e->timerId());
    }
}

void PurpleInteraction::prefs_init() {
    purple_prefs_add_none("/qpurple");
}

#ifdef QT_DEBUG
void PurpleInteraction::debug_init() {
    ;
}
#endif

void PurpleInteraction::ui_init() {
    // Ініціалізація UiOps
    purple_conversations_set_ui_ops(&conv_uiops);
}

int PurpleInteraction::init_libpurple() {
    /* Користувацькі теки */
    QDir icons_cache = QDir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation)).absoluteFilePath("icons");
    if (!icons_cache.exists())
        icons_cache.mkpath(".");
    purple_buddy_icons_set_cache_dir(strconv(icons_cache.absolutePath()));
    purple_util_set_user_dir(CUSTOM_USER_DIRECTORY);

#ifdef QT_DEBUG
    purple_debug_set_enabled(TRUE);
#else
    purple_debug_set_enabled(FALSE);
#endif

    /* Set the core-uiops */
    purple_core_set_ui_ops(&core_uiops);

    /* uiops для циклу подій qt */
    unique_key_last = 0;
    purple_eventloop_set_ui_ops(&qt_eventloops);

    /* Шлях пошуку розширень */
    // NOTE Напевно, не потрібно
    //purple_plugins_add_search_path(CUSTOM_PLUGIN_PATH);
    //purple_plugins_refresh();

    /* Запуск ядра */
    if (!purple_core_init(UI_ID)) {
        /* Initializing the core failed. Terminate. */
        return -1;
    }

    /* Load the preferences. */
    // Нічого не завантажуємо, все додамо вручну.
    purple_prefs_load();
    //purple_blist_load();
    //purple_pounces_load();

    if (!purple_get_blist())
        purple_set_blist(purple_blist_new());

    /* Load the desired plugins. The client should save the list of loaded plugins in
     * the preferences using purple_plugins_save_loaded(PLUGIN_SAVE_PREF) */
    //purple_plugins_load_saved(PLUGIN_SAVE_PREF);
    return 0;
}

void PurpleInteraction::acc_connecting(PurpleAccount* acc) {
    AccountStruct str;
    str.username = purple_account_get_username(acc);
    str.protocol = purple_account_get_protocol_id(acc);
    emit instance->accountConnecting(str);
}

void PurpleInteraction::acc_signed_on(PurpleAccount* acc) {
    AccountStruct str;
    str.username = purple_account_get_username(acc);
    str.protocol = purple_account_get_protocol_id(acc);
    emit instance->accountConnected(str);
    GSList* contact_list = purple_find_buddies(acc, NULL);
    QList<ContactStruct> contacts;
    for (GSList* it = contact_list; it; it = it->next) {
        ContactStruct str;
        PurpleBuddy* buddy = (PurpleBuddy*)it->data;
        if (buddy->alias && strlen(buddy->alias))
            str.alias = buddy->alias;
        else
            str.alias = buddy->server_alias;
        str.name = buddy->name;
        size_t img_size;
        if (purple_buddy_get_icon(buddy)) {
            const unsigned char* icon = (const unsigned char*)purple_buddy_icon_get_data(purple_buddy_get_icon(buddy), &img_size);
            if (icon) {
                purple_buddy_icon_ref(purple_buddy_get_icon(buddy));
                if (QImageReader::supportedImageFormats().contains(QByteArray(purple_buddy_icon_get_extension(purple_buddy_get_icon(buddy))).toUpper())) {
                    str.avatar = QImage::fromData(icon, img_size, purple_buddy_icon_get_extension(purple_buddy_get_icon(buddy)));
                } else {
                    str.avatar = QImage::fromData(icon, img_size);
                }
            }
        }
        contacts.push_back(str);
    }
    g_slist_free(contact_list);
    emit instance->contactsLoaded(str, contacts);
}

void PurpleInteraction::acc_signed_off(PurpleAccount* acc) {
    AccountStruct str;
    str.username = purple_account_get_username(acc);
    str.protocol = purple_account_get_protocol_id(acc);
    emit instance->accountDisconnected(str);
}

void PurpleInteraction::acc_disabled(PurpleAccount* acc) {
    AccountStruct str;
    str.username = purple_account_get_username(acc);
    str.protocol = purple_account_get_protocol_id(acc);
    emit instance->accountDisabled(str);
}

void PurpleInteraction::acc_enabled(PurpleAccount* acc) {
    AccountStruct str;
    str.username = purple_account_get_username(acc);
    str.password = purple_account_get_password(acc);
    str.protocol = purple_account_get_protocol_id(acc);
    emit instance->accountEnabled(str);
}

void PurpleInteraction::acc_error(PurpleAccount* acc, PurpleConnectionError err, const gchar* desc) {
    if (err) {
        AccountStruct str;
        str.username = purple_account_get_username(acc);
        str.protocol = purple_account_get_protocol_id(acc);
        emit instance->accountConnectError(str, desc);
    }
}

void PurpleInteraction::cont_changed(PurpleBuddy* buddy) {
    AccountStruct acc;
    acc.username = purple_account_get_username(buddy->account);
    acc.protocol = purple_account_get_protocol_id(buddy->account);
    ContactStruct cont;
    if (buddy->alias && strlen(buddy->alias))
        cont.alias = buddy->alias;
    else
        cont.alias = buddy->server_alias;
    cont.name = buddy->name;
    size_t img_size;
    if (purple_buddy_get_icon(buddy)) {
        const unsigned char* icon = (const unsigned char*)purple_buddy_icon_get_data(purple_buddy_get_icon(buddy), &img_size);
        if (icon) {
            purple_buddy_icon_ref(purple_buddy_get_icon(buddy));
            if (QImageReader::supportedImageFormats().contains(QByteArray(purple_buddy_icon_get_extension(purple_buddy_get_icon(buddy))).toUpper())) {
                cont.avatar = QImage::fromData(icon, img_size, purple_buddy_icon_get_extension(purple_buddy_get_icon(buddy)));
            } else {
                cont.avatar = QImage::fromData(icon, img_size);
            }
        }
    }
    emit instance->contactChanged(acc, buddy->name, cont);
}

void PurpleInteraction::on_purple_quit() {
    emit instance->finished();
}

void PurpleInteraction::connect_to_signals() {
    static int handle;
    // Purple Accounts
    purple_signal_connect(purple_accounts_get_handle(), "account-connecting", &handle,
        PURPLE_CALLBACK(acc_connecting), NULL);
    purple_signal_connect(purple_accounts_get_handle(), "account-signed-on", &handle,
        PURPLE_CALLBACK(acc_signed_on), NULL);
    purple_signal_connect(purple_accounts_get_handle(), "account-signed-off", &handle,
        PURPLE_CALLBACK(acc_signed_off), NULL);
    purple_signal_connect(purple_accounts_get_handle(), "account-disabled", &handle,
        PURPLE_CALLBACK(acc_disabled), NULL);
    purple_signal_connect(purple_accounts_get_handle(), "account-enabled", &handle,
        PURPLE_CALLBACK(acc_enabled), NULL);
    purple_signal_connect(purple_accounts_get_handle(), "account-connection-error", &handle,
        PURPLE_CALLBACK(acc_error), NULL);
    // Purple Buddies
    purple_signal_connect(purple_blist_get_handle(), "buddy-icon-changed", &handle,
        PURPLE_CALLBACK(cont_changed), NULL);
}

void PurpleInteraction::run() {
    signal(SIGCHLD, SIG_IGN);
    qRegisterMetaType<PurpleInteraction::AccountStruct>("PurpleInteraction::AccountStruct");
    qRegisterMetaType<AccountStruct>("AccountStruct");
    qRegisterMetaType<PurpleInteraction::ContactStruct>("PurpleInteraction::ContactStruct");
    qRegisterMetaType<ContactStruct>("ContactStruct");
    qRegisterMetaType<QList<PurpleInteraction::ContactStruct> >("QList<PurpleInteraction::ContactStruct>");
    qRegisterMetaType<QList<ContactStruct> >("QList<ContactStruct>");
    qRegisterMetaType<PurpleInteraction::MessageStruct>("PurpleInteraction::MessageStruct");
    qRegisterMetaType<MessageStruct>("MessageStruct");
    int initResult = init_libpurple();
    bool successfulInit = !initResult;
    emit init(successfulInit);
    if (successfulInit) { // Зʼєднуємося із сигналами
        connect_to_signals();

#ifdef QT_DEBUG
        GList* protocols = purple_plugins_get_protocols();
        while (protocols) {
            qDebug() << ((PurplePlugin*)protocols->data)->info->id;
            protocols = protocols->next;
        }
#endif
    }
}

void PurpleInteraction::addAccount(AccountStruct acc) {
    PurpleAccount* account = purple_account_new(strconv(acc.username), strconv(acc.protocol));
    purple_account_set_password(account, strconv(acc.password));
    purple_accounts_add(account);
    purple_account_set_enabled(account, UI_ID, acc.enabled);
}

void PurpleInteraction::putOnline(AccountStruct acc) {
    PurpleAccount* account = purple_accounts_find(strconv(acc.username), strconv(acc.protocol));
    purple_account_set_status(account, purple_primitive_get_id_from_type(PURPLE_STATUS_AVAILABLE), true, NULL);
}

void PurpleInteraction::putInvisible(AccountStruct acc) {
    PurpleAccount* account = purple_accounts_find(strconv(acc.username), strconv(acc.protocol));
    purple_account_set_status(account, purple_primitive_get_id_from_type(PURPLE_STATUS_INVISIBLE), true, NULL);
}

void PurpleInteraction::changeEnabled(AccountStruct acc) {
    PurpleAccount* account = purple_accounts_find(strconv(acc.username), strconv(acc.protocol));
    purple_account_set_enabled(account, UI_ID, acc.enabled);
}

void PurpleInteraction::connectAccount(AccountStruct acc) {
    PurpleAccount* account = purple_accounts_find(strconv(acc.username), strconv(acc.protocol));
    purple_account_connect(account);
}

void PurpleInteraction::createConversation(PurpleInteraction::AccountStruct acc, QString cont) {
    PurpleAccount* account = purple_accounts_find(strconv(acc.username), strconv(acc.protocol));
    purple_conversation_new(PURPLE_CONV_TYPE_IM, account, strconv(cont));
}

void PurpleInteraction::sendMessage(PurpleInteraction::AccountStruct acc, QString cont, QString message) {
    PurpleAccount* account = purple_accounts_find(strconv(acc.username), strconv(acc.protocol));
    PurpleConvIm* conv = PURPLE_CONV_IM(purple_find_conversation_with_account(PURPLE_CONV_TYPE_IM, strconv(cont), account));
    purple_conv_im_send(conv, strconv(message));
}

void PurpleInteraction::message_recieved(PurpleConversation* conv, const char*, const char*, const char* message, PurpleMessageFlags flags, time_t mtime) {
    if (flags & PURPLE_MESSAGE_ERROR) {
#ifdef QT_DEBUG
        qDebug() << conv->name << " (" << QDateTime::fromTime_t(mtime).toString() << "): " << message << "\n";
#endif
    } else if (conv) {
        AccountStruct acc;
        acc.username = purple_account_get_username(conv->account);
        acc.protocol = purple_account_get_protocol_id(conv->account);
        MessageStruct msg;
        msg.text = message;
        msg.time = QDateTime::fromTime_t(mtime);
        msg.type = (flags & PURPLE_MESSAGE_SEND) ? MessageStruct::Outcoming : MessageStruct::Incoming;
        emit instance->messageRecieved(acc, conv->name, msg);
    }
}
