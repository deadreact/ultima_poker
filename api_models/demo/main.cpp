#include <QApplication>
#include <form.h>

int main(int argc, char *argv[])
{
    QApplication::setDesktopSettingsAware(true);


    QApplication a(argc, argv);
    QApplication::setEffectEnabled(Qt::UI_General,        true);
    QApplication::setEffectEnabled(Qt::UI_AnimateMenu,    true);
    QApplication::setEffectEnabled(Qt::UI_FadeMenu,       true);
    QApplication::setEffectEnabled(Qt::UI_AnimateCombo,   true);
    QApplication::setEffectEnabled(Qt::UI_AnimateTooltip, true);
    QApplication::setEffectEnabled(Qt::UI_FadeTooltip,    true);
    QApplication::setEffectEnabled(Qt::UI_AnimateToolBox, true);




    Form form;
    form.show();

    return a.exec();
}
