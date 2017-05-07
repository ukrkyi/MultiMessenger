# MultiMessenger
Багатосервісна програма-листувальник // Multi-service messenger

This program is designed to simplify conversations with one person using different messaging services. It uses `libpurple` as a background, therefore supports as many services as libpurple does.
In 1.0 version, only Facebook Messenger and Google Hangouts are implemented.
Any improvements will be appreciated.

Програма створена з метою полегшити спілкування з однією людиною, використовуючи різні сервіси миттєвих повідомлень. Для фонової взаємодії використовується `libpurple`, тож у преспективі можлива підтримка всіх підтримуваних нею сервісів.
У версії 1.0, підтримуються Facebook Messenger та Google Hangouts (для роботи FB Messenger необхідно встановити доповнення `purple-facebook`).
Будь-які доопрацювання вітаються.

***

### MultiMessenger

Основна програма. Інтерфейс інтуітивно зрозумілий і не потребує пояснень.

### QPurple

Обгортка Qt для LibPurple. Реалізована як singleton-клас з використанням сигналів Qt.

Qt wrapper for libpurple.

