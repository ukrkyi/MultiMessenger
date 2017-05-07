TEMPLATE = subdirs
CONFIG += cd_change_global
SUBDIRS = MultiMessenger \
QPurple
MultiMessenger.subdir = MultiMessenger
QPurple.subdir = QPurple
MultiMessenger.depends = QPurple
