TEMPLATE = subdirs

# Projects->Build_steps->additional_arguments:
#"CONFIG+=dynamic_build" "DEFINES+=GBP_DECLARE_TYPE_GEN_ADDITIONALS"
#
# Sanitizer:
#"QMAKE_CXXFLAGS += -fno-omit-frame-pointer -fsanitize=address -fuse-ld=gold"
#"QMAKE_LFLAGS += -fno-omit-frame-pointer -fsanitize=address -fuse-ld=gold -L/usr/local/lib"

CONFIG += ordered

SUBDIRS += \
           tools/tools \
           SMTPEmail \
           EMailSender \
           common/rpcbase \
           api_models/api_models \
           basewidgets/basewidgets \
           poker_client/src \
           plugins/Mappers \
           animation/animation \
           gui \
           poker_client

TRANSLATIONS_PATH = $$PWD/resources/unpacked/translations
TRANSLATIONS += $$TRANSLATIONS_PATH/lang_en.ts
TRANSLATIONS += $$TRANSLATIONS_PATH/lang_ru.ts
TRANSLATIONS += $$TRANSLATIONS_PATH/lang_uk.ts


#system(mkdir ./gen_tmp_dir)

#!exists($$PWD/gen_tmp_dir) {
#    message($$PWD/gen_tmp_dir do not exists)
##    mkpath($$PWD/gen_tmp_dir)
#}

#exists($$PWD/gen_tmp_dir) {
#    message($$PWD/gen_tmp_dir exists)
#} else {
#    DEFINES+=GBP_DECLARE_TYPE_DISABLE_DECLARE_TYPE
#}

RESOURCES += \
    resources/unpacked/r.qrc
