#pragma once

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <QThread>
#include <QToolButton>
#include <QUrl>
#include <qaction.h>
#include <interface/identifiable.h>
#include <interface/initable.h>
#include <QFileDialog>
#include "render/spritebuilder.h"

namespace Ui {
class GameTable;
}

namespace ns_descriptor {
    struct GameTable;
}
class TableOver;
class UserControls;

class GameTable : public InitableDialog, public GameIdObject
{
    Q_OBJECT

    Q_PROPERTY(bool editorOpenedByDefault MEMBER m_editorOpenedByDefault)
private:
    Ui::GameTable *m_ui;
    const GameId m_gameID;
    QPixmap m_glRTTObject;
    bool m_isNowResetting;

    QSize m_withoutEditorSize;
    QSize m_withEditorSize;

    bool m_needClose;
    bool m_editorOpenedByDefault;
public:
    explicit GameTable(const GameId& id, QWidget *parent = 0);

    virtual ~GameTable();
    TableOver* layers() const;
    UserControls* hud() const;
signals:
    void glRenderedToTexture(const QPixmap& pixmap);
//    void signalBttnClicked(QAbstractButton* bttn);
public slots:
    void setEditorVisible(bool visible = true);
    void updateCurrentItemPosition(const QVector3D& pos);
    void updateCurrentItemScale(const QVector3D& scale);
    void updateCurrentItemRotation(const QQuaternion& quat);
    void updateCurrentItemOpacity(qreal opacity);

    void chatPlayersMsg(const QString& name, const QString& msg);

    void applyCurrentGameSettings();
public:
    virtual void init() override;

    void setSitCount(int sitCount);

    void displayEndOfRound(gbp_i8 roundNumber);
    void displayEndOfDistribution();
    void displayEndOfGame();

    void displayWinningAt(gbp_i8 pos
                         , gbp_i64 total
                         , gbp_i64 gain
                         , gbp_i8 potNumber
                         , gbp::ns_api::ns_table::e_cards_combination combination
                         , const std::vector<gbp_i8> &cards
                         , bool showCombination = false);

    void closeTable();
private slots:
    void onComboBoxIndexChanged(int index);
    void setupRenderedObjects(SpriteBuilder* builder);
    void updateRTTObject(const QPixmap& pixmap);
private:
    void updateTitle();
    void beginReset() { m_isNowResetting = true; }
    void endReset() { m_isNowResetting = false; }
    bool isNowResetting() const { return m_isNowResetting; }
    double sumOrMul(double a, double b);
    double subOrDiv(double a, double b);

protected:
    virtual void closeEvent(QCloseEvent *e) override;
    virtual void changeEvent(QEvent *) override;
    virtual void actionEvent(QActionEvent *e) override;

    // QDialog interface
public slots:
    virtual void accept() override;
    virtual void reject() override;
};
