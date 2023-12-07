#pragma once
#include <QObject>
#include <QTranslator>

class QAction;
class QActionGroup;

class Localizer : public QObject
{
    Q_OBJECT
private:
    QTranslator m_translator;
    QTranslator m_translatorQt;
    QString m_currLang;
    QActionGroup* m_languages;
public:
    Localizer();
    void init(QWidget* mainWidget);
    bool switchTranslator(QTranslator& translator, const QString& filename);
    bool switchTranslator(QTranslator& translator, const QString& filename, const QLocale &locale);
    void loadLanguage(const QString& rLanguage);
    QList<QAction *> languageList() const;
    const QActionGroup* languageGroup() const;
    QAction* currentLanguage() const;
    QAction* language(const QString& lang) const;
public slots:
    void changeLanguage(QAction* action);
};
